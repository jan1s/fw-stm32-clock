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
#include "uart.h"
#include "usb_cdc.h"


#define KEY_CODE_BS         (8)     /* Backspace key code */
#define KEY_CODE_DEL        (127)   /* Delete key code */
#define KEY_CODE_ESC        (27)    /* Escape key code */
#define KEY_CODE_ENTER      (13)    /* Enter key code  */


static uint8_t cli_buffer[CLI_END][CFG_INTERFACE_MAXMSGSIZE];
static uint8_t *cli_buffer_ptr[CLI_END];
void (*cli_send[CLI_END]) (uint8_t *,uint32_t);

static void cliMenu(cli_select_t t);

/**************************************************************************/
/*!
    @brief Initialises the command line using the appropriate interface
*/
/**************************************************************************/
void cliInit(cli_select_t t)
{
	switch(t)
	{
	case CLI_USBCDC:
		{
			USB_CDC_Init();
			cli_send[t] = USB_CDC_SendBuffer;
		}
		break;

	case CLI_USART1:
		{
			uart1Init();
			cli_send[t] = uart1Send;
		}
		break;

	case CLI_USART2:
		{
			//uart2Init();
			//cli_send[t] = uart2Send;
		}
		break;
	}

	cli_buffer_ptr[t] = cli_buffer[t];

    // Show the menu
    cliMenu(t);
}

/**************************************************************************/
/*!
    @brief  Polls the relevant incoming message queue to see if anything
            is waiting to be processed.
*/
/**************************************************************************/
void cliPoll(cli_select_t t)
{
	uint8_t c;

	switch(t)
	{
	case CLI_USBCDC:
		{
			while (USB_CDC_Read(&c))
			{
				cliRx(t, c);
			}
		}
		break;

	case CLI_USART1:
		{
			while (uart1ReadChar(&c))
			{
				cliRx(t, c);
			}
		}
		break;

	case CLI_USART2:
		{
			//while (uart2ReadChar(&c))
			//{
			//	cliRx(t, c);
			//}
		}
		break;
	}


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
void cliRx(cli_select_t t, uint8_t c)
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
        *cli_buffer_ptr[t] = '\0';
#if CFG_INTERFACE_SILENTMODE == 0
        print(cli_send[t], "%s", CFG_PRINTF_NEWLINE);
#endif
        cliParse(t, (char *)cli_buffer);
        cli_buffer_ptr[t] = cli_buffer[t];
        break;

    case '\b':
#if CFG_INTERFACE_SILENTMODE == 0
        print(cli_send[t], "%c", c);
#endif
        if (cli_buffer_ptr[t] == cli_buffer[t])
        {
            // Send bell alert and space (to maintain position)
            print(cli_send[t], "\a");
        }
        else if (cli_buffer_ptr[t] > cli_buffer[t])
        {
            cli_buffer_ptr[t]--;
#if CFG_INTERFACE_SILENTMODE == 0
            print(cli_send[t], "\b \b");
#endif
        }
        break;

    case 127:
#if CFG_INTERFACE_SILENTMODE == 0
        print(cli_send[t], "%c", c);
#endif
        if (cli_buffer_ptr[t] == cli_buffer[t])
        {
            // Send bell alert and space (to maintain position)
            print(cli_send[t], "\a");
        }
        else if (cli_buffer_ptr[t] > cli_buffer[t])
        {
            cli_buffer_ptr[t]--;
#if CFG_INTERFACE_SILENTMODE == 0
            print(cli_send[t], "\b \b");
#endif
        }
        break;

    default:
        if (cli_buffer_ptr[t] < cli_buffer[t] + CFG_INTERFACE_MAXMSGSIZE - 2)
        {
#if CFG_INTERFACE_SILENTMODE == 0
            print(cli_send[t], "%c", c);
#endif
            *cli_buffer_ptr[t]++ = c;
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
static void cliMenu(cli_select_t t)
{
#if CFG_INTERFACE_SILENTMODE == 0
    print(cli_send[t], CFG_PRINTF_NEWLINE);
    print(cli_send[t], CFG_INTERFACE_PROMPT);
#endif
#if CFG_INTERFACE_CONFIRMREADY == 1
    print(cli_send[t], "%s%s", CFG_INTERFACE_CONFIRMREADY_TEXT, CFG_PRINTF_NEWLINE);
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
void cliParse(cli_select_t t, char *cmd)
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
                print (cli_send[t], "%s%s%s", cli_tbl[i].description, CFG_PRINTF_NEWLINE, CFG_PRINTF_NEWLINE);
                print (cli_send[t], "%s%s", cli_tbl[i].parameters, CFG_PRINTF_NEWLINE);
            }
            else if ((argc - 1) < cli_tbl[i].minArgs)
            {
                // Too few arguments supplied
                print (cli_send[t], "%s (%s %d)%s", "Too few arguments", "Expected", cli_tbl[i].minArgs, CFG_PRINTF_NEWLINE);
                print (cli_send[t], "%s'%s ?' %s%s%s", CFG_PRINTF_NEWLINE, cli_tbl[i].command, "for more information", CFG_PRINTF_NEWLINE, CFG_PRINTF_NEWLINE);
            }
            else if ((argc - 1) > cli_tbl[i].maxArgs)
            {
                // Too many arguments supplied
                print (cli_send[t], "%s (%s %d)%s", "Too many arguments", "Maximum", cli_tbl[i].maxArgs, CFG_PRINTF_NEWLINE);
                print (cli_send[t], "%s'%s ?' %s%s%s", CFG_PRINTF_NEWLINE, cli_tbl[i].command, "for more information", CFG_PRINTF_NEWLINE, CFG_PRINTF_NEWLINE);
            }
            else
            {
                // Dispatch command to the appropriate function
                cli_tbl[i].func(t, argc - 1, &argv [1]);
            }

            // Refresh the command prompt
            cliMenu(t);
            return;
        }
    }
    // Command not recognized
    print(cli_send[t], "%s: '%s'%s%s", "Command Not Recognised", cmd, CFG_PRINTF_NEWLINE, CFG_PRINTF_NEWLINE);
#if CFG_INTERFACE_SILENTMODE == 0
    print(cli_send[t], "%s%s", "Type '?' for a list of all available commands", CFG_PRINTF_NEWLINE);
#endif

    cliMenu(t);
}



/**************************************************************************/
/*!
    'help' command handler
*/
/**************************************************************************/
void cmd_help(cli_select_t t, uint8_t argc, char **argv)
{
    size_t i;

    print(cli_send[t], "%s      %s%s", "Command", "Description", CFG_PRINTF_NEWLINE);
    print(cli_send[t], "-------      -----------%s", CFG_PRINTF_NEWLINE);

    // Display full command list
    for (i = 0; i < CMD_COUNT; i++)
    {
        if (!cli_tbl[i].hidden)
        {
            print(cli_send[t], "%-10s   %s%s", cli_tbl[i].command, cli_tbl[i].description, CFG_PRINTF_NEWLINE);
        }
    }

    print(cli_send[t], "%s%s", "Command parameters can be seen by entering: <command-name> ?", CFG_PRINTF_NEWLINE);
}
