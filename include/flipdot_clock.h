#ifndef FLIPDOT_CLOCK_H_
#define FLIPDOT_CLOCK_H_

#include "stm32f10x.h"

typedef enum
{
    fclock_mode_inactive,
    fclock_mode_carp,
    fclock_mode_bass,
    fclock_mode_catfish,
    fclock_mode_perch,
    fclock_mode_redfish,
    fclock_mode_salmon,
    fclock_mode_trout,
    fclock_mode_walleye,
    fclock_mode_sunfish
} fclock_mode_t;

extern fclock_mode_t flipdotClockMode;

// ----------------------------------------------------------------------------

void flipdotClockInit(void);
void flipdotClockShowTime(uint32_t t);

// ----------------------------------------------------------------------------

#endif // FLIPDOT_CLOCK_H_
