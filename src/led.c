#include "platform_config.h"
#include "led.h"

#ifdef CFG_FLIPDOT
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
#endif

#ifdef CFG_NIXIE
void led_init()
{
    /* GPIOA, GPIOB and SPI1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_SetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << 9);
    GPIO_SetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << 8);
}

void led_sys_on()
{
    GPIO_ResetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << 9);
}

void led_sys_off()
{
    GPIO_SetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << 9);
}

void led_usr_on()
{
    GPIO_ResetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << 8);
}

void led_usr_off()
{
    GPIO_SetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << 8);
}
#endif
