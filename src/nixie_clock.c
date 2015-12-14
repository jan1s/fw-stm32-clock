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
    nixieClockMode = (BKP_ReadBackupRegister(BKP_DR4) & 0xFF);

    nixieInit();
    nixieHighVoltageEnable();
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
        d.digits[0] = ts.tm_min / 10;
        d.digits[1] = ts.tm_min % 10;
        d.digits[2] = ts.tm_sec / 10;
        d.digits[3] = ts.tm_sec % 10;
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
        d.digits[3] = (ts.tm_year / 1000) % 10;
        d.digits[2] = (ts.tm_year / 100) % 10;
        d.digits[1] = (ts.tm_year / 10) % 10;
        d.digits[0] = ts.tm_year % 10;
        nixieDisplay4t(&d);
    }
    break;
    }
#endif // CFG_TYPE_NIXIE_4T
}

#endif // CFG_TYPE_NIXIE

// ----------------------------------------------------------------------------
