#ifndef __NIXIECLOCK_H__
#define __NIXIECLOCK_H__

#include "platform_config.h"
#include "rtc/rtc_functions.h"

typedef enum
{
    NIXIECLOCK_MODE_NONE = 0,
    NIXIECLOCK_MODE_HHMMSS,
    NIXIECLOCK_MODE_HHMM,
    NIXIECLOCK_MODE_MMSS,
    NIXIECLOCK_MODE_YYYY,
    NIXIECLOCK_MODE_END
} nixieclockMode_t;

extern nixieclockMode_t nixieclockMode;

// ----------------------------------------------------------------------------

void nixieclockInit(void);
void nixieclockShowTime(rtcTime_t t);

void nixieclockStoreMode( const nixieclockMode_t m );
nixieclockMode_t nixieclockLoadMode();
void nixieclockSetMode( const nixieclockMode_t m );
nixieclockMode_t nixieclockGetMode( void );

// ----------------------------------------------------------------------------

#endif // __NIXIECLOCK_H__
