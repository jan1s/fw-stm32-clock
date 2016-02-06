#include "platform_config.h"

#include <stdio.h>

/**************************************************************************/
/*!
    'cmd_rtc_read' command handler
*/
/**************************************************************************/
void cmd_rtc_read(uint8_t argc, char **argv)
{
    /*
    err_t error;
    rtcTime_t time;

    error = pcf2129ReadTime(&time);
    if (error)
    {
      printf("%s%s", STRING(LOCALISATION_TEXT_No_response_on_the_I2C_bus), CFG_PRINTF_NEWLINE);
      return;
    }

    printf("%04d/%02d/%02d%s", time.years+1900, time.months+1, time.days, CFG_PRINTF_NEWLINE);
    printf("%02d:%02d:%02d%s", time.hours, time.minutes, time.seconds, CFG_PRINTF_NEWLINE);
    printf("%u%s", rtcToEpochTime(&time), CFG_PRINTF_NEWLINE);
    */
    //printf("%s\n", "stupid");
}
