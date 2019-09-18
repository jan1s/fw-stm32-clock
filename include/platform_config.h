#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/*=========================================================================
    EEPROM Emulation
    -----------------------------------------------------------------------

    EEPROM is used to persist certain user modifiable values to make
    sure that these changes remain in effect after a reset or hard
    power-down.  The addresses in EEPROM for these various system
    settings/values are defined below.  The first 256 bytes of EEPROM
    are reserved for this (0x0000..0x00FF).

    CFG_EEPROM_RESERVED       The last byte of reserved EEPROM memory

          EEPROM Address (0x0000..0x00FF)
          ===============================
          0 1 2 3 4 5 6 7 8 9 A B C D E F
    000x  . . . . . . . . . . . . . . . .
    001x  x x x x x x . . . . . . . . . .   Timezone STD
    002x  x x x x x x . . . . . . . . . .   Timezone DST
    003x  x . x . . . . . . . . . . . . .   Clock Source / Nightmode
    004x  x x x x . . . . . . . . . . . .   Nixie Type/Mode
    005x  . . . . . . . . . . . . . . . .
    006x  . . . . . . . . . . . . . . . .
    007x  . . . . . . . . . . . . . . . .
    008x  . . . . . . . . . . . . . . . .
    009x  . . . . . . . . . . . . . . . .
    00Ax  . . . . . . . . . . . . . . . .
    00Bx  . . . . . . . . . . . . . . . .
    00Cx  . . . . . . . . . . . . . . . .
    00Dx  . . . . . . . . . . . . . . . .
    00Ex  . . . . . . . . . . . . . . . .
    00Fx  . . . . . . . . . . . . . . . .

    -----------------------------------------------------------------------*/
#define CFG_EEPROM_RESERVED     (0xFFFF)
#define CFG_EEPROM_TZ_STD       (uint16_t)0x0010
#define CFG_EEPROM_TZ_DST       (uint16_t)0x0020
#define CFG_EEPROM_CLOCK_SRC    (uint16_t)0x0030
#define CFG_EEPROM_CLOCK_NM     (uint16_t)0x0032
#define CFG_EEPROM_NIXIE_TYPE   (uint16_t)0x0040
#define CFG_EEPROM_NIXIE_MODE   (uint16_t)0x0042
/*=========================================================================*/


/*=========================================================================
    NIXIE
    -----------------------------------------------------------------------

    CFG_NIXIE                 If this field is defined the clock type is
                              set to Nixieclock
    -----------------------------------------------------------------------*/
#define CFG_NIXIE
/*=========================================================================*/

/*=========================================================================
    FLIP_BUS
    -----------------------------------------------------------------------

    CFG_FLIP_BUS              If this field is defined the clock type is
                              set to Flipdot clock
    -----------------------------------------------------------------------*/
//#define CFG_FLIP_BUS
/*=========================================================================*/

/*=========================================================================
    FLIP_BROSE
    -----------------------------------------------------------------------

    CFG_FLIP_BROSE            If this field is defined the clock type is
                              set to Flipdot clock
    -----------------------------------------------------------------------*/
//#define CFG_FLIP_BROSE
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
//#define CFG_PROTOCOL_USART1
//#define CFG_PROTOCOL_UART2
/*=========================================================================*/

#define CFG_PRINTF_NEWLINE          "\r\n"

#endif // __PLATFORM_CONFIG_H
