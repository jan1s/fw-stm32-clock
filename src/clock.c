#include "platform_config.h"

#include "clock.h"

#ifdef CFG_FLIPDOT
#include "flipdot.h"
#include "flipdot_clock.h"
#endif

#ifdef CFG_NIXIE
#include "nixie/nixie.h"
#include "nixie/nixieclock.h"
#endif

#include "rtc/rtc.h"
#include "rtc/tz.h"
#include "rtc/dcf.h"
#include "rtc/gps.h"

uint32_t lastEpoch = 0;

clockSource_t clockSource;

void clockStoreSource( const clockSource_t s )
{
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Read previous state */
    //uint16_t bkp = BKP_ReadBackupRegister(BKP_DR4);
    //bkp &= ~(0xFF00);
    //bkp |= (m << 8);

    /* Write to the BKP Domain */
    //BKP_WriteBackupRegister(BKP_DR4, bkp);

    /* Deny access to BKP Domain */
    PWR_BackupAccessCmd(DISABLE);
}

clockSource_t clockLoadSource()
{
    return (BKP_ReadBackupRegister(BKP_DR4) & 0xFF00) >> 8;
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

#ifdef CFG_FLIPDOT
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
        rtcTime_t utc;
        rtcTime_t local;

        rtcCreateTimeFromEpoch( epoch, &utc );
        tzUTCToLocal( &utc, &local );

#ifdef CFG_FLIPDOT
        flipdotClockShowTime(local);
#endif

#ifdef CFG_NIXIE
        nixieclockShowTime(local);
#endif
    }
}
