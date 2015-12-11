#ifndef __NIXIE_H__
#define __NIXIE_H__

#include "platform_config.h"

#ifdef CFG_TYPE_NIXIE

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
    NIXIE_MAP_EUROGRID,
    NIXIE_MAP_E13D,
    NIXIE_MAP_ZM1325,
    NIXIE_MAP_END
} nixieMapping_t;

void nixieInit();

void nixieDisplay4t( nixieDisplay4t_t *d );
void nixieDisplay6t( nixieDisplay4t_t *d );

void nixieSetMapping( const nixieMapping_t m );
nixieMapping_t nixieGetMapping();
void nixieStoreMapping( const nixieMapping_t m );
nixieMapping_t nixieLoadMapping();

void nixieHighVoltageEnable( void );
void nixieHighVoltageDisable( void );

#endif // CFG_TYPE_NIXIE

#endif // __NIXIE_H__
