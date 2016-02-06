/**************************************************************************/
/*!
    @file     cli_tbl.h
    @author   K. Townsend (microBuilder.eu), Janis (jan1s@github)

    @brief    Command lookup table for the CLI
    @ingroup  CLI

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013, K. Townsend
    Modified work Copyright 2016, Janis
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

#ifndef __CLI_TBL_H__
#define __CLI_TBL_H__

#include "platform_config.h"

#include <stdio.h>

#define CMD_COUNT (sizeof(cli_tbl)/sizeof(cli_t))
#define CMD_NOPARAMS ( "This command has no parameters" )

/* Function prototypes for the command table */
void cmd_help(uint8_t argc, char **argv);         /* handled by cli/cli.c */
void cmd_sysinfo(uint8_t argc, char **argv);

void cmd_rtc_read(uint8_t argc, char **argv);
void cmd_rtc_write(uint8_t argc, char **argv);

/**************************************************************************/
/*!
    Command list for the command-line interpreter and the name of the
    corresponding method that handles the command.

    Note that a trailing ',' is required on the last entry, which will
    cause a NULL entry to be appended to the end of the table.
*/
/**************************************************************************/
cli_t cli_tbl[] =
{
    // command name, min args, max args, hidden, function name, command description, syntax
    { "?",            0,  0,  0, cmd_help                                   , "Help"                              , CMD_NOPARAMS },
    { "V",            0,  0,  0, cmd_sysinfo                                , "System Info"                       , CMD_NOPARAMS },
    { "tr",           0, 0,  0, cmd_rtc_read                                , "RTC read"                           , CMD_NOPARAMS },
    { "tw",           6, 7,  0, cmd_rtc_write                               , "RTC write"                          , "'tw <yr> <mon> <day> <hr> <min> <sec>'" },
};

#endif
