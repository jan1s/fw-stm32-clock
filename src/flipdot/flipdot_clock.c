#include "platform_config.h"

#ifdef CFG_FLIPDOT

#include "flipdot/flipdot.h"
#include "flipdot/flipdot_clock.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "timer.h"

flipdotclockMode_t flipdotclockMode;

const char *wdays[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
const char *month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// ----------------------------------------------------------------------------

void flipdotClockInit()
{
    flipdotclockMode = flipdotclockLoadMode();

    fdisp_84x7_t d;
    const uint8_t shallo[] = " Flipdot 84x7";
    flipdot_setstring_84x7(&d, shallo, sizeof(shallo));
    flipdot_set_84x7(&d);
    timer_sleep(200000);

    for(int i = 0; i < 4; ++i)
    {
    	flipdot_wipe_84x7(0);
		timer_sleep(100000);
		flipdot_wipe_84x7(1);
		timer_sleep(100000);
    }

#ifdef CFG_TYPE_FLIPDOT_112X16
    flipdot_wipe_112x16(0);
    fdisp_112x16_t d = {0};
    const uint8_t shallo[] = "Flipdot 112x16";
    flipdot_setstring_112x16(&d, shallo, sizeof(shallo));
    flipdot_set_112x16(&d);
#endif

    timer_sleep(400000);
}

void flipdotclockStoreMode( const flipdotclockMode_t m )
{
	/* Allow access to FLASH Domain */
    FLASH_Unlock();

    /* Write to the FLASH Domain */
	EE_WriteVariable(CFG_EEPROM_NIXIE_MODE, (uint16_t)m);

	/* Deny access to FLASH Domain */
	FLASH_Lock();
}

flipdotclockMode_t flipdotclockLoadMode()
{
	uint16_t m;
	EE_ReadVariable(CFG_EEPROM_NIXIE_MODE, (uint16_t*)&m);
	return FLIPDOTCLOCK_MODE_ddmmHHMM; //Quickfix
}

void flipdotclockSetMode( const flipdotclockMode_t m )
{
	flipdotclockMode = m;
}

flipdotclockMode_t flipdotclockGetMode( void )
{
    return flipdotclockMode;
}

void flipdotClockShowTime(rtcTime_t t)
{

    char buffer[14] = {0};

    switch(flipdotclockMode)
    {
    case FLIPDOTCLOCK_MODE_ddmmHHMM:
        sprintf(buffer, "%2d.%s. %02d:%02d", t.days, (uint8_t*)month[t.months-1], t.hours, t.minutes);
        break;
    case FLIPDOTCLOCK_MODE_ddHHMMSS:
        sprintf(buffer, "%s. %02d:%02d:%02d", (uint8_t*)wdays[t.weekdays], t.hours, t.minutes, t.seconds);
        break;
    case FLIPDOTCLOCK_MODE_ddmmyyyy:
        if(t.seconds % 20 > 10)
        {
            sprintf(buffer, "%2d.%s.  %04d", t.days, (uint8_t*)month[t.months-1], t.years + 1900);
        }
        else
        {
            sprintf(buffer, "%s.  %02d:%02d:%02d", (uint8_t*)wdays[t.weekdays], t.hours, t.minutes, t.seconds);
        }
        break;
    default:
        break;
    }


    if(flipdotclockMode != FLIPDOTCLOCK_MODE_NONE)
    {
        fdisp_84x7_t d;
        flipdot_setstring_84x7(&d, (uint8_t*)buffer, sizeof(buffer));
        flipdot_set_84x7(&d);
    }

}

#endif // CFG_TYPE_FLIPDOT

// ----------------------------------------------------------------------------
