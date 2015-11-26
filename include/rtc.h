#ifndef RTC_H_
#define RTC_H_

#include "stm32f10x.h"

#include <stddef.h>


// ----------------------------------------------------------------------------

void rtcInit(void);

// ----------------------------------------------------------------------------

uint32_t rtcGet(void);
void rtcSet(uint32_t t);

// ----------------------------------------------------------------------------

#endif // RTC_H_
