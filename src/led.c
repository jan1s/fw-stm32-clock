#include "led.h"
#include "stm32f10x_conf.h"


void led_init()
{
    /* GPIOA, GPIOB and SPI1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_ResetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << 10);
    GPIO_ResetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << 11);
}

void led_sys_on()
{
    GPIO_SetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << 10);
}

void led_sys_off()
{
    GPIO_ResetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << 10);
}

void led_usr_on()
{
    GPIO_SetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << 11);
}

void led_usr_off()
{
    GPIO_ResetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << 11);
}