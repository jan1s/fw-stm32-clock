/**************************************************************************/
/*!
    @file     gps.c
    @author   Janis (jan1s@github)

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

#ifdef CFG_GPS

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "gps.h"

#ifdef CFG_GPS_UART
#include "core/uart/uart.h"
#endif

#if CFG_GPS_INT_ENABLE == 1
#include "core/gpio/gpio.h"
#endif

static uint8_t gps_buffer[CFG_GPS_MAXMSGSIZE];
static uint8_t *gps_buffer_ptr;

static uint8_t pinState = 0;

static gprmc_t rmc;
static void (*_gpsCallback)(void) = NULL;


/**************************************************************************/
/*!
    @brief Initialises the gps reception using the appropriate interface
*/
/**************************************************************************/
void gpsInit()
{
#if defined CFG_GPS && defined CFG_GPS_UART
    LPC_IOCON->PIO1_13        = 0x3;            // RXD
    LPC_IOCON->PIO1_14        = 0x3;            // TXD

    // Check if UART is already initialised
    uart_pcb_t *pcb = uartGetPCB();
    if (!pcb->initialised)
    {
        uartInit(CFG_UART_BAUDRATE);
    }
#endif

    /* INT is open drain so we need to enable the pullup on the pin */
    LPC_IOCON->TRST_PIO0_14 = (0 << 0) | (2 << 3) | (1 << 7);

    /* Init the gps_buffer ptr */
    gps_buffer_ptr = gps_buffer;
}

/**************************************************************************/
/*!
    @brief  Polls the relevant incoming message queue to see if anything
            is waiting to be processed.
*/
/**************************************************************************/
void gpsPoll()
{
#ifdef CFG_GPS_UART
    while (uartRxBufferDataPending())
    {
        uint8_t c = uartRxBufferRead();
        gpsRx(c);
    }
#endif

    /* Get the pinState for the defined pin */
    uint8_t pinStateNow = LPC_GPIO->B0[CFG_GPS_PIN];

    /* Edge detection */
    if(pinStateNow != pinState)
    {
        /* Rising edge */
        if(pinStateNow != 0)
        {
            /* Call the callback function if present */
            if (NULL != _gpsCallback )
            {
                _gpsCallback();
            }
        }
    }
}

/**************************************************************************/
/*!
    @brief  Handles a single incoming character.  If a new line is
            detected, the entire command will be passed to the command
            parser.  If a text character is detected, it will be added to
            the message buffer until a new line is detected (up to the
            maximum queue size, CFG_GPS_MAXMSGSIZE).

    @param[in]  c
                The character to parse.
*/
/**************************************************************************/
void gpsRx(uint8_t c)
{
    switch (c)
    {
    case '\n':
        /* Terminate the buffer and reset the ptr */
        /* Send it to the handler for processing */
        *gps_buffer_ptr = '\0';
        nmeaParse((char *)gps_buffer);
        gps_buffer_ptr = gps_buffer;
        break;

    case '$':
        /* Beginning of a NMEA packet */
        break;

    default:
        /* Received a single character */
        *gps_buffer_ptr++ = c;
        break;
    }
}


error_t gpsTime(rtcTime_t *utc)
{
    if(!rmc.valid)
    {
        /* ToDo: use proper error type */
        return 1;
    }

    uint16_t year = (rmc.date % 100) + 100;
    uint8_t month = (rmc.date / 100) % 100;
    uint8_t day = (rmc.date / 10000) % 100;

    uint8_t hour = ((uint32_t)rmc.time / 10000) % 100;
    uint8_t minute = ((uint32_t)rmc.time / 100) % 100;
    uint8_t second = ((uint32_t)rmc.time % 100);

    /* ToDo: check for error */
    ASSERT_STATUS(rtcCreateTime(year, month, day, hour, minute, second, 0, utc));

    return ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Parse the NMEA string. This function tokenizes the command
            input, then searches for corresponding NMEA sentence.

    @param[in]  cmd
                The entire command string to be parsed
*/
/**************************************************************************/
bool nmeaParse(char *nmea)
{
    /* Check if sentence complete */
    if (nmea[strlen(nmea) - 4] != '*')
    {
        return false;
    }

    /* ToDo: checksum check */
    /*
    uint16_t sum = parseHex(nmea[strlen(nmea)-3]) * 16;
    sum += parseHex(nmea[strlen(nmea)-a]);
    for (uint8_t i=1; i < (strlen(nmea)-4); i++)
    {
        sum ^= nmea[i];
    }
    if (sum != 0)
    {
        return false;
    }
    */

    /* Parse GPRMC sentence */
    if (strstr(nmea, "GPRMC"))
    {
        char *p = nmea;

        float timef;
        bool fix;
        float latitude;
        float longitude;
        //float speed;
        //float coarse;
        uint32_t date;

        p = strchr(p, ',') + 1;
        timef = atof(p);

        p = strchr(p, ',') + 1;
        if (p[0] == 'A')
        {
            fix = true;
        }
        else if (p[0] == 'V')
        {
            fix = false;
        }
        else
        {
            return false;
        }

        p = strchr(p, ',') + 1;
        latitude = atof(p);

        p = strchr(p, ',') + 1;
        if (p[0] == 'S')
        {
            latitude *= -1.0;
        }
        else if (p[0] == 'N')
        {
            latitude = latitude;
        }
        else if (p[0] == ',')
        {
            latitude = 0.0;
        }
        else
        {
            return false;
        }

        p = strchr(p, ',') + 1;
        longitude = atof(p);

        p = strchr(p, ',') + 1;
        if (p[0] == 'W')
        {
            longitude *= -1.0;
        }
        else if (p[0] == 'E')
        {
            longitude = longitude;
        }
        else if (p[0] == ',')
        {
            longitude = 0.0;
        }
        else
        {
            return false;
        }

        p = strchr(p, ',') + 1;
        //speed = atof(p);

        p = strchr(p, ',') + 1;
        //coarse = atof(p);

        p = strchr(p, ',') + 1;
        date = atol(p);

        rmc.time = timef;
        rmc.date = date;
        rmc.valid = fix;
        rmc.latitude = latitude;
        rmc.longitude = longitude;

        return true;
    }

    return false;
}

/**************************************************************************/
/*!
    @brief  Registers the optional callback function that will be called
            eve

    @section EXAMPLE

    @code

    uint32_t _gpsCounter;
    void gpsCounter(void)
    {
      _gpsCounter++;
    }

    // ...

    // Setup an INT callback to rtcCounter
    gpsSetCallback (&gpsCounter);

    @endcode
*/
/**************************************************************************/
void gpsSetCallback (void (*pFunc)(void))
{
    _gpsCallback = pFunc;
}


#endif
