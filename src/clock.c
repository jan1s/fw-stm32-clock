#include "platform_config.h"

#ifdef CFG_FLIPDOT
#include "flipdot.h"
#include "flipdot_clock.h"
#endif

#ifdef CFG_NIXIE
#include "nixie/nixie.h"
#include "nixie/nixie_clock.h"
#endif

#include "rtc/rtc.h"
#include "rtc/tz.h"

uint32_t lastEpoch = 0;


void clockInit()
{
    rtcInit();
    tzInit();
    lastEpoch = rtcGet();

#ifdef CFG_FLIPDOT
    flipdot_init();
    flipdotClockInit();
#endif

#ifdef CFG_NIXIE
    nixieInit();
    nixieClockInit();
#endif
}

void clockPoll()
{
    uint32_t epoch = rtcGet();

    if(epoch != lastEpoch)
    {
        rtcTime_t utc;
        rtcTime_t local;

        rtcCreateTimeFromEpoch( epoch, &utc );
        tzUTCToLocal( &utc, &local );

#ifdef CFG_FLIPDOT
        flipdotClockShowTime(local);
#endif

#ifdef CFG_NIXIE
        nixieClockShowTime(local);
#endif
    }
}
