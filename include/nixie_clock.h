#ifndef NIXIE_CLOCK_H_
#define NIXIE_CLOCK_H_

#include "platform_config.h"

#ifdef CFG_TYPE_NIXIE_4T
typedef enum
{
    nclock_mode_inactive,
    nclock_mode_hhmm,
    nclock_mode_mmss,
    nclock_mode_yyyy
} nclock_mode_t;
#endif // CFG_TYPE_NIXIE_4T

extern nclock_mode_t nixieClockMode;

// ----------------------------------------------------------------------------

void nixieClockInit(void);
void nixieClockShowTime(uint32_t t);

void nixieClockStoreMode( const nclock_mode_t m );
nclock_mode_t nixieClockLoadMode();
void nixieClockSetMode( const nclock_mode_t m );
nclock_mode_t nixieClockGetMode( void );

// ----------------------------------------------------------------------------

#endif // NIXIE_CLOCK_H_
