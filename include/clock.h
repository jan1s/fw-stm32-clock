#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "platform_config.h"
#include "rtc/rtc_functions.h"

typedef enum
{
    CLOCK_SOURCE_NONE = 0,
    CLOCK_SOURCE_DCF77,
    CLOCK_SOURCE_GPS,
    CLOCK_SOURCE_CLI,
    CLOCK_SOURCE_END
} clockSource_t;

typedef struct nightModeRule_st
{
    uint8_t startHour;
    uint8_t startMinute;
    uint8_t endHour;
    uint8_t endMinute;
    uint8_t dayMask; /* Bitmask for days active, 0 = None, 1 = Monday, 3 = Monday+Tuesday */
} nightModeRule_t;

void clockInit(void);
void clockPoll(void);

void clockSetSource( const clockSource_t s );
void clockStoreSource( const clockSource_t s );
clockSource_t clockLoadSource( void );
clockSource_t clockGetSource( void );

void clockSetNightmode( const nightModeRule_t s );
void clockStoreNightmode( const nightModeRule_t s );
nightModeRule_t clockLoadNightmode( void );
nightModeRule_t clockGetNightmode( void );
uint8_t clockIsNightmode ( rtcTime_t t );

#endif
