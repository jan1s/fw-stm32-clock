
/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"

#include "led.h"
#include "timer.h"
#include "clock.h"
#include "protocol.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
    led_init();
    timer_init();

    protocolInit();
    clockInit();

    while(1)
    {
        led_sys_on();
        protocolPoll();
        led_sys_off();
        clockPoll();
    }
}
