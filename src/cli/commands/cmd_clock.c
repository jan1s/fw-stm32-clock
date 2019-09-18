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

#include "clock.h"
#include "cli/cli.h"
#include "print.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void cmd_clock_set_source(cli_select_t t, uint8_t argc, char **argv)
{
    char* end;
    int32_t source = strtol(argv[1], &end, 10);

    /* Make sure values are valid */
    if ((source < 0) || (source >= CLOCK_SOURCE_END))
    {
      print(cli_send[t], "%s: %s%s", "ERROR", "range", CFG_PRINTF_NEWLINE);
      return;
    }

    clockSource_t s = source;
    clockStoreSource(s);
    clockSetSource(s);

    print(cli_send[t], "%s%s", "OK", CFG_PRINTF_NEWLINE);
}

void cmd_clock_get_source(cli_select_t t, uint8_t argc, char **argv)
{
	clockSource_t s = clockGetSource();
    print(cli_send[t], "%s: %02d%s", "SOURCE", s, CFG_PRINTF_NEWLINE);
}

void cmd_clock_set_nightmode(cli_select_t t, uint8_t argc, char **argv)
{
    char* end;
    int32_t mask = strtol(argv[0], &end, 10);
    int32_t startHour = strtol(argv[1], &end, 10);
    int32_t startMinute = strtol(argv[2], &end, 10);
    int32_t endHour = strtol(argv[3], &end, 10);
    int32_t endMinute = strtol(argv[4], &end, 10);

    /* Make sure values are valid */
    if ((mask < 0) || (mask > 0x7f))
    {
      print(cli_send[t], "%s%s", "Mask must be between 0 and 0x7f", CFG_PRINTF_NEWLINE);
      return;
    }
    if ((startHour < 0) || (startHour > 23))
    {
        print(cli_send[t], "%s%s", "Start hour must be between 0 and 23", CFG_PRINTF_NEWLINE);
        return;
    }
    if ((startMinute < 0) || (startMinute > 59))
    {
        print(cli_send[t], "%s%s", "Start minute must be between 0 and 59", CFG_PRINTF_NEWLINE);
        return;
    }
    if ((endHour < 0) || (endHour > 23))
    {
        print(cli_send[t], "%s%s", "End hour must be between 0 and 23", CFG_PRINTF_NEWLINE);
        return;
    }
    if ((endMinute < 0) || (endMinute > 59))
    {
        print(cli_send[t], "%s%s", "End minute must be between 0 and 59", CFG_PRINTF_NEWLINE);
        return;
    }

    nightModeRule_t m;
    m.dayMask = mask;
    m.startHour = startHour;
    m.startMinute = startMinute;
    m.endHour = endHour;
    m.endMinute = endMinute;

    clockStoreNightmode(m);
    clockSetNightmode(m);

    print(cli_send[t], "%s%s", "OK", CFG_PRINTF_NEWLINE);
}

void cmd_clock_get_nightmode(cli_select_t t, uint8_t argc, char **argv)
{
    nightModeRule_t m = clockGetNightmode();
    print(cli_send[t], "%02x %02d %02d %02d %02d%s", m.dayMask, m.startHour, m.startMinute, m.endHour, m.endMinute, CFG_PRINTF_NEWLINE);
}
