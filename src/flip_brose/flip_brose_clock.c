#include "platform_config.h"

#ifdef CFG_FLIP_BROSE

#include "flip_brose/flip_brose.h"
#include "flip_brose/flip_brose_clock.h"

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

    for(int i = 0; i < 3; ++i)
    {
    	flipdot_wipe_21x13(0);
		timer_sleep(80000);
		flipdot_wipe_21x13(1);
		timer_sleep(80000);
    }

    //fdisp_21x13_t d;
	//const uint8_t shallo[] = "Flip!";
	//flipdot_setstring_4x6_21x13(&d, shallo, sizeof(shallo));
	//flipdot_set_21x13(&d);
	//timer_sleep(200000);
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
	return FLIPDOTCLOCK_MODE_HHMM; //Quickfix
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
    switch(flipdotclockMode)
    {
    case FLIPDOTCLOCK_MODE_HHMM:
		{
			char buffer[4] = {0};
			sprintf(buffer, "%02d%02d", t.hours, t.minutes);
			fdisp_21x13_t d = {0};
			flipdot_setstring_HHMM_21x13(&d, (uint8_t*)buffer, sizeof(buffer));
			flipdot_set_21x13(&d);
		}
        break;
    case FLIPDOTCLOCK_MODE_HHMMddmm:
    	{
			char buffer[8] = {0};
			sprintf(buffer, "%02d%02d%02d%02d", t.hours, t.minutes, t.days, t.months);
			fdisp_21x13_t d = {0};
			flipdot_setstring_HHMMddmm_21x13(&d, (uint8_t*)buffer, sizeof(buffer));
			flipdot_set_21x13(&d);
    	}
    	break;

    default:
        break;
    }
}

#endif // CFG_FLIP_BROSE

// ----------------------------------------------------------------------------
