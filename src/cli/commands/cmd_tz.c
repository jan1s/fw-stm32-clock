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

#include "rtc/tz.h"
#include "cli/cli.h"
#include "print.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void cmd_tz_write(cli_select_t t, uint8_t argc, char **argv)
{
    bool isStd = false;
    bool isDst = false;
    if(strncmp(argv[0], "std", 3) == 0)
    {
        isStd = true;
    }
    else if(strncmp(argv[0], "dst", 3) == 0)
    {
        isDst = true;
    }
    else
    {
        print(cli_send[t], "%s%s", "Must be either STD or DST", CFG_PRINTF_NEWLINE);
        return;
    }

    char* end;
    int32_t offset = strtol(argv[1], &end, 10);
    int32_t hour = strtol(argv[2], &end, 10);
    int32_t dow = strtol(argv[3], &end, 10);
    int32_t week = strtol(argv[4], &end, 10);
    int32_t month = strtol(argv[5], &end, 10);

    /* Make sure values are valid */
    if ((offset < -1440) || (offset > 1440))
    {
        print(cli_send[t], "%s%s", "Offset must be between -1440 and 1440 minutes", CFG_PRINTF_NEWLINE);
        return;
    }
    if ((hour < 0) || (hour > 23))
    {
        print(cli_send[t], "%s%s", "Hour must be between 0 and 23", CFG_PRINTF_NEWLINE);
        return;
    }
    if ((dow < 0) || (dow > 6))
    {
        print(cli_send[t], "%s%s", "Day of week must be between 0 and 6", CFG_PRINTF_NEWLINE);
        return;
    }
    if ((week < 0) || (week > 4))
    {
        print(cli_send[t], "%s%s", "Week must be between 0 and 4", CFG_PRINTF_NEWLINE);
        return;
    }
    if ((month < 0) || (month > 12))
    {
        print(cli_send[t], "%s%s", "Month must be between 0 and 12", CFG_PRINTF_NEWLINE);
        return;
    }

    /* Try to create a rule */
    tzRule_t r;
    tzCreateRule(offset, hour, dow, week, month, &r);

    /* Set and store the rule */
    if(isStd)
    {
        tzStoreSTD(&r);
        tzSetSTD(&r);
    }
    else if(isDst)
    {
        tzStoreDST(&r);
        tzSetDST(&r);
    }
    print(cli_send[t], "%s%s", "OK", CFG_PRINTF_NEWLINE);
}

void cmd_tz_read(cli_select_t t, uint8_t argc, char **argv)
{
    bool isStd = false;
    bool isDst = false;

    if(argc == 1)
    {
        if(strncmp(argv[0], "std", 3) == 0)
        {
            isStd = true;
        }
        else if(strncmp(argv[0], "dst", 3) == 0)
        {
            isDst = true;
        }
        else
        {
            print(cli_send[t], "%s%s", "Must be either STD or DST", CFG_PRINTF_NEWLINE);
            return;
        }
    }
    else
    {
        isStd = true;
        isDst = true;
    }

    tzRule_t r;
    if(isStd)
    {
        tzGetSTD(&r);
        print(cli_send[t], "%s: %04d %02d %01d %02d %02d%s", "STD", r.offset, r.hour, r.dow, r.week, r.month, CFG_PRINTF_NEWLINE);
    }
    if(isDst)
    {
        tzGetDST(&r);
        print(cli_send[t], "%s: %04d %02d %01d %02d %02d%s", "DST", r.offset, r.hour, r.dow, r.week, r.month, CFG_PRINTF_NEWLINE);
    }
}
