
/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"

#include "led.h"
#include "timer.h"
#include "clock.h"
#include "cli/cli.h"
#include "protocol/protocol.h"

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

    led_sys_on();
    timer_sleep(10000);
    led_sys_off();

    cliInit();
    //protocolInit();
    clockInit();

    while(1)
    {
        //led_sys_on();
        //protocolPoll();
        cliPoll();
        //led_sys_off();
        clockPoll();
    }
}
