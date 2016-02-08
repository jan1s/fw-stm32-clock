/**************************************************************************/
/*!
    @file     cmd_nixie.c
    @author   Janis (jan1s@github)

    @brief    Sets the display of the Nixie
    @ingroup  CLI

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013, Janis (jan1s@github)
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

#include "rtc/rtc.h"
#include "rtc/rtc_functions.h"
#include "print.h"

#include <stdio.h>
#include <stdlib.h>

void cmd_rtc_write(uint8_t argc, char **argv)
{
    char* end;
    int32_t year = strtol(argv[0], &end, 10);
    int32_t month = strtol(argv[1], &end, 10);
    int32_t day = strtol(argv[2], &end, 10);
    int32_t hour = strtol(argv[3], &end, 10);
    int32_t minute = strtol(argv[4], &end, 10);
    int32_t second = strtol(argv[5], &end, 10);

    /* Make sure values are valid */
    if ((year < 2000) || (year > 2038))
    {
        print("%s%s", "Year must be between 2000 and 2023", CFG_PRINTF_NEWLINE);
        return;
    }
    if ((month < RTC_MONTHS_JANUARY) || (month > RTC_MONTHS_DECEMBER))
    {
        print("%s%s", "Month must be between 1 and 12", CFG_PRINTF_NEWLINE);
        return;
    }
    if ((day < 1) || (day > 31))
    {
        print("%s%s", "Day must be between 1 and 31", CFG_PRINTF_NEWLINE);
        return;
    }
    if ((hour < 0) || (hour > 23))
    {
        print("%s%s", "Hour must be between 0 and 23", CFG_PRINTF_NEWLINE);
        return;
    }
    if ((minute < 0) || (minute > 59))
    {
        print("%s%s", "Minute must be between 0 and 59", CFG_PRINTF_NEWLINE);
        return;
    }
    if ((second < 0) || (second > 59))
    {
        print("%s%s", "Second must be between 0 and 59", CFG_PRINTF_NEWLINE);
        return;
    }

    /* Try to create a date */
    rtcTime_t t;
    uint8_t error = rtcCreateTime(year, month, day, hour, minute, second, 0, &t);
    if (error)
    {
        print("%s%s", "Invalid timestamp", CFG_PRINTF_NEWLINE);
        return;
    }

    /* Write the time to the RTC */
    uint32_t epoch = rtcToEpochTime (&t);
    rtcSet(epoch);
}

void cmd_rtc_read(uint8_t argc, char **argv)
{
    uint32_t epoch = rtcGet();

    rtcTime_t t;
    rtcCreateTimeFromEpoch(epoch, &t);

    print("%s: %04d %02d %02d %02d %02d %02d%s", "UTC", t.years + 1900, t.months + 1, t.days, t.hours, t.minutes, t.seconds, CFG_PRINTF_NEWLINE);
}
