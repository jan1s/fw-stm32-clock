#ifndef __NIXIE_H__
#define __NIXIE_H__

#include "platform_config.h"

#ifdef CFG_NIXIE

typedef struct
{
    uint8_t digits[4];
    uint8_t dots[2];
} nixieDisplay4t_t;

typedef struct
{
    uint8_t digits[6];
    uint8_t dots[4];
} nixieDisplay6t_t;

typedef enum
{
    NIXIE_TYPE_NONE = 0,
    NIXIE_TYPE_A,
    NIXIE_TYPE_B,
    NIXIE_TYPE_C,
    NIXIE_TYPE_D,
    NIXIE_TYPE_E,
    NIXIE_TYPE_F,
    NIXIE_TYPE_G,
    NIXIE_TYPE_END
} nixieMapping_t;

void nixieInit();

void nixieDisplay4t( nixieDisplay4t_t *d );
void nixieDisplay6t( nixieDisplay6t_t *d );

void nixieSetMapping( const nixieMapping_t m );
nixieMapping_t nixieGetMapping();
void nixieStoreMapping( const nixieMapping_t m );
nixieMapping_t nixieLoadMapping();

void nixieHighVoltageEnable( void );
void nixieHighVoltageDisable( void );

#endif

#endif // __NIXIE_H__
