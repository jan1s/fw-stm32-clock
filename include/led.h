#ifndef LED_H_
#define LED_H_

#include "stm32f10x.h"

// ----------------------------------------------------------------------------
void led_init(void);
void led_sys_on(void);
void led_sys_off(void);
void led_usr_on(void);
void led_usr_off(void);

// ----------------------------------------------------------------------------

#endif // LED_H_
