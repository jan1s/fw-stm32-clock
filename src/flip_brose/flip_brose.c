#include "platform_config.h"

#ifdef CFG_FLIP_BROSE

#include "flip_brose/flip_brose.h"
#include "timer.h"
#include <string.h>

#define CFG_FLIPPER_SREN_PIN                 (1)
#define CFG_FLIPPER_SRLA_PIN                 (0)

#define CFG_FLIPPER_VSEN_PIN                 (4)

void flipdot_flip(uint8_t row, uint8_t col, uint8_t dir);

// ----------------------------------------------------------------------------

void flipdot_init()
{
    /* GPIOA, GPIOB and SPI1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/
    /* Configure SCK and MOSI pins as Alternate Function Push Pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure VS EN (Enable 24V Power Switch) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_ResetBits((GPIO_TypeDef *)GPIOA_BASE, 1 << CFG_FLIPPER_VSEN_PIN);

    /* Configure VS ST (24V Power Switch Status) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure Latch and EN pins for Shift Registers */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_ResetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << CFG_FLIPPER_SREN_PIN);

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

    /* Set System into a safe state */
    uint8_t buf[8] = {0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    for (uint8_t i = 0; i < sizeof(buf); ++i)
    {
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, buf[i]);
    }
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    timer_sleep(1);
	GPIO_SetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << CFG_FLIPPER_SRLA_PIN);
	GPIO_ResetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << CFG_FLIPPER_SRLA_PIN);
	timer_sleep(1);

    GPIO_SetBits((GPIO_TypeDef *)GPIOA_BASE, 1 << CFG_FLIPPER_VSEN_PIN);
}

fdisp_21x13_t flipdotState21x13;

void flipdot_wipe_21x13(uint8_t dir)
{
    for(uint8_t col = 0; col < 21; ++col)
    {
        for(uint8_t row = 0; row < 13; ++row)
        {
            flipdot_flip(row+6, col+7, dir);
            //timer_sleep(2000);
        }
    }

    if(dir == 0)
    {
        memset(&flipdotState21x13, 0, sizeof(fdisp_21x13_t));
    }
    else
    {
        memset(&flipdotState21x13, 0xff, sizeof(fdisp_21x13_t));
    }
}

void flipdot_set_21x13(const fdisp_21x13_t *d)
{
    for(uint8_t col = 0; col < 21; ++col)
    {
        for(uint8_t row = 0; row < 13; ++row)
        {
            uint8_t dir = (d->cols[col] & (1 << row)) >> row;
            uint8_t dir_state = (flipdotState21x13.cols[col] & (1 << row)) >> row;
            if(dir != dir_state)
            {
                flipdot_flip(row+6, col+7, dir);
            }
        }
    }
    flipdotState21x13 = *d;
}


void  flipdot_flip(uint8_t row, uint8_t col, uint8_t dir)
{
    uint8_t buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    // select column
    col = col + 1 + (col/7);
    uint8_t fp2800_digit = (col >> 3);
    uint8_t fp2800_segment = (col & 0b00000111);

    buf[1] = 0x7c;
    buf[1] &= ~((fp2800_segment & 0b00000111) << 2);
    buf[1] &= ~((fp2800_digit & 0b00000011) << 5);

    // select data
    buf[1] |= (dir & 0b00000001) << 1;
    if(dir)
    {
        // select anodes
    	uint32_t r = 1 << row;
        buf[7] = (uint32_t)(r & 0b01111111) << 1;
        buf[6] = (uint32_t)(r & (0b01111111 << 7)) >> 6;
        buf[5] = (uint32_t)(r & (0b01111111 << 14)) >> 13;
    }
    else
    {
        // select cathodes
    	uint32_t r = 1 << row;
        buf[4] = (uint32_t)(r & 0b01111111) << 1;
        buf[3] = (uint32_t)(r & (0b01111111 << 7)) >> 6;
        buf[2] = (uint32_t)(r & (0b01111111 << 14)) >> 13;
    }

    // write data first
    uint8_t adr = 0x01;
    buf[1] &= ~((adr & 0b00000001) << 7);
    buf[0] &= ~(adr & 0b11111110);
    for (uint8_t i = 0; i < sizeof(buf); ++i)
    {
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, buf[i]);
    }
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    timer_sleep(1);
    GPIO_SetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << CFG_FLIPPER_SRLA_PIN);
    GPIO_ResetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << CFG_FLIPPER_SRLA_PIN);
    timer_sleep(1);

    // set enable
    buf[1] |= (adr & 0b00000001) << 7;
    buf[0] |= (adr & 0b11111110);
    for (uint8_t i = 0; i < sizeof(buf); ++i)
    {
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, buf[i]);
    }
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    timer_sleep(1);
    GPIO_SetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << CFG_FLIPPER_SRLA_PIN);
    GPIO_ResetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << CFG_FLIPPER_SRLA_PIN);
    timer_sleep(100);

    // reset enable
    buf[1] &= ~((adr & 0b00000001) << 7);
    buf[0] &= ~(adr & 0b11111110);
    for (uint8_t i = 0; i < sizeof(buf); ++i)
    {
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, buf[i]);
    }
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    timer_sleep(1);
    GPIO_SetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << CFG_FLIPPER_SRLA_PIN);
    GPIO_ResetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << CFG_FLIPPER_SRLA_PIN);
    timer_sleep(1);
}

#endif // CFG_FLIP_BROSE
