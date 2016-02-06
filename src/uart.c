#include "platform_config.h"

#include "stm32f10x_usart.h"


/* RX Ring Buffer */
uint8_t  USART1_Rx_Buffer [CFG_USART1_BUFSIZE];
uint32_t USART1_Rx_In_Ptr = 0;
uint32_t USART1_Rx_Out_Ptr = 0;
uint32_t USART1_Rx_Length = 0;

void uart1Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO |
                           RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    /* TX Pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* RX Pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = CFG_USART1_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_EnableIRQ(USART1_IRQn);
}

void uart1SendChar(uint8_t c)
{
    USART_SendData(USART1, c);

    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {
    }
}

void uart1Send(uint8_t *buffer, uint32_t length)
{
    while(length != 0)
    {
        uint8_t c = *buffer;
        uart1SendChar(c);
        buffer++;
        length--;
    }
}

uint32_t uart1ReadChar(uint8_t *c)
{
    uint32_t available = USART1_Rx_Length;
    if(USART1_Rx_Length > 0)
    {
        *c = USART1_Rx_Buffer[USART1_Rx_Out_Ptr];
        USART1_Rx_Out_Ptr++;
        USART1_Rx_Length--;

        if(USART1_Rx_Out_Ptr >= CFG_USART1_BUFSIZE)
        {
            USART1_Rx_Out_Ptr = 0;
        }
    }
    return available;
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uint8_t c = USART_ReceiveData(USART1);
        USART1_Rx_Buffer[USART1_Rx_In_Ptr] = c;
        USART1_Rx_In_Ptr++;
        USART1_Rx_Length++;

        if(USART1_Rx_In_Ptr == CFG_USART1_BUFSIZE)
        {
            USART1_Rx_In_Ptr = 0;
        }
        if(USART1_Rx_Length == CFG_USART1_BUFSIZE)
        {
            // error discard rx
        }
    }
}
