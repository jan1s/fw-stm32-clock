#ifndef NIXIE_CLOCK_H_
#define NIXIE_CLOCK_H_

#include "platform_config.h"

typedef enum
{
    nclock_mode_inactive,
    nclock_mode_carp,
    nclock_mode_bass
} nclock_mode_t;

extern nclock_mode_t nixieClockMode;

// ----------------------------------------------------------------------------

void nixieClockInit(void);
void nixieClockShowTime(uint32_t t);

// ----------------------------------------------------------------------------

#endif // NIXIE_CLOCK_H_
