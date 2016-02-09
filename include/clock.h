#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "platform_config.h"

typedef enum
{
    CLOCK_SOURCE_NONE = 0,
    CLOCK_SOURCE_DCF77,
    CLOCK_SOURCE_GPS,
    CLOCK_SOURCE_CLI,
    CLOCK_SOURCE_END
} clockSource_t;

void clockInit(void);
void clockPoll(void);

void clockSetSource( const clockSource_t s );
void clockStoreSource( const clockSource_t s );
clockSource_t clockLoadSource();
clockSource_t clockGetSource( void );

#endif
