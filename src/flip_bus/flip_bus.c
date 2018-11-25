#include "platform_config.h"

#ifdef CFG_FLIP_BUS

#include "flip_bus/flip_bus.h"
#include "timer.h"
#include <string.h>

#define CFG_FLIPPER_SREN_PIN                 (1)
#define CFG_FLIPPER_SRLA_PIN                 (0)

void flipdot_flip(uint8_t row, uint8_t col, uint8_t dir);

// ----------------------------------------------------------------------------

void flipdot_init()
{
    /* GPIOA, GPIOB and SPI1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/
    /* Confugure SCK and MOSI pins as Alternate Function Push Pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_ResetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << CFG_FLIPPER_SREN_PIN);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_ResetBits((GPIO_TypeDef *)GPIOA_BASE, 1 << 8);


    SPI_InitTypeDef  SPI_InitStructure;

    /* SPI1 configuration ------------------------------------------------------*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    /* Enable SPI1 */
    SPI_Cmd(SPI1, ENABLE);
}

fdisp_84x7_t flipdotState84x7;

void flipdot_wipe_84x7(uint8_t dir)
{
    for(uint8_t col = 0; col < 84; ++col)
    {
        for(uint8_t row = 0; row < 7; ++row)
        {
            flipdot_flip(row, col, dir);
        }
    }

    if(dir == 0)
    {
        memset(&flipdotState84x7, 0, sizeof(fdisp_84x7_t));
    }
    else
    {
        memset(&flipdotState84x7, 0xff, sizeof(fdisp_84x7_t));
    }
}

void flipdot_set_84x7(const fdisp_84x7_t *d)
{
    for(uint8_t col = 0; col < 84; ++col)
    {
        for(uint8_t row = 0; row < 7; ++row)
        {
            uint8_t dir = (d->cols[col] & (1 << row)) >> row;
            uint8_t dir_state = (flipdotState84x7.cols[col] & (1 << row)) >> row;
            if(dir != dir_state)
            {
                flipdot_flip(row, col, dir);
            }
        }
    }
    flipdotState84x7 = *d;
}

#ifdef CFG_TYPE_FLIPDOT_112X16
fdisp_112x16_t flipdotState112x16;

void flipdot_wipe_112x16(uint8_t dir)
{
    for(uint8_t col = 0; col < 112; ++col)
    {
        for(uint8_t row = 0; row < 16; ++row)
        {
            flipdot_flip(row, col, dir);
        }
    }

    if(dir == 0)
    {
        memset(&flipdotState112x16, 0, sizeof(fdisp_112x16_t));
    }
    else
    {
        memset(&flipdotState112x16, 0xff, sizeof(fdisp_112x16_t));
    }
}

void flipdot_set_112x16(const fdisp_112x16_t *d)
{
    for(uint8_t col = 0; col < 112; ++col)
    {
        for(uint8_t row = 0; row < 16; ++row)
        {
            uint8_t dir = (d->cols[col] & (1 << row)) >> row;
            uint8_t dir_state = (flipdotState112x16.cols[col] & (1 << row)) >> row;
            if(dir != dir_state)
            {
                flipdot_flip(row, col, dir);
            }
        }
    }
    flipdotState112x16 = *d;
}
#endif

void  flipdot_flip(uint8_t row, uint8_t col, uint8_t dir)
{
    uint8_t buf[3] = {0x00, 0x00, 0x00};

    row = row + (row / 7) + 1;
    col = col + (col / 7) + 1;

    buf[2] |= row & 0b00011111;
    buf[2] |= 0b00100000; // select first

    buf[1] |= col & 0b00011111;

    if(dir)
    {
        buf[1] &= ~(0b00100000);
    }
    else
    {
        buf[1] |= 0b00100000;
    }

    for(uint32_t i = 0; i < 128; ++i)
    {
        uint8_t sel = (col & 0b11100000) >> 5;
        if(sel == 0)
        {
            buf[1] |= 0b01000000; // toggle unfug bit
        }
        else if(sel == 1)
        {
            buf[1] |= 0b10000000; // toggle unfug bit
        }
        else if(sel == 2)
        {
            buf[0] |= 0b00000001; // toggle unfug bit
        }
        else if(sel == 3)
        {
            buf[0] |= 0b00000010; // toggle unfug bit
        }


        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, buf[0]);
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, buf[1]);
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, buf[2]);
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

        GPIO_SetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << CFG_FLIPPER_SRLA_PIN);
        GPIO_ResetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << CFG_FLIPPER_SRLA_PIN);

        timer_sleep(1);

        if(sel == 0)
        {
            buf[1] &= ~0b01000000; // toggle unfug bit
        }
        else if(sel == 1)
        {
            buf[1] &= ~0b10000000; // toggle unfug bit
        }
        else if(sel == 2)
        {
            buf[0] &= ~0b00000001; // toggle unfug bit
        }
        else if(sel == 3)
        {
            buf[0] &= ~0b00000010; // toggle unfug bit
        }

        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, buf[0]);
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, buf[1]);
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, buf[2]);
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

        GPIO_SetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << CFG_FLIPPER_SRLA_PIN);
        GPIO_ResetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << CFG_FLIPPER_SRLA_PIN);

        timer_sleep(1);
    }
}

#endif // CFG_FLIPDOT
