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

    switch(nixieClockMode)
    {
    case nclock_mode_carp:
    {
#ifdef CFG_TYPE_NIXIE_4T
        nixieDisplay4t_t d;
        d.digits[0] = ts.tm_hour / 10;
        d.digits[1] = ts.tm_hour % 10;
        d.digits[2] = ts.tm_min / 10;
        d.digits[3] = ts.tm_min % 10;
        nixieDisplay4t(&d);
#endif
    }
    break;

    case nclock_mode_bass:
    {
#ifdef CFG_TYPE_NIXIE_4T
        nixieDisplay4t_t d;
        d.digits[0] = ts.tm_min / 10;
        d.digits[1] = ts.tm_min % 10;
        d.digits[2] = ts.tm_sec / 10;
        d.digits[3] = ts.tm_sec % 10;
        nixieDisplay4t(&d);
#endif
    }
    break;

    default:
    {
#ifdef CFG_TYPE_NIXIE_4T
        nixieDisplay4t_t d;
        d.digits[3] = ts.tm_min / 10;
        d.digits[2] = ts.tm_min % 10;
        d.digits[1] = ts.tm_sec / 10;
        d.digits[0] = ts.tm_sec % 10;
        nixieDisplay4t(&d);
#endif
    }
    break;
    }
}

#endif // CFG_TYPE_NIXIE

// ----------------------------------------------------------------------------
