#ifndef __RTC_H__
#define __RTC_H__

#include "platform_config.h"
#include <stddef.h>

void rtcInit(void);
uint32_t rtcGet(void);
void rtcSet(uint32_t t);

#endif
