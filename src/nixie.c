#include "platform_config.h"

#ifdef CFG_TYPE_NIXIE

#include <string.h>
#include "nixie.h"


#define CFG_NIXIE_RCK_PIN       (6)
#define CFG_NIXIE_SREN_PIN      (4)
#define CFG_NIXIE_HVEN_PIN      (0)

nixieMapping_t mapping;

void nixieInit()
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

    /* Enable SREN and RCK */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_ResetBits((GPIO_TypeDef *)GPIOA_BASE, 1 << CFG_NIXIE_SREN_PIN);
    GPIO_ResetBits((GPIO_TypeDef *)GPIOA_BASE, 1 << CFG_NIXIE_RCK_PIN);

    /* Enable HVEN */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_ResetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << CFG_NIXIE_HVEN_PIN);


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

    LPC_GPIO->DIR[CFG_NIXIE_RCK_PORT] |= (1 << CFG_NIXIE_RCK_PIN);
    LPC_GPIO->DIR[CFG_NIXIE_SCL_PORT] |= (1 << CFG_NIXIE_SCL_PIN);
    LPC_GPIO->DIR[CFG_NIXIE_SREN_PORT] |= (1 << CFG_NIXIE_SREN_PIN);
    LPC_GPIO->DIR[CFG_NIXIE_HVEN_PORT] |= (1 << CFG_NIXIE_HVEN_PIN);

    nixieGetMapping(&mapping);
    nixieHighVoltageEnable();
}

void nixieSetMapping( const nixieMapping_t m )
{
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Write to the BKP Domain */
    BKP_WriteBackupRegister(BKP_DR4, m & 0xFF);

    /* Deny access to BKP Domain */
    PWR_BackupAccessCmd(DISABLE);
}

nixieMapping_t nixieGetMapping()
{
    return (BKP_ReadBackupRegister(BKP_DR4) & 0xFF);
}

#ifdef CFG_TYPE_NIXIE_4T
void nixieDisplay4t( nixieDisplay4t_t *d )
{
    // output buffer -> gets written into shift registers in the end
    uint8_t outBuf[] = {0, 0, 0, 0, 0, 0};
    uint8_t outBufCount = 0;
    uint8_t outBufBitCount = 0;
    uint8_t outBufBitMask = 0x80;

    //Do thing for each digit
    for(uint8_t digitCount = 0; digitCount < 4; ++digitCount)
    {
        // catch seperation dots
        // nicer way would be nice
        if( digitCount == 2)
        {
            outBuf[outBufCount] |= outBufBitMask;
            outBufBitMask = outBufBitMask >> 1;
            outBuf[outBufCount] |= outBufBitMask;
            outBufBitMask = outBufBitMask >> 1;
            outBufBitCount = outBufBitCount + 2;
        }

        // get mapping for the digit to be displayed
        uint16_t digitMapping = digitMap[mapping][d->digits[digitCount]];

        // iterate over every digit, to be displayed.
        for(uint8_t digit_bit_count = 0; digit_bit_count < 11; ++digit_bit_count)
        {
            // check if bit should be set
            if(digitMapping & 0x0200)
            {
                outBuf[outBufCount] |= outBufBitMask;
            }

            // increment checking position
            digitMapping = digitMapping << 1;
            outBufBitMask = outBufBitMask >> 1;
            outBufBitCount++;

            // one shift register is full
            if(outBufBitCount >= 8)
            {
                outBufCount++;
                outBufBitCount = 0;
                outBufBitMask = 0x80;
            }
        }
    }

    nixieRegisterWrite(outBuf, sizeof(outBuf));
}
#endif // CFG_TYPE_NIXIE_4T

#ifdef CFG_TYPE_NIXIE_6T
void nixieDisplay6t( nixieDisplay6t_t *d )
{
    // output buffer -> gets written into shift registers in the end
    uint8_t outBuf[] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t outBufCount = 0;
    uint8_t outBufBitCount = 0;
    uint8_t outBufBitMask = 0x80;

    //Do thing for each digit
    for(uint8_t digitCount = 0; digitCount < 6; ++digitCount)
    {
        // catch seperation dots
        // nicer way would be nice
        if( digitCount == 2 || digitCount == 4 )
        {
            outBuf[outBufCount] |= outBufBitMask;
            outBufBitMask = outBufBitMask >> 1;
            outBuf[outBufCount] |= outBufBitMask;
            outBufBitMask = outBufBitMask >> 1;
            outBufBitCount = outBufBitCount + 2;
        }

        // get mapping for the digit to be displayed
        uint16_t digitMapping = digitMap[mapping][d->digits[digitCount]];

        // iterate over every digit, to be displayed.
        for(uint8_t digit_bit_count = 0; digit_bit_count < 10; ++digit_bit_count)
        {
            // check if bit should be set
            if(digitMapping & 0x0200)
            {
                outBuf[outBufCount] |= outBufBitMask;
            }

            // increment checking position
            digitMapping = digitMapping << 1;
            outBufBitMask = outBufBitMask >> 1;
            outBufBitCount++;

            // one shift register is full
            if(outBufBitCount >= 8)
            {
                outBufCount++;
                outBufBitCount = 0;
                outBufBitMask = 0x80;
            }
        }
    }

    nixieRegisterWrite(outBuf, sizeof(outBuf));
}
#endif // CFG_TYPE_NIXIE_6T

nixieRegisterWrite(uint8_t *buf, size_t length)
{
    // send the whole bitbanana
    for(uint8_t i = 0; i < length; ++i)
    {
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, buf[i]);
    }
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

    // shift register values to outputs
    GPIO_ResetBits((GPIO_TypeDef *)GPIOA_BASE, 1 << CFG_NIXIE_RCK_PIN);
    GPIO_SetBits((GPIO_TypeDef *)GPIOA_BASE, 1 << CFG_NIXIE_RCK_PIN);

    // enable the display, if not already happend
    GPIO_ResetBits((GPIO_TypeDef *)GPIOA_BASE, 1 << CFG_NIXIE_SREN_PIN);
}

void nixieHighVoltageEnable ( void )
{
    GPIO_SetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << CFG_NIXIE_HVEN_PIN);
}

void nixieHighVoltageDisable ( void )
{
    GPIO_ResetBits((GPIO_TypeDef *)GPIOB_BASE, 1 << CFG_NIXIE_HVEN_PIN);
}

#endif // CFG_TYPE_NIXIE

