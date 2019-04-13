#include "platform_config.h"

#ifdef CFG_FLIP_BROSE

#include "flip_brose/flip_brose.h"

/*
Clockface 5x7 21x13: 
  HH:MM dd.mm

  012345678901234567890

0 HHHH_HHHH___MMMM_MMMM
1 HHHH_HHHH_1_MMMM_MMMM
2 HHHH_HHHH_1_MMMM_MMMM
3 HHHH_HHHH___MMMM_MMMM
4 HHHH_HHHH_1_MMMM_MMMM
5 HHHH_HHHH_1_MMMM_MMMM
6 _____________________
7 dddd_dddd___mmmm_mmmm
8 dddd_dddd___mmmm_mmmm
9 dddd_dddd___mmmm_mmmm
0 dddd_dddd___mmmm_mmmm
1 dddd_dddd_1_mmmm_mmmm
2 dddd_dddd_1_mmmm_mmmm
*/

void flipdot_setstring_HHMMddmm_21x13(fdisp_21x13_t *d, uint8_t *s, size_t slen)
{
    for(uint8_t i = 0; (i < slen) && (i < 9); ++i)
    {
    	d->cols[10] = 0x0036;
        for(uint8_t col = 0; col < 4; ++col)
        {
            if(i < 2)
            {
                d->cols[col + (i*5)] = font_5x7[s[i]-32][col];
            }
            else if(i < 4)
            {
                d->cols[col + (i*5) + 2] = font_5x7[s[i]-32][col];
            }
            /*
            else if(i < 7)
            {
                d->cols[col + ((i-4)*5)] |= font_5x7[s[i]-32][col] << 7;
            }
            else if(i < 9)
            {
                d->cols[col + ((i-4)*5) + 2] |= font_5x7[s[i]-32][col] << 7;
            }
            */
        }
    }
}

/*
Clockface 5x7 21x13: 
  HH:MM

  012345678901234567890

0 _____________________
1 _____________________
2 111111111111111111111
3 _____________________
4 HHHH_HHHH___MMMM_MMMM
5 HHHH_HHHH_1_MMMM_MMMM
6 HHHH_HHHH_1_MMMM_MMMM
7 HHHH_HHHH___MMMM_MMMM
8 HHHH_HHHH_1_MMMM_MMMM
9 HHHH_HHHH_1_MMMM_MMMM
0 _____________________
1 111111111111111111111
2 _____________________


*/

void flipdot_setstring_HHMM_21x13(fdisp_21x13_t *d, uint8_t *s, size_t slen)
{
    for(uint8_t i = 0; (i < slen) && (i < 5); ++i)
    {
    	d->cols[10] = (0x0036 << 4) + (1<<2) + (1<<11);
        for(uint8_t col = 0; col < 4; ++col)
        {
            if(i < 2)
            {
                d->cols[col + (i*5)] |= (font_5x7[s[i]-32][col] << 4) + (1<<2) + (1<<11);
            }
            else if(i < 4)
            {
                d->cols[col + (i*5) + 2] |= (font_5x7[s[i]-32][col] << 4) + (1<<2) + (1<<11);
            }
            /*
            else if(i < 7)
            {
                d->cols[col + ((i-4)*5)] |= font_5x7[s[i]-32][col] << 7;
            }
            else if(i < 9)
            {
                d->cols[col + ((i-4)*5) + 2] |= font_5x7[s[i]-32][col] << 7;
            }
            */
        }
    }
    for(uint8_t i = 0; i < 21; ++i)
	{
		d->cols[i] |= (1<<2) + (1<<11);
	}
}

#endif // CFG_FLIP_BROSE
