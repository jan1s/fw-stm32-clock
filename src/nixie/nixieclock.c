#include "platform_config.h"

#ifdef CFG_NIXIE

#include "nixie/nixie.h"
#include "nixie/nixieclock.h"
#include "rtc/rtc.h"
#include "rtc/rtc_functions.h"
#include "timer.h"

#include <stdio.h>
#include <string.h>

nixieclockMode_t nixieclockMode;


void nixieclockInit()
{
    nixieclockMode = nixieclockLoadMode();

    nixieInit();
    nixieHighVoltageEnable();
}

void nixieclockStoreMode( const nixieclockMode_t m )
{
	/* Allow access to FLASH Domain */
    FLASH_Unlock();

    /* Write to the FLASH Domain */
	EE_WriteVariable(CFG_EEPROM_NIXIE_MODE, (uint16_t)m);

	/* Deny access to FLASH Domain */
	FLASH_Lock();
}

nixieclockMode_t nixieclockLoadMode()
{
	uint16_t m;
	EE_ReadVariable(CFG_EEPROM_NIXIE_MODE, (uint16_t*)&m);
	return m;
}

void nixieclockSetMode( const nixieclockMode_t m )
{
    nixieclockMode = m;
}

nixieclockMode_t nixieclockGetMode( void )
{
    return nixieclockMode;
}

void nixieclockShowTime(rtcTime_t t)
{
    switch(nixieclockMode)
    {
    case NIXIECLOCK_MODE_NONE:
    {
        nixieDisplay4t_t d;
        d.digits[3] = 1;
        d.digits[2] = 3;
        d.digits[1] = 3;
        d.digits[0] = 7;
        d.dots[1] = 1;
        d.dots[0] = 1;
        nixieDisplay4t(&d);
    }
    break;

    case NIXIECLOCK_MODE_HHMM:
    {
        nixieDisplay4t_t d;
        d.digits[3] = t.hours / 10;
        d.digits[2] = t.hours % 10;
        d.digits[1] = t.minutes / 10;
        d.digits[0] = t.minutes % 10;
        d.dots[1] = t.seconds % 2;
        d.dots[0] = t.seconds % 2;
        nixieDisplay4t(&d);
    }
    break;

    case NIXIECLOCK_MODE_MMSS:
    {
        nixieDisplay4t_t d;
        d.digits[3] = t.minutes / 10;
        d.digits[2] = t.minutes % 10;
        d.digits[1] = t.seconds / 10;
        d.digits[0] = t.seconds % 10;
        d.dots[1] = 1;
        d.dots[0] = 1;
        nixieDisplay4t(&d);
    }
    break;

    case NIXIECLOCK_MODE_YYYY:
    {
        nixieDisplay4t_t d;
        uint16_t year = t.years + 1900;
        d.digits[3] = (year / 1000) % 10;
        d.digits[2] = (year / 100) % 10;
        d.digits[1] = (year / 10) % 10;
        d.digits[0] = year % 10;
        d.dots[1] = 1;
        d.dots[0] = 1;
        nixieDisplay4t(&d);
    }
    break;

    case NIXIECLOCK_MODE_HHMMSS:
    {
        nixieDisplay6t_t d;
        d.digits[5] = t.hours / 10;
        d.digits[4] = t.hours % 10;
        d.digits[3] = t.minutes / 10;
        d.digits[2] = t.minutes % 10;
        d.digits[1] = t.seconds / 10;
        d.digits[0] = t.seconds % 10;
        d.dots[3] = t.seconds % 2;
        d.dots[2] = t.seconds % 2;
        d.dots[1] = t.seconds % 2;
        d.dots[0] = t.seconds % 2;
        nixieDisplay6t(&d);
    }
    break;

    case NIXIECLOCK_MODE_HHMMSS_R:
        {
            nixieDisplay6t_t d;
            d.digits[0] = t.hours / 10;
            d.digits[1] = t.hours % 10;
            d.digits[2] = t.minutes / 10;
            d.digits[3] = t.minutes % 10;
            d.digits[4] = t.seconds / 10;
            d.digits[5] = t.seconds % 10;
            d.dots[3] = t.seconds % 2;
            d.dots[2] = t.seconds % 2;
            d.dots[1] = t.seconds % 2;
            d.dots[0] = t.seconds % 2;
            nixieDisplay6t(&d);
        }
        break;

    default:
        break;
    }
}


void nixieclockSaveTubes()
{
    switch(nixieclockMode)
    {
    case NIXIECLOCK_MODE_NONE:
    case NIXIECLOCK_MODE_HHMM:
    case NIXIECLOCK_MODE_MMSS:
    case NIXIECLOCK_MODE_YYYY:
    {
    	nixieDisplay4t_t display;
    	for(uint8_t j = 0; j < 10; j++)
    	{
			for(uint8_t i = 0; i < 10; i++)
			{
			  display.digits[0] = i;
			  display.digits[1] = i;
			  display.digits[2] = i;
			  display.digits[3] = i;
			  nixieDisplay4t( &display );
			  timer_sleep(10000);
			}
		}
    }
    break;

    case NIXIECLOCK_MODE_HHMMSS:
    case NIXIECLOCK_MODE_HHMMSS_R:
        {
        	nixieDisplay6t_t display;
        	for(uint8_t j = 0; j < 10; j++)
        	{
				for(uint8_t i = 0; i < 10; i++)
				{
				  display.digits[0] = i;
				  display.digits[1] = i;
				  display.digits[2] = i;
				  display.digits[3] = i;
				  display.digits[4] = i;
				  display.digits[5] = i;
				  nixieDisplay6t( &display );
				  timer_sleep(10000);
				}
        	}
        }
        break;

    default:
        break;
    }
}

#endif // CFG_TYPE_NIXIE
