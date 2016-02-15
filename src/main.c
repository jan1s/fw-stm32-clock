
/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"

#include "led.h"
#include "timer.h"
#include "cli/cli.h"
#include "clock.h"
#include "protocol/protocol.h"
#include "usb_pwr.h"


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
    timer_sleep(50000);
    led_sys_off();

    cliInit(CLI_USBCDC);

    //protocolInit();
    clockInit();

    while(1)
    {
    	led_sys_on();
    	cliPoll(CLI_USBCDC);
        //protocolPoll();
        led_sys_off();
        
        clockPoll();
    }
}
