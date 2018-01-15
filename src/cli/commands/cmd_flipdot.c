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

#ifdef CFG_FLIPDOT

#include "flipdot/flipdot.h"
#include "flipdot/flipdot_clock.h"
#include "timer.h"
#include "cli/cli.h"
#include "print.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void cmd_flipdot_test(cli_select_t t, uint8_t argc, char **argv)
{
    print(cli_send[t], "%s%s", "OK", CFG_PRINTF_NEWLINE);
}

void cmd_flipdot_set_mode(cli_select_t t, uint8_t argc, char **argv)
{
    char* end;
    int32_t mode = strtol(argv[0], &end, 10);

    /* Make sure values are valid */
    if ((mode < 0) || (mode >= FLIPDOTCLOCK_MODE_END))
    {
        print(cli_send[t], "%s: %s%s", "ERROR", "range", CFG_PRINTF_NEWLINE);
        return;
    }

    flipdotclockMode_t m = mode;
    flipdotclockStoreMode(m);
    flipdotclockSetMode(m);
    print(cli_send[t], "%s%s", "OK", CFG_PRINTF_NEWLINE);
}

void cmd_flipdot_get_mode(cli_select_t t, uint8_t argc, char **argv)
{
    flipdotclockMode_t m = flipdotclockGetMode();
    print(cli_send[t], "%s: %02d%s", "MODE", m, CFG_PRINTF_NEWLINE);
}

#endif
