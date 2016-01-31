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

#include "projectconfig.h"

#ifdef CFG_DCF

#include <stdio.h>

#include "dcf.h"
#include "core/gpio/gpio.h"
#include "core/delay/delay.h"


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

static bool dcfInSync = false;
static uint64_t dcfBitwurst = 0;
static uint8_t dcfBitwurstIndex = 0;

static uint8_t pinState = 0;
static uint32_t edgeTime = 0;

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
    //LPC_IOCON->TRST_PIO0_14 = (0<<0) | (2<<3) | (1<<7);
    LPC_IOCON->TRST_PIO0_14 = (1 << 0) | (0 << 3) | (1 << 7);

    /* Set interrupt/gpio pin to input */
    LPC_GPIO->DIR[CFG_DCF_PORT] &= ~(1 << CFG_DCF_PIN);
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
    uint8_t pinStateNow = LPC_GPIO->B0[CFG_DCF_PIN];

    /* Edge detection */
    if(pinStateNow != pinState)
    {
        uint32_t edgeTimeNow = delayGetTicks();
        uint32_t edgeTimeDiff = edgeTimeNow - edgeTime;
        /* Rising edge */
        if(pinStateNow != 0)
        {
            LPC_GPIO->CLR[0] = (1 << 18);

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
            LPC_GPIO->SET[0] = (1 << 18);

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
error_t dcfTime(rtcTime_t *utc)
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
    ASSERT_STATUS(rtcCreateTime(year, month, day, hour, minute, second, 0, utc));

    return ERROR_NONE;
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


#endif
