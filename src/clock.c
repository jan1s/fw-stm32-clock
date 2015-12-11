#include "platform_config.h"

#ifdef CFG_TYPE_FLIPDOT
#include "flipdot.h"
#include "flipdot_clock.h"
#endif

#ifdef CFG_TYPE_NIXIE
#include "nixie.h"
#include "nixie_clock.h"
#endif

#include "rtc.h"
#include <time.h>

time_t lastUtc = 0;


void clockInit()
{
    rtcInit();
    lastUtc = rtcGet();

#ifdef CFG_TYPE_FLIPDOT
    flipdot_init();
    flipdotClockInit();
#endif

#ifdef CFG_TYPE_NIXIE
    nixieInit();
    nixieClockInit();
#endif
}

void clockPoll()
{
    time_t utc = rtcGet();
    if(utc != lastUtc)
    {
#ifdef CFG_TYPE_FLIPDOT
        flipdotClockShowTime(utc);
#endif

#ifdef CFG_TYPE_NIXIE
        nixieClockShowTime(utc);
#endif
    }
}