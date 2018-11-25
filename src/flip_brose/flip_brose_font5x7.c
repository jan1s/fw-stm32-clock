#include "platform_config.h"

#ifdef CFG_FLIP_BROSE

#include "flip_brose/flip_brose.h"

// 5x7 font, 32 char offset
const uint8_t font_5x7[96][4] =
{
    {0x00, 0x00, 0x00, 0x00}, // SPACE

    {0x00, 0x00, 0x00, 0x00}, // !
    {0x00, 0x00, 0x00, 0x00}, // "
    {0x00, 0x00, 0x00, 0x00}, // #
    {0x00, 0x00, 0x00, 0x00}, // $
    {0x00, 0x00, 0x00, 0x00}, // %
    {0x00, 0x00, 0x00, 0x00}, // &
    {0x00, 0x00, 0x00, 0x00}, // '
    {0x00, 0x00, 0x00, 0x00}, // (
    {0x00, 0x00, 0x00, 0x00}, // )
    {0x00, 0x00, 0x00, 0x00}, // *
    {0x00, 0x00, 0x00, 0x00}, // +
    {0x00, 0x00, 0x00, 0x00}, // ,
    {0x00, 0x00, 0x00, 0x00}, // -
    {0x00, 0x00, 0x00, 0x00}, // .
    {0x00, 0x00, 0x00, 0x00}, // /

    {0x1E, 0x21, 0x21, 0x1E}, // 0
    {0x00, 0x22, 0x3F, 0x20}, // 1
    {0x32, 0x29, 0x29, 0x26}, // 2
    {0x11, 0x25, 0x27, 0x19}, // 3
    {0x0C, 0x0A, 0x3F, 0x08}, // 4
    {0x17, 0x25, 0x25, 0x19}, // 5
    {0x1E, 0x25, 0x25, 0x18}, // 6
    {0x01, 0x31, 0x0D, 0x03}, // 7
    {0x1A, 0x25, 0x25, 0x1A}, // 8
    {0x06, 0x29, 0x29, 0x1E}, // 9

    {0x00, 0x00, 0x00, 0x00}, // :
    {0x00, 0x00, 0x00, 0x00}, // ;
    {0x00, 0x00, 0x00, 0x00}, // <
    {0x00, 0x00, 0x00, 0x00}, // =
    {0x00, 0x00, 0x00, 0x00}, // >
    {0x00, 0x00, 0x00, 0x00}, // ?
    {0x00, 0x00, 0x00, 0x00}, // @

    {0x00, 0x00, 0x00, 0x00}, // A
    {0x00, 0x00, 0x00, 0x00}, // B
    {0x00, 0x00, 0x00, 0x00}, // C
    {0x00, 0x00, 0x00, 0x00}, // D
    {0x00, 0x00, 0x00, 0x00}, // E
    {0x00, 0x00, 0x00, 0x00}, // F
    {0x00, 0x00, 0x00, 0x00}, // G
    {0x00, 0x00, 0x00, 0x00}, // H
    {0x00, 0x00, 0x00, 0x00}, // I
    {0x00, 0x00, 0x00, 0x00}, // J
    {0x00, 0x00, 0x00, 0x00}, // K
    {0x00, 0x00, 0x00, 0x00}, // L
    {0x00, 0x00, 0x00, 0x00}, // M
    {0x00, 0x00, 0x00, 0x00}, // N
    {0x00, 0x00, 0x00, 0x00}, // O
    {0x00, 0x00, 0x00, 0x00}, // P
    {0x00, 0x00, 0x00, 0x00}, // Q
    {0x00, 0x00, 0x00, 0x00}, // R
    {0x00, 0x00, 0x00, 0x00}, // S
    {0x00, 0x00, 0x00, 0x00}, // T
    {0x00, 0x00, 0x00, 0x00}, // U
    {0x00, 0x00, 0x00, 0x00}, // V
    {0x00, 0x00, 0x00, 0x00}, // W
    {0x00, 0x00, 0x00, 0x00}, // X
    {0x00, 0x00, 0x00, 0x00}, // Y
    {0x00, 0x00, 0x00, 0x00}, // Z

    {0x00, 0x00, 0x00, 0x00}, // [
    {0x00, 0x00, 0x00, 0x00}, // backslash
    {0x00, 0x00, 0x00, 0x00}, // ]
    {0x00, 0x00, 0x00, 0x00}, // ^
    {0x00, 0x00, 0x00, 0x00}, // _
    {0x00, 0x00, 0x00, 0x00}, // `

    {0x00, 0x00, 0x00, 0x00}, // a
    {0x00, 0x00, 0x00, 0x00}, // b
    {0x00, 0x00, 0x00, 0x00}, // c
    {0x00, 0x00, 0x00, 0x00}, // d
    {0x00, 0x00, 0x00, 0x00}, // e
    {0x00, 0x00, 0x00, 0x00}, // f
    {0x00, 0x00, 0x00, 0x00}, // g
    {0x00, 0x00, 0x00, 0x00}, // h
    {0x00, 0x00, 0x00, 0x00}, // i
    {0x00, 0x00, 0x00, 0x00}, // j
    {0x00, 0x00, 0x00, 0x00}, // k
    {0x00, 0x00, 0x00, 0x00}, // l
    {0x00, 0x00, 0x00, 0x00}, // m
    {0x00, 0x00, 0x00, 0x00}, // n
    {0x00, 0x00, 0x00, 0x00}, // o
    {0x00, 0x00, 0x00, 0x00}, // p
    {0x00, 0x00, 0x00, 0x00}, // q
    {0x00, 0x00, 0x00, 0x00}, // r
    {0x00, 0x00, 0x00, 0x00}, // s
    {0x00, 0x00, 0x00, 0x00}, // t
    {0x00, 0x00, 0x00, 0x00}, // u
    {0x00, 0x00, 0x00, 0x00}, // v
    {0x00, 0x00, 0x00, 0x00}, // w
    {0x00, 0x00, 0x00, 0x00}, // x
    {0x00, 0x00, 0x00, 0x00}, // y
    {0x00, 0x00, 0x00, 0x00}, // z

    {0x00, 0x00, 0x00, 0x00}, // {
    {0x00, 0x00, 0x00, 0x00}, // |
    {0x00, 0x00, 0x00, 0x00}, // }
    {0x00, 0x00, 0x00, 0x00}, // ~

    {0x00, 0x00, 0x00, 0x00}
};

#endif // CFG_FLIP_BROSE