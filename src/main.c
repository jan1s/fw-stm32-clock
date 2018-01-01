
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

/* Virtual address of EEPROM emulated variables */
uint16_t VirtAddVarTab[NumbOfVar] =
{
    CFG_EEPROM_TZ_STD,
    CFG_EEPROM_TZ_DST,
    CFG_EEPROM_CLOCK_SRC,
    CFG_EEPROM_NIXIE_TYPE,
    CFG_EEPROM_NIXIE_MODE
};

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

    //cliInit(CLI_USBCDC);
    cliInit(CLI_USART1);
    //cliInit(CLI_USART2);

    //protocolInit();
    clockInit();

    while(1)
    {
    	led_sys_on();
    	//cliPoll(CLI_USBCDC);
    	cliPoll(CLI_USART1);
    	//cliPoll(CLI_USART2);
        //protocolPoll();
        led_sys_off();
        
        clockPoll();
    }
}
