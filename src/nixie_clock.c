#include "platform_config.h"

#ifdef CFG_TYPE_NIXIE

#include "nixie.h"
#include "nixie_clock.h"
#include "rtc.h"
#include "rtc_functions.h"

#include <stdio.h>
#include <string.h>

nclock_mode_t nixieClockMode;

// ----------------------------------------------------------------------------

void nixieClockInit()
{
    //nixieClockMode = nixieClockLoadMode();
    nixieClockMode = nclock_mode_hhmm;

    nixieInit();
    nixieHighVoltageEnable();
}

void nixieClockStoreMode( const nclock_mode_t m )
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

nclock_mode_t nixieClockLoadMode()
{
    return (BKP_ReadBackupRegister(BKP_DR4) & 0xFF);
}

void nixieClockSetMode( const nclock_mode_t m )
{
    nixieClockMode = m;
}

nclock_mode_t nixieClockGetMode( void )
{
    return nixieClockMode;
}

void nixieClockShowTime(rtcTime_t t)
{
#ifdef CFG_TYPE_NIXIE_4T
    switch(nixieClockMode)
    {
    case nclock_mode_inactive:
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

    case nclock_mode_hhmm:
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

    case nclock_mode_mmss:
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

    case nclock_mode_yyyy:
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
    }
#endif // CFG_TYPE_NIXIE_4T
}

#endif // CFG_TYPE_NIXIE

// ----------------------------------------------------------------------------
