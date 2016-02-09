/**************************************************************************/
/*!
    @file     dcf.c
    @author   Janis (jan1s@github)
    @author   Johannes

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013 Janis (jan1s@github)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
/**************************************************************************/

#include "platform_config.h"

#include <stdio.h>
#include <stdbool.h>

#include "rtc/dcf.h"
#include "led.h"
#include "timer.h"

typedef struct
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t dow;
    uint8_t week;
    uint8_t month;
    uint8_t year;
    bool valid;
} dcf_t;

static void (*_dcfCallback)(void) = NULL;

static bool dcfInSync;
static uint64_t dcfBitwurst;
static uint8_t dcfBitwurstIndex;

static uint8_t pinState;
static uint32_t edgeTime;

static dcf_t dcf;

bool dcfParse(uint64_t bitwurst);
bool dcfParity(uint32_t bits, uint8_t len);
uint8_t dcfBCDToDec(uint8_t val);


/**************************************************************************/
/*!
    @brief Initialises the dcf reception using the appropriate interface
*/
/**************************************************************************/
void dcfInit()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /* RX Pin */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    dcfInSync = false;
    dcfBitwurst = 0;
    dcfBitwurstIndex = 0;

    pinState = 0;
    edgeTime = 0;

    dcf.valid = false;
}

/**************************************************************************/
/*!
    @brief  Polls the relevant incoming message queue to see if anything
            is waiting to be processed. This needs to be called with
            relatively high frequency, to ensure proper decoding. (>100Hz)
*/
/**************************************************************************/
void dcfPoll()
{
    uint8_t pinStateNow = GPIO_ReadInputDataBit((GPIO_TypeDef *)GPIOA_BASE, GPIO_Pin_10);

    /* Edge detection */
    if(pinStateNow != pinState)
    {
        uint32_t edgeTimeNow = timer_delayCount;
        uint32_t edgeTimeDiff = edgeTimeNow - edgeTime;
        /* Rising edge */
        if(pinStateNow != 0)
        {
            led_usr_on();

            /* Detect minute sync */
            if((edgeTimeDiff < 2000) && (edgeTimeDiff > 1700))
            {
                /* If already in sync: full minute detected */
                if(dcfInSync)
                {
                    dcf.valid = dcfParse(dcfBitwurst);
                }

                /* Marks the beginning of a minute */
                dcfInSync = true;
                dcf.second = 0;
                dcfBitwurstIndex = 0;
            }
            /* Detect begin of second */
            else if((edgeTimeDiff < 1000) && (edgeTimeDiff > 700))
            {
                /* Call the callback function if present */
                if (NULL != _dcfCallback )
                {
                    _dcfCallback();
                }
            }
            /* Unexpected pulse */
            else
            {
                dcfInSync = false;
                dcf.valid = false;
            }
        }
        /* Falling edge */
        else
        {
            led_usr_off();

            /* Only work on dcfBitwurst if sure where to work */
            if(dcfInSync)
            {
                /* Detect 1 */
                if((edgeTimeDiff < 250) && (edgeTimeDiff > 150))
                {
                    dcfBitwurst |= ((uint64_t)1 << dcfBitwurstIndex);
                }
                /* Detect 0 */
                else if ((edgeTimeDiff < 150) && (edgeTimeDiff > 50))
                {
                    dcfBitwurst &= ~((uint64_t)1 << dcfBitwurstIndex);
                }
                /* Unexpected pulse */
                else
                {
                    dcfInSync = false;
                    dcf.valid = false;
                }

                dcf.second++;
                dcfBitwurstIndex++;
            }
        }

        edgeTime = edgeTimeNow;
        pinState = pinStateNow;
    }

}

/**************************************************************************/
/*!
    @brief  Parses bitwurst into static dcf structure. Returns false if
    		parity checks failed.
*/
/**************************************************************************/
bool dcfParse(uint64_t bitwurst)
{
    uint8_t minutesBCD = (uint8_t)((bitwurst & (0xFF << 21)) >> 21);
    if(!dcfParity(minutesBCD, 7)) return false;
    dcf.minute = dcfBCDToDec((uint8_t)minutesBCD & 0x7F);

    uint8_t hoursBCD = (uint8_t)((bitwurst & ((uint64_t)0x7F << 29)) >> 29);
    if(!dcfParity(hoursBCD, 6)) return false;
    dcf.hour = dcfBCDToDec((uint8_t)hoursBCD & 0x3F);

    uint64_t calBCD = ((bitwurst & ((uint64_t)0x7FFFFF << 36)) >> 36);
    if(!dcfParity(calBCD, 23)) return false;

    dcf.day = dcfBCDToDec((uint8_t)calBCD & 0x3F);

    calBCD = calBCD >> 6;
    dcf.dow = calBCD & 0x07;

    calBCD = calBCD >> 3;
    dcf.month = dcfBCDToDec((uint8_t)calBCD & 0x1F);

    calBCD = calBCD >> 5;
    dcf.year = dcfBCDToDec((uint8_t)calBCD & 0xFF);

    return true;
}

/**************************************************************************/
/*!
    @brief  Returns the current time received by the DCF
*/
/**************************************************************************/
uint8_t dcfTime(rtcTime_t *local)
{
    if(!dcf.valid)
    {
        return 1; /* ToDo: use proper error type */
    }

    uint16_t year = dcf.year + 100;
    uint8_t month = dcf.month;
    uint8_t day = dcf.day;

    uint8_t hour = dcf.hour;
    uint8_t minute = dcf.minute;
    uint8_t second = dcf.second;

    /* ToDo: check for error */
    rtcCreateTime(year, month, day, hour, minute, second, 0, local);

    return 0;
}

/**************************************************************************/
/*!
    @brief  Checks the parity of a given set of bits. The last bit at len
    		is the parity bit.
*/
/**************************************************************************/
bool dcfParity(uint32_t bits, uint8_t len)
{
    uint8_t result = 0;
    for(uint8_t i = 0; i < len; i++)
    {
        result ^= (bits >> i) & 0x01;
    }
    return (result == ((bits >> len) & 0x01));
}

/**************************************************************************/
/*!
    @brief  Converts BCD value received by DCF to decimal value
*/
/**************************************************************************/
uint8_t dcfBCDToDec(uint8_t val)
{
    return (val >> 4) * 10 + (val & 0x0F);
}

/**************************************************************************/
/*!
    @brief  Registers the optional callback function that will be called
            in the interrupt handler
*/
/**************************************************************************/
void dcfSetCallback (void (*pFunc)(void))
{
    _dcfCallback = pFunc;
}
