#include "platform_config.h"

#ifdef CFG_TYPE_NIXIE

#include "nixie.h"
#include "nixie_clock.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "timer.h"

nclock_mode_t nixieClockMode;

// ----------------------------------------------------------------------------

void nixieClockInit()
{
    nixieClockMode = nixieClockLoadMode();

    nixieInit();
    nixieHighVoltageEnable();
}

void nixieClockStoreMode( const nclock_mode_t m )
{
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Write to the BKP Domain */
    BKP_WriteBackupRegister(BKP_DR4, m & 0xFF);

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

void nixieClockShowTime(uint32_t t)
{
    struct tm ts = {0};
    ts = *gmtime(&t);

#ifdef CFG_TYPE_NIXIE_4T
    switch(nixieClockMode)
    {
    case nclock_mode_inactive:
    {

    }
    break;

    case nclock_mode_hhmm:
    {
        nixieDisplay4t_t d;
        d.digits[3] = ts.tm_hour / 10;
        d.digits[2] = ts.tm_hour % 10;
        d.digits[1] = ts.tm_min / 10;
        d.digits[0] = ts.tm_min % 10;
        nixieDisplay4t(&d);
    }
    break;

    case nclock_mode_mmss:
    {
        nixieDisplay4t_t d;
        d.digits[3] = ts.tm_min / 10;
        d.digits[2] = ts.tm_min % 10;
        d.digits[1] = ts.tm_sec / 10;
        d.digits[0] = ts.tm_sec % 10;
        nixieDisplay4t(&d);
    }
    break;

    case nclock_mode_yyyy:
    {
        nixieDisplay4t_t d;
        uint16_t year = ts.tm_year + 1900;
        d.digits[3] = (year / 1000) % 10;
        d.digits[2] = (year / 100) % 10;
        d.digits[1] = (year / 10) % 10;
        d.digits[0] = year % 10;
        nixieDisplay4t(&d);
    }
    break;
    }
#endif // CFG_TYPE_NIXIE_4T
}

#endif // CFG_TYPE_NIXIE

// ----------------------------------------------------------------------------
