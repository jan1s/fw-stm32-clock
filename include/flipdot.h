#ifndef FLIPDOT_H_
#define FLIPDOT_H_

#ifdef CFG_TYPE_FLIPDOT

#include "stm32f10x.h"
#include "timer.h"

#include <stddef.h>

typedef enum
{
    fdisp_type_84x7,
    fdisp_type_112x16,
} fdisp_type_t;

extern fdisp_type_t flipdotType;

#ifdef CFG_TYPE_FLIPDOT_84X7
typedef struct
{
    uint8_t cols[84];
} fdisp_84x7_t;

extern fdisp_84x7_t flipdotState84x7;
#endif

#ifdef CFG_TYPE_FLIPDOT_112X16
typedef struct
{
    uint16_t cols[112];
} fdisp_112x16_t;

extern fdisp_112x16_t flipdotState112x16;
#endif

// ----------------------------------------------------------------------------

void flipdot_init(void);

// ----------------------------------------------------------------------------
#ifdef CFG_TYPE_FLIPDOT_84X7
void flipdot_wipe_84x7(const uint8_t dir);
void flipdot_set_84x7(const fdisp_84x7_t *d);
void flipdot_setstring_84x7(fdisp_84x7_t *d, uint8_t *s, size_t slen);
#endif


#ifdef CFG_TYPE_FLIPDOT_112X16
void flipdot_wipe_112x16(const uint8_t dir);
void flipdot_set_112x16(const fdisp_112x16_t *d);
#endif
// ----------------------------------------------------------------------------

#endif // CFG_TYPE_FLIPDOT

#endif // FLIPDOT_H_
