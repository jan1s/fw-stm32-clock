#ifndef FLIP_BUS_CLOCK_H_
#define FLIP_BUS_CLOCK_H_

#include "platform_config.h"
#include "rtc/rtc_functions.h"

#ifdef CFG_FLIP_BUS

typedef enum
{
	FLIPDOTCLOCK_MODE_NONE = 0,
	FLIPDOTCLOCK_MODE_ddmmHHMM,
	FLIPDOTCLOCK_MODE_ddHHMMSS,
	FLIPDOTCLOCK_MODE_ddmmyyyy,
	FLIPDOTCLOCK_MODE_END
} flipdotclockMode_t;

extern flipdotclockMode_t flipdotclockMode;

void flipdotClockInit(void);
void flipdotClockShowTime(rtcTime_t t);

void flipdotclockStoreMode( const flipdotclockMode_t m );
flipdotclockMode_t flipdotclockLoadMode();
void flipdotclockSetMode( const flipdotclockMode_t m );
flipdotclockMode_t fliptdotclockGetMode( void );

#endif // CFG_FLIP_BUS

#endif // FLIPDOT_CLOCK_H_
