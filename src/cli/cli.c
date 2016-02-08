/*******************************************************************
    Original work Copyright (c) 2009 FreakLabs
    Modified work Copyright 2016, Janis
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.

    Originally written by Christopher Wang aka Akiba.
    Please post support questions to the FreakLabs forum.
*******************************************************************/

/**************************************************************************/
/*!
    @file     cli.c
    @author   Christopher Wang (Freaklabs)
              Modified by: K. Townsend (microBuilder.eu)
              Modified by: Janis (jan1s@github)

    @ingroup  CLI

    Original code taken from the FreakUSB Open Source USB Device Stack
    http://freaklabs.org/index.php/FreakUSB-Open-Source-USB-Device-Stack.html

    If it works well, you can thank Akiba at Freaklabs.  If it fails
    miserably, you can blame me (since parts of it it were rather
    ungraciously modified). :-)

*/
/**************************************************************************/

#include "platform_config.h"

#include <stdio.h>
#include <string.h>

#include "cli/cli.h"
#include "cli/cli_tbl.h"
#include "print.h"


#ifdef CFG_INTERFACE_USART1
#include "uart.h"
#endif

#ifdef CFG_INTERFACE_USBCDC
#include "usb_cdc.h"
#endif


#define KEY_CODE_BS         (8)     /* Backspace key code */
#define KEY_CODE_DEL        (127)   /* Delete key code */
#define KEY_CODE_ESC        (27)    /* Escape key code */
#define KEY_CODE_ENTER      (13)    /* Enter key code  */


static uint8_t cli_buffer[CFG_INTERFACE_MAXMSGSIZE];
static uint8_t *cli_buffer_ptr;

static void cliMenu();

/**************************************************************************/
/*!
    @brief Initialises the command line using the appropriate interface
*/
/**************************************************************************/
void cliInit()
{
    // Init USART1
    uart1Init();

    // init the cli_buffer ptr
    cli_buffer_ptr = cli_buffer;

    // Show the menu
    cliMenu();
}

/**************************************************************************/
/*!
    @brief  Polls the relevant incoming message queue to see if anything
            is waiting to be processed.
*/
/**************************************************************************/
void cliPoll()
{
#ifdef CFG_INTERFACE_USART1
    uint8_t c;
    while (uart1ReadChar(&c))
    {
        cliRx(c);
    }
#endif
}

/**************************************************************************/
/*!
    @brief  Handles a single incoming character.  If a new line is
            detected, the entire command will be passed to the command
            parser.  If a text character is detected, it will be added to
            the message buffer until a new line is detected (up to the
            maximum queue size, CFG_INTERFACE_MAXMSGSIZE).

    @param[in]  c
                The character to parse.
*/
/**************************************************************************/
void cliRx(uint8_t c)
{
    // read out the data in the buffer and echo it back to the host.
    switch (c)
    {
    case '\r':
#if CFG_INTERFACE_DROPCR == 1
        break;
#endif
    case '\n':
        // terminate the cli_buffer and reset the cli_buffer ptr. then send
        // it to the handler for processing.
        *cli_buffer_ptr = '\0';
#if CFG_INTERFACE_SILENTMODE == 0
        print("%s", CFG_PRINTF_NEWLINE);
#endif
        cliParse((char *)cli_buffer);
        cli_buffer_ptr = cli_buffer;
        break;

    case '\b':
#if CFG_INTERFACE_SILENTMODE == 0
        print("%c", c);
#endif
        if (cli_buffer_ptr == cli_buffer)
        {
            // Send bell alert and space (to maintain position)
            print("\a");
        }
        else if (cli_buffer_ptr > cli_buffer)
        {
            cli_buffer_ptr--;
#if CFG_INTERFACE_SILENTMODE == 0
            print("\b \b");
#endif
        }
        break;

    case 127:
#if CFG_INTERFACE_SILENTMODE == 0
        print("%c", c);
#endif
        if (cli_buffer_ptr == cli_buffer)
        {
            // Send bell alert and space (to maintain position)
            print("\a");
        }
        else if (cli_buffer_ptr > cli_buffer)
        {
            cli_buffer_ptr--;
#if CFG_INTERFACE_SILENTMODE == 0
            print("\b \b");
#endif
        }
        break;

    default:
        if (cli_buffer_ptr < cli_buffer + CFG_INTERFACE_MAXMSGSIZE - 2)
        {
#if CFG_INTERFACE_SILENTMODE == 0
            print("%c", c);
#endif
            *cli_buffer_ptr++ = c;
        }
        break;
    }
}

/**************************************************************************/
/*!
    @brief  Displays the command prompt.  The text that appears is defined
            in projectconfig.h.
*/
/**************************************************************************/
static void cliMenu()
{
#if CFG_INTERFACE_SILENTMODE == 0
    print(CFG_PRINTF_NEWLINE);
    print(CFG_INTERFACE_PROMPT);
#endif
#if CFG_INTERFACE_CONFIRMREADY == 1
    print("%s%s", CFG_INTERFACE_CONFIRMREADY_TEXT, CFG_PRINTF_NEWLINE);
#endif
}

/**************************************************************************/
/*!
    @brief  Parse the command line. This function tokenizes the command
            input, then searches for the command table entry associated
            with the commmand. Once found, it will jump to the
            corresponding function.

    @param[in]  cmd
                The entire command string to be parsed
*/
/**************************************************************************/
void cliParse(char *cmd)
{
    size_t argc, i = 0;
    char *argv[30];

    argv[i] = strtok(cmd, " ");
    do
    {
        argv[++i] = strtok(NULL, " ");
    }
    while ((i < 30) && (argv[i] != NULL));

    argc = i;
    for (i = 0; i < CMD_COUNT; i++)
    {
        if (!strcmp(argv[0], cli_tbl[i].command))
        {
            if ((argc == 2) && !strcmp (argv [1], "?"))
            {
                // Display parameter help menu on 'command ?'
                print ("%s%s%s", cli_tbl[i].description, CFG_PRINTF_NEWLINE, CFG_PRINTF_NEWLINE);
                print ("%s%s", cli_tbl[i].parameters, CFG_PRINTF_NEWLINE);
            }
            else if ((argc - 1) < cli_tbl[i].minArgs)
            {
                // Too few arguments supplied
                print ("%s (%s %d)%s", "Too few arguments", "Expected", cli_tbl[i].minArgs, CFG_PRINTF_NEWLINE);
                print ("%s'%s ?' %s%s%s", CFG_PRINTF_NEWLINE, cli_tbl[i].command, "for more information", CFG_PRINTF_NEWLINE, CFG_PRINTF_NEWLINE);
            }
            else if ((argc - 1) > cli_tbl[i].maxArgs)
            {
                // Too many arguments supplied
                print ("%s (%s %d)%s", "Too many arguments", "Maximum", cli_tbl[i].maxArgs, CFG_PRINTF_NEWLINE);
                print ("%s'%s ?' %s%s%s", CFG_PRINTF_NEWLINE, cli_tbl[i].command, "for more information", CFG_PRINTF_NEWLINE, CFG_PRINTF_NEWLINE);
            }
            else
            {
                // Dispatch command to the appropriate function
                cli_tbl[i].func(argc - 1, &argv [1]);
            }

            // Refresh the command prompt
            cliMenu();
            return;
        }
    }
    // Command not recognized
    print("%s: '%s'%s%s", "Command Not Recognised", cmd, CFG_PRINTF_NEWLINE, CFG_PRINTF_NEWLINE);
#if CFG_INTERFACE_SILENTMODE == 0
    print("%s%s", "Type '?' for a list of all available commands", CFG_PRINTF_NEWLINE);
#endif

    cliMenu();
}



/**************************************************************************/
/*!
    'help' command handler
*/
/**************************************************************************/
void cmd_help(uint8_t argc, char **argv)
{
    size_t i;

    print("%s      %s%s", "Command", "Description", CFG_PRINTF_NEWLINE);
    print("-------      -----------%s", CFG_PRINTF_NEWLINE);

    // Display full command list
    for (i = 0; i < CMD_COUNT; i++)
    {
        if (!cli_tbl[i].hidden)
        {
            print("%-10s   %s%s", cli_tbl[i].command, cli_tbl[i].description, CFG_PRINTF_NEWLINE);
        }
    }

    print("%s%s", "Command parameters can be seen by entering: <command-name> ?", CFG_PRINTF_NEWLINE);
}
