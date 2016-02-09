#include "platform_config.h"

#include "print.h"
#include "uart.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

void print(void (*send)(uint8_t *,uint32_t), const char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start (args, format);
    vsnprintf(buffer, 255, format, args);
    (*send)((uint8_t*)buffer, strlen(buffer));
    va_end(args);
}
