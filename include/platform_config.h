#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/*=========================================================================
    NIXIE
    -----------------------------------------------------------------------

    CFG_NIXIE                 If this field is defined the clock type is
                              set to Nixieclock
    -----------------------------------------------------------------------*/
#define CFG_NIXIE
/*=========================================================================*/

/*=========================================================================
    FLIPDOT
    -----------------------------------------------------------------------

    CFG_FLIPDOT               If this field is defined the clock type is
                              set to Flipdot clock
    -----------------------------------------------------------------------*/
//#define CFG_FLIPDOT
/*=========================================================================*/


/*=========================================================================
    USART1
    -----------------------------------------------------------------------

    CFG_USART1_BAUDRATE       The default USART1 speed. This value is used
                              when initialising USART1, and should be a
                              standard value like 57600, 9600, etc.
    CFG_USART1_BUFSIZE        The length in bytes of the USART1 RX FIFO.
                              This will determine the maximum number of
                              received characters to store in memory.

    -----------------------------------------------------------------------*/
#define CFG_USART1_BAUDRATE           (115200)
#define CFG_USART1_BUFSIZE            (256)
/*=========================================================================*/

/*=========================================================================
    USART2
    -----------------------------------------------------------------------

    CFG_USART2_BAUDRATE       The default USART2 speed. This value is used
                              when initialising USART2, and should be a
                              standard value like 57600, 9600, etc.
    CFG_USART2_BUFSIZE        The length in bytes of the USART2 RX FIFO.
                              This will determine the maximum number of
                              received characters to store in memory.

    -----------------------------------------------------------------------*/
#define CFG_USART2_BAUDRATE           (115200)
#define CFG_USART2_BUFSIZE            (256)
/*=========================================================================*/

/*=========================================================================
    COMMAND LINE INTERFACE
    -----------------------------------------------------------------------

    CFG_INTERFACE             If this field is defined the UART or USBCDC
                              based command-line interface will be included
    CFG_INTERFACE_MAXMSGSIZE  The maximum number of bytes to accept for an
                              incoming command
    CFG_INTERFACE_PROMPT      The command prompt to display at the start
                              of every new data entry line
    CFG_INTERFACE_SILENTMODE  If this is set to 1 only text generated in
                              response to commands will be send to the
                              output buffer.  The command prompt will not
                              be displayed and incoming text will not be
                              echoed back to the output buffer (allowing
                              you to see the text you have input).  This
                              is normally only desirable in a situation
                              where another MCU is communicating with
                              the LPC1343.
    CFG_INTERFACE_DROPCR      If this is set to 1 all incoming \r
                              characters will be dropped
    CFG_INTERFACE_CONFIRMREADY  If this is set to 1 a text confirmation
                              will be sent when the command prompt is
                              ready for a new command.  This is in
                              addition to CFG_INTERFACE_ENABLEIRQ if
                              this is also enabled.  The character used
                              is defined below.
    CFG_INTERFACE_LONGSYSINFO If this is set to 1 extra information will
                              be included in the Sys Info ('V') command
                              on the CLI. This can be useful when trying
                              to debug problems on remote HW, or with
                              unknown firmware.  It will also use about
                              0.5KB flash, though, so only enable it is
                              necessary.

    NOTE:                     The command-line interface will use either
                              USB-CDC or UART depending on whether
                              CFG_PRINTF_UART or CFG_PRINTF_USBCDC are
                              selected.
    -----------------------------------------------------------------------*/
#define CFG_INTERFACE
#define CFG_INTERFACE_MAXMSGSIZE    (256)
#define CFG_INTERFACE_PROMPT        "$"
#define CFG_INTERFACE_SILENTMODE    (0)
#define CFG_INTERFACE_DROPCR        (0)
#define CFG_INTERFACE_CONFIRMREADY  (0)
#define CFG_INTERFACE_LONGSYSINFO   (1)
#define CFG_INTERFACE_USART1
#define CFG_INTERFACE_USART2
/*=========================================================================*/

/*=========================================================================
    PROTOCOL
    -----------------------------------------------------------------------

    CFG_PROTOCOL              If this field is defined a binary protocol
                              will be used
    -----------------------------------------------------------------------*/
#define CFG_PROTOCOL
//#define CFG_PROTOCOL_USBCDC
#define CFG_PROTOCOL_USART1
//#define CFG_PROTOCOL_UART2
/*=========================================================================*/

#define CFG_PRINTF_NEWLINE          "\r\n"

#endif // __PLATFORM_CONFIG_H
