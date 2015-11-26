#include "platform_config.h"

#include "stm32f10x_usart.h"

#include "protocol.h"
#include <string.h>

typedef enum
{
    PROTOCOL_DECODER_STATE_SYNC_0 = 0,
    PROTOCOL_DECODER_STATE_SYNC_1,
    PROTOCOL_DECODER_STATE_CLASS_ID,
    PROTOCOL_DECODER_STATE_MSG_ID,
    PROTOCOL_DECODER_STATE_LENGTH_0,
    PROTOCOL_DECODER_STATE_LENGTH_1,
    PROTOCOL_DECODER_STATE_PAYLOAD,
    PROTOCOL_DECODER_STATE_CHECKSUM_0,
    PROTOCOL_DECODER_STATE_CHECKSUM_1
} protocolDecoderState_t;


/* RX Ring Buffer */
#define USART1_RX_BUFFER_SIZE 1024
uint8_t  USART1_Rx_Buffer [USART1_RX_BUFFER_SIZE];
uint32_t USART1_Rx_In_Ptr = 0;
uint32_t USART1_Rx_Out_Ptr = 0;
uint32_t USART1_Rx_Length = 0;


void protocolInit(void)
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

    USART_InitStructure.USART_BaudRate = 115200;
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


void protocolPoll(void)
{
    static protocolPacket_t packet;

    if(protocolGetPacket(&packet))
    {
        if(protocolCheckPacket(&packet))
        {
            if(protocolEvaluatePacket(&packet))
            {
                // finish
            }
            else
            {
                // received not implemented packet
                // spit out some error
            }
        }
        else
        {
            // spit out some error
        }
    }
}

bool protocolGetPacket(protocolPacket_t *packet)
{
    static protocolDecoderState_t state;
    static uint16_t length;
    bool isWholePacket = false;

    while(USART1_Rx_Length > 0)
    {
        uint8_t c = USART1_Rx_Buffer[USART1_Rx_Out_Ptr];
        USART1_Rx_Out_Ptr++;
        USART1_Rx_Length--;

        if(USART1_Rx_Out_Ptr >= USART1_RX_BUFFER_SIZE)
        {
            USART1_Rx_Out_Ptr = 0;
        }

        switch(state)
        {
        case PROTOCOL_DECODER_STATE_SYNC_0:
            state = (c == PROTOCOL_SYNC_0) ? PROTOCOL_DECODER_STATE_SYNC_1 : PROTOCOL_DECODER_STATE_SYNC_0;
            break;

        case PROTOCOL_DECODER_STATE_SYNC_1:
            state = (c == PROTOCOL_SYNC_1) ? PROTOCOL_DECODER_STATE_CLASS_ID : PROTOCOL_DECODER_STATE_SYNC_0;
            break;

        case PROTOCOL_DECODER_STATE_CLASS_ID:
            packet->msgId = c;
            state = PROTOCOL_DECODER_STATE_MSG_ID;
            break;

        case PROTOCOL_DECODER_STATE_MSG_ID:
            packet->msgId += (c << 8);
            state = PROTOCOL_DECODER_STATE_LENGTH_0;
            break;

        case PROTOCOL_DECODER_STATE_LENGTH_0:
            length = c;
            state = PROTOCOL_DECODER_STATE_LENGTH_1;
            break;

        case PROTOCOL_DECODER_STATE_LENGTH_1:
            length += (c << 8);
            packet->payloadLength = length;
            if(length > 0)
            {
                state = PROTOCOL_DECODER_STATE_PAYLOAD;
            }
            else
            {
                state = PROTOCOL_DECODER_STATE_CHECKSUM_0;
            }
            break;

        case PROTOCOL_DECODER_STATE_PAYLOAD:
            packet->payload[packet->payloadLength - length--] = c;
            if(length == 0)
            {
                state = PROTOCOL_DECODER_STATE_CHECKSUM_0;
            }
            break;

        case PROTOCOL_DECODER_STATE_CHECKSUM_0:
            packet->checksum = c;
            state = PROTOCOL_DECODER_STATE_CHECKSUM_1;
            break;

        case PROTOCOL_DECODER_STATE_CHECKSUM_1:
            packet->checksum += (c << 8);
            state = PROTOCOL_DECODER_STATE_SYNC_0;
            isWholePacket = true;
            break;
        }

        return isWholePacket;
    }
    return isWholePacket;
}

void protocolReplyPacket(uint16_t msgId)
{
    protocolPacket_t packet;
    packet.sync[0] = PROTOCOL_SYNC_0;
    packet.sync[1] = PROTOCOL_SYNC_1;
    packet.msgId = msgId;
    packet.payloadLength = 0;
    packet.checksum = protocolCalculateChecksum(&packet);
    protocolSendPacket(&packet);
}

void protocolSendPacket(protocolPacket_t *packet)
{
    protocolSendChar(packet->sync[0]);
    protocolSendChar(packet->sync[1]);
    protocolSendChar(packet->msgId & 0xFF);
    protocolSendChar(packet->msgId >> 8);
    protocolSendChar(packet->payloadLength & 0xFF);
    protocolSendChar(packet->payloadLength >> 8);

    for(uint16_t i = 0; i < packet->payloadLength; ++i)
    {
        protocolSendChar(packet->payload[i]);
    }

    protocolSendChar(packet->checksum & 0xFF);
    protocolSendChar(packet->checksum >> 8);
}

bool protocolSendChar(uint8_t c)
{
    USART_SendData(USART1, c);

    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {
    }

    return true;
}

bool protocolCheckPacket(const protocolPacket_t *packet)
{
    uint16_t cs = protocolCalculateChecksum(packet);
    return (packet->checksum == cs);
}

uint16_t protocolCalculateChecksum(const protocolPacket_t *packet)
{
    uint8_t a = 0;
    uint8_t b = 0;

    a = a + (packet->msgId & 0xFF);
    b = b + a;

    a = a + (packet->msgId >> 8);
    b = b + a;

    a = a + (packet->payloadLength & 0xFF);
    b = b + a;

    a = a + (packet->payloadLength >> 8);
    b = b + a;

    for(uint16_t i = 0; i < packet->payloadLength; ++i)
    {
        a = a + packet->payload[i];
        b = b + a;
    }

    return (b << 8) + a;
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uint8_t c = USART_ReceiveData(USART1);
        USART1_Rx_Buffer[USART1_Rx_In_Ptr] = c;
        USART1_Rx_In_Ptr++;
        USART1_Rx_Length++;

        if(USART1_Rx_In_Ptr == USART1_RX_BUFFER_SIZE)
        {
            USART1_Rx_In_Ptr = 0;
        }
        if(USART1_Rx_Length == USART1_RX_BUFFER_SIZE)
        {
            // error discard rx
        }
    }
}
