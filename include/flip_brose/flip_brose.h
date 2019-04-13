#ifndef FLIP_BROSE_H_
#define FLIP_BROSE_H_

#include "platform_config.h"
#include <stddef.h>

#ifdef CFG_FLIP_BROSE

extern const uint8_t font_5x7[96][4];

void flipdot_init(void);

typedef struct
{
    uint16_t cols[21];
} fdisp_21x13_t;
extern fdisp_21x13_t flipdotState21x13;

typedef struct
{
    uint16_t cols[28];
} fdisp_28x16_t;
extern fdisp_28x16_t flipdotState28x16;

void flipdot_wipe_21x13(const uint8_t dir);
void flipdot_set_21x13(const fdisp_21x13_t *d);

void flipdot_wipe_28x16(const uint8_t dir);
void flipdot_set_28x16(const fdisp_28x16_t *d);

void flipdot_setstring_HHMM_21x13(fdisp_21x13_t *d, uint8_t *s, size_t slen);
void flipdot_setstring_HHMMddmm_21x13(fdisp_21x13_t *d, uint8_t *s, size_t slen);

#endif // CFG_FLIP_BROSE

#endif // FLIP_BROSE_H_
