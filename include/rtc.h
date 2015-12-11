#ifndef RTC_H_
#define RTC_H_

#include "platform_config.h"
#include <stddef.h>

void rtcInit(void);
uint32_t rtcGet(void);
void rtcSet(uint32_t t);

#endif // RTC_H_
