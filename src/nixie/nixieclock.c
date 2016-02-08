#include "platform_config.h"

#ifdef CFG_NIXIE

#include "nixie/nixie.h"
#include "nixie/nixieclock.h"
#include "rtc/rtc.h"
#include "rtc/rtc_functions.h"

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
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Read previous state */
    uint16_t bkp = BKP_ReadBackupRegister(BKP_DR4);
    bkp &= ~(0xFF);
    bkp |= m;

    /* Write to the BKP Domain */
    BKP_WriteBackupRegister(BKP_DR4, bkp);

    /* Deny access to BKP Domain */
    PWR_BackupAccessCmd(DISABLE);
}

nixieclockMode_t nixieclockLoadMode()
{
    return (BKP_ReadBackupRegister(BKP_DR4) & 0xFF);
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
        d.dots[1] = t.seconds % 2;
        d.dots[0] = t.seconds % 2;
        nixieDisplay6t(&d);
    }
    break;

    default:
        break;
    }
}

#endif // CFG_TYPE_NIXIE
