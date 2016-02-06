#include "platform_config.h"

#include "rtc/rtc_functions.h"

#include <stdio.h>
/**************************************************************************/
/*!
    'cmd_rtc_write' command handler
*/
/**************************************************************************/
void cmd_rtc_write(uint8_t argc, char **argv)
{
    //err_t error;
    rtcTime_t time;
    int32_t year, month, day, hour, minute, second;

    //getNumber(argv[0], &year);
    //getNumber(argv[1], &month);
    //getNumber(argv[2], &day);
    //getNumber(argv[3], &hour);
    //getNumber(argv[4], &minute);
    //getNumber(argv[5], &second);

    /* Make sure values are valid */
    if ((year < 2000) || (year > 2038))
    {
        //printf("%s%s", "Year must be between 2000 and 2023", CFG_PRINTF_NEWLINE);
        return;
    }
    if ((month < RTC_MONTHS_JANUARY) || (month > RTC_MONTHS_DECEMBER))
    {
        //printf("%s%s", "Month must be between 1 and 12", CFG_PRINTF_NEWLINE);
        return;
    }
    if ((day < 1) || (day > 31))
    {
        //printf("%s%s", "Day must be between 1 and 31", CFG_PRINTF_NEWLINE);
        return;
    }
    if ((hour < 0) || (hour > 23))
    {
        //printf("%s%s", "Hour must be between 0 and 23", CFG_PRINTF_NEWLINE);
        return;
    }
    if ((minute < 0) || (minute > 59))
    {
        //printf("%s%s", "Minute must be between 0 and 59", CFG_PRINTF_NEWLINE);
        return;
    }
    if ((second < 0) || (second > 59))
    {
        //printf("%s%s", "Second must be between 0 and 59", CFG_PRINTF_NEWLINE);
        return;
    }

    /* Try to create a date */
    /*
    error = rtcCreateTime(year, month, day, hour, minute, second, 0, &time);
    if (error)
    {
      printf("%s%s", "Invalid timestamp", CFG_PRINTF_NEWLINE);
      return;
    }
    */

    /* Write the time to the RTC */
    /*
    error = pcf2129SetTime(time);
    if (error)
    {
      printf("%s%s", STRING(LOCALISATION_TEXT_No_response_on_the_I2C_bus), CFG_PRINTF_NEWLINE);
      return;
    }
    */
}
