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

#ifdef CFG_NIXIE

#include "nixie/nixie.h"
#include "nixie/nixie_clock.h"
#include "print.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void cmd_nixie_test(uint8_t argc, char **argv)
{
  /*
  nixieDisplay_t display;

  for(uint8_t i = 0; i < 10; i++)
  {
    display.digits[0] = i;
    display.digits[1] = i;
    display.digits[2] = i;
    display.digits[3] = i;
    display.digits[4] = i;
    display.digits[5] = i;
    nixieDisplay( &display );
    delay(1000);
  }
  */
  print("%s%s", "OK", CFG_PRINTF_NEWLINE);
}


void cmd_nixie_set_type(uint8_t argc, char **argv)
{
  char* end;
  int32_t mapping = strtol(argv[1], &end, 10);

  /* Make sure values are valid */
  if ((mapping < 0) || (mapping >= NIXIE_TYPE_END))
  {
    print("%s: %s%s", "ERROR", "range", CFG_PRINTF_NEWLINE);
    return;
  }

  nixieMapping_t m = mapping;
  nixieStoreMapping(m);
  nixieSetMapping(m);
}

void cmd_nixie_get_type(uint8_t argc, char **argv)
{
  nixieMapping_t m = nixieGetMapping();
  print("%s: %02d%s", "TYPE", m, CFG_PRINTF_NEWLINE);
}

void cmd_nixie_set_mode(uint8_t argc, char **argv)
{
  char* end;
  int32_t mode = strtol(argv[1], &end, 10);

  /* Make sure values are valid */
  if ((mode < 0) || (mode >= nclock_mode_end))
  {
    print("%s: %s%s", "ERROR", "range", CFG_PRINTF_NEWLINE);
    return;
  }

  nclock_mode_t m = mode;
  nixieClockStoreMode(m);
  nixieClockSetMode(m);
}

void cmd_nixie_get_mode(uint8_t argc, char **argv)
{
  nclock_mode_t m = nixieClockGetMode();
  print("%s: %02d%s", "MODE", m, CFG_PRINTF_NEWLINE);
}

#endif
