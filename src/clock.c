#include "platform_config.h"

#include "clock.h"

#ifdef CFG_FLIP_BUS
#include "flip_bus/flip_bus.h"
#include "flip_bus/flip_bus_clock.h"
#endif

#ifdef CFG_FLIP_BROSE
#include "flip_brose/flip_brose.h"
#include "flip_brose/flip_brose_clock.h"
#endif

#ifdef CFG_NIXIE
#include "nixie/nixie.h"
#include "nixie/nixieclock.h"
#endif

#include "rtc/rtc.h"
#include "rtc/tz.h"
#include "rtc/dcf.h"
#include "rtc/gps.h"

static uint32_t lastEpoch = 0;

clockSource_t clockSource;

void clockStoreSource( const clockSource_t s )
{
	/* Allow access to FLASH Domain */
	FLASH_Unlock();

	/* Write to the FLASH Domain */
	EE_WriteVariable(CFG_EEPROM_CLOCK_SRC, (uint16_t)s);

	/* Deny access to FLASH Domain */
	FLASH_Lock();
}

clockSource_t clockLoadSource()
{
	uint16_t s;
	EE_ReadVariable(CFG_EEPROM_CLOCK_SRC, (uint16_t*)&s);
	return s;
}

void clockSetSource( const clockSource_t s )
{
    clockSource = s;

    switch(clockSource)
    {
        case CLOCK_SOURCE_NONE:
        {

        }
        break;

        case CLOCK_SOURCE_DCF77:
        {
            dcfInit();
        }
        break;

        case CLOCK_SOURCE_GPS:
        {
            //gpsInit();
        }
        break;

        case CLOCK_SOURCE_CLI:
        {

        }
        break;

        default:
        {

        }
        break;
    }
}

clockSource_t clockGetSource( void )
{
    return clockSource;
}


void clockInit()
{
    rtcInit();
    tzInit();
    lastEpoch = rtcGet();
    clockSource = clockLoadSource();

#ifdef CFG_FLIP_BUS
    flipdot_init();
    flipdotClockInit();
#endif

#ifdef CFG_FLIP_BROSE
    flipdot_init();
    flipdotClockInit();
#endif

#ifdef CFG_NIXIE
    nixieInit();
    nixieclockInit();
#endif
}

void clockPoll()
{
    uint32_t epoch = rtcGet();

    switch(clockSource)
    {
        case CLOCK_SOURCE_NONE:
        {

        }
        break;

        case CLOCK_SOURCE_DCF77:
        {
            dcfPoll();
        }
        break;

        case CLOCK_SOURCE_GPS:
        {
            //gpsPoll();
        }
        break;

        case CLOCK_SOURCE_CLI:
        {

        }
        break;

        default:
        {

        }
        break;
    }

    if(epoch != lastEpoch)
    {
        lastEpoch = epoch;

        rtcTime_t utc;
        rtcTime_t local;

        rtcCreateTimeFromEpoch( epoch, &utc );
        tzUTCToLocal( &utc, &local );

#ifdef CFG_FLIP_BUS
        flipdotClockShowTime(local);
#endif

#ifdef CFG_FLIP_BROSE
        flipdotClockShowTime(local);
#endif

#ifdef CFG_NIXIE
        nixieclockShowTime(local);
#endif
    }
}
