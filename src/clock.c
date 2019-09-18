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
nightModeRule_t nightMode;

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

void clockStoreNightmode( const nightModeRule_t m )
{
    /* Allow access to FLASH Domain */
    FLASH_Unlock();

    /* Write to the FLASH Domain */
    EE_WriteVariable(CFG_EEPROM_CLOCK_NM + 0, (uint16_t)m.dayMask);
    EE_WriteVariable(CFG_EEPROM_CLOCK_NM + 1, (uint16_t)((m.startHour << 8) + m.startMinute));
    EE_WriteVariable(CFG_EEPROM_CLOCK_NM + 2, (uint16_t)((m.endHour << 8) + m.endMinute));

    /* Deny access to FLASH Domain */
    FLASH_Lock();
}

nightModeRule_t clockLoadNightmode()
{
    nightModeRule_t m;
    uint8_t mask;
    uint16_t start;
    uint16_t end;

    EE_ReadVariable(CFG_EEPROM_CLOCK_NM + 0, &mask);
    EE_ReadVariable(CFG_EEPROM_CLOCK_NM + 1, &start);
    EE_ReadVariable(CFG_EEPROM_CLOCK_NM + 2, &end);

    m.dayMask = mask;
    m.startHour = start >> 8;
    m.startMinute = start & 0xFF;
    m.endHour = end >> 8;
    m.endMinute = end & 0xFF;
    return m;
}

void clockSetNightmode( const nightModeRule_t m )
{
    nightMode = m;
}

nightModeRule_t clockGetNightmode( void )
{
    return nightMode;
}


void clockInit()
{
    rtcInit();
    tzInit();
    lastEpoch = rtcGet();
    clockSource = clockLoadSource();
    nightMode = clockLoadNightmode();

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

        uint32_t lepoch = rtcToEpochTime( &local );
        if(lepoch % 86400 == 0)
        {
            nixieclockSaveTubes();
        }
#endif
    }
}
