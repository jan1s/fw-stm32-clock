#include "platform_config.h"

void uart1Init(void);
void uart2Init(void);
void uart1SendChar(uint8_t c);
void uart2SendChar(uint8_t c);
void uart1Send(uint8_t *buffer, uint32_t length);
void uart2Send(uint8_t *buffer, uint32_t length);
uint32_t uart1ReadChar(uint8_t *c);
uint32_t uart2ReadChar(uint8_t *c);
