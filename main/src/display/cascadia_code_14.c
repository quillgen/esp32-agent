/*******************************************************************************
 * Size: 14 px
 * Bpp: 1
 * Opts: --bpp 1 --size 14 --no-compress --font ./assets/CaskaydiaMonoNerdFontMono-Regular.ttf --range 32-126,0xF1EB,0xF240,0xF0F3 --format lvgl --force-fast-kern-format -o ./main/src/display/cascadia_code_14.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef CASCADIA_CODE_14
#define CASCADIA_CODE_14 1
#endif

#if CASCADIA_CODE_14

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xfc, 0x3c,

    /* U+0022 "\"" */
    0xff, 0xff,

    /* U+0023 "#" */
    0x24, 0x48, 0x97, 0xf6, 0xcd, 0x9b, 0x7f, 0x48,
    0x91, 0x20,

    /* U+0024 "$" */
    0x10, 0x43, 0x3f, 0xd3, 0x4f, 0x1e, 0x1c, 0x71,
    0xe7, 0xf8, 0x41, 0x0,

    /* U+0025 "%" */
    0x61, 0x22, 0x44, 0xb6, 0xc6, 0x1b, 0x69, 0x12,
    0x24, 0x30,

    /* U+0026 "&" */
    0x38, 0x6c, 0x60, 0x60, 0x20, 0x36, 0x76, 0xce,
    0xc4, 0xc4, 0x7e,

    /* U+0027 "'" */
    0xff,

    /* U+0028 "(" */
    0x1b, 0x19, 0x8c, 0x63, 0x18, 0xc6, 0x18, 0xc1,
    0x80,

    /* U+0029 ")" */
    0xe0, 0xc1, 0x83, 0xc, 0x30, 0xc3, 0xc, 0x31,
    0x8c, 0xe0,

    /* U+002A "*" */
    0x30, 0xcd, 0xde, 0x39, 0x34, 0x80,

    /* U+002B "+" */
    0x30, 0x60, 0xc7, 0xf3, 0x6, 0xc, 0x0,

    /* U+002C "," */
    0xfe, 0x80,

    /* U+002D "-" */
    0xfe,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x6, 0x8, 0x30, 0x60, 0x83, 0x4, 0x18, 0x20,
    0x41, 0x82, 0xc, 0x10, 0x0,

    /* U+0030 "0" */
    0x38, 0xdb, 0x1e, 0x3c, 0x7e, 0xfd, 0xe3, 0xc6,
    0xd8, 0xe0,

    /* U+0031 "1" */
    0x73, 0xc3, 0xc, 0x30, 0xc3, 0xc, 0x30, 0xcf,
    0xc0,

    /* U+0032 "2" */
    0x7d, 0x8c, 0x18, 0x30, 0x61, 0xc3, 0xc, 0x30,
    0xc3, 0xf8,

    /* U+0033 "3" */
    0x7b, 0x10, 0x41, 0x18, 0xe0, 0xc1, 0x4, 0x3f,
    0x80,

    /* U+0034 "4" */
    0x6, 0x66, 0x66, 0x66, 0x46, 0x46, 0xc6, 0xff,
    0x6, 0x6, 0x6,

    /* U+0035 "5" */
    0xfd, 0x83, 0x6, 0xf, 0xd9, 0xc1, 0x83, 0xc7,
    0x9d, 0xf0,

    /* U+0036 "6" */
    0x1c, 0x61, 0x86, 0xd, 0xdc, 0xf1, 0xe3, 0xc7,
    0xd9, 0xf0,

    /* U+0037 "7" */
    0xff, 0xc6, 0xc6, 0x6, 0x4, 0xc, 0xc, 0x8,
    0x18, 0x18, 0x10,

    /* U+0038 "8" */
    0x7d, 0x8f, 0x1e, 0x34, 0x47, 0x11, 0x63, 0xc7,
    0x8d, 0xf0,

    /* U+0039 "9" */
    0x3d, 0xdf, 0x1e, 0x3c, 0x79, 0xdd, 0x83, 0xc,
    0x31, 0xc0,

    /* U+003A ":" */
    0xf0, 0xf,

    /* U+003B ";" */
    0xf0, 0x3, 0xea,

    /* U+003C "<" */
    0x2, 0x3d, 0xe6, 0xe, 0x7, 0x83, 0x80,

    /* U+003D "=" */
    0xfe, 0x0, 0x0, 0xf, 0xe0,

    /* U+003E ">" */
    0x81, 0xc0, 0xe0, 0x71, 0xee, 0x30, 0x0,

    /* U+003F "?" */
    0x7b, 0x30, 0xc3, 0x18, 0xe3, 0xc, 0x0, 0xc3,
    0x0,

    /* U+0040 "@" */
    0x3c, 0x8f, 0xc, 0xda, 0x74, 0x68, 0xd1, 0x9f,
    0x81, 0x81, 0xe0,

    /* U+0041 "A" */
    0x18, 0x18, 0x3c, 0x34, 0x24, 0x24, 0x66, 0x7e,
    0x42, 0x43, 0xc3,

    /* U+0042 "B" */
    0xf9, 0x9b, 0x36, 0x6c, 0x9e, 0x37, 0x63, 0xc7,
    0x8f, 0xf0,

    /* U+0043 "C" */
    0x3e, 0x62, 0x60, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0x60, 0x70, 0x3f,

    /* U+0044 "D" */
    0xf9, 0x9b, 0x16, 0x3c, 0x78, 0xf1, 0xe3, 0xc7,
    0x9b, 0xe0,

    /* U+0045 "E" */
    0xff, 0x83, 0x6, 0xc, 0x1f, 0xb0, 0x60, 0xc1,
    0x83, 0xf8,

    /* U+0046 "F" */
    0xff, 0x83, 0x6, 0xc, 0x18, 0x3f, 0x60, 0xc1,
    0x83, 0x0,

    /* U+0047 "G" */
    0x3e, 0xcf, 0x6, 0xc, 0x18, 0x33, 0xe3, 0xc6,
    0xcc, 0xf8,

    /* U+0048 "H" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x7f, 0xf1, 0xe3, 0xc7,
    0x8f, 0x18,

    /* U+0049 "I" */
    0xfc, 0xc3, 0xc, 0x30, 0xc3, 0xc, 0x30, 0xcf,
    0xc0,

    /* U+004A "J" */
    0x1e, 0xc, 0x18, 0x30, 0x60, 0xc1, 0xe3, 0xc7,
    0xd9, 0xe0,

    /* U+004B "K" */
    0xc7, 0x8f, 0x1e, 0x6c, 0xdb, 0x3e, 0x64, 0xcd,
    0x8f, 0x18,

    /* U+004C "L" */
    0xc1, 0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1,
    0x83, 0xf8,

    /* U+004D "M" */
    0xef, 0xdf, 0xbf, 0x7f, 0x7a, 0xf5, 0xe3, 0xc7,
    0x8f, 0x18,

    /* U+004E "N" */
    0xcf, 0xbe, 0xfb, 0xef, 0xbe, 0xf7, 0xdf, 0x7c,
    0xc0,

    /* U+004F "O" */
    0x38, 0xdb, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xc6,
    0xd8, 0xe0,

    /* U+0050 "P" */
    0xf9, 0x9b, 0x1e, 0x3c, 0x78, 0xf3, 0x7c, 0xc1,
    0x83, 0x0,

    /* U+0051 "Q" */
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0x66, 0x7e, 0x3c, 0x18, 0x18, 0xe,

    /* U+0052 "R" */
    0xfd, 0x9f, 0x1e, 0x3c, 0x79, 0xbe, 0x64, 0xcd,
    0x8b, 0x18,

    /* U+0053 "S" */
    0x7b, 0x3c, 0x30, 0xf1, 0xe3, 0xc3, 0xc, 0x3f,
    0x80,

    /* U+0054 "T" */
    0xfe, 0x30, 0x60, 0xc1, 0x83, 0x6, 0xc, 0x18,
    0x30, 0x60,

    /* U+0055 "U" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xc7,
    0x8d, 0xf0,

    /* U+0056 "V" */
    0xc3, 0x43, 0x42, 0x62, 0x66, 0x26, 0x24, 0x34,
    0x3c, 0x1c, 0x18,

    /* U+0057 "W" */
    0xc1, 0xc3, 0xc3, 0xdb, 0x5b, 0x5b, 0x5a, 0x5a,
    0x5a, 0x56, 0x66,

    /* U+0058 "X" */
    0x43, 0x66, 0x24, 0x3c, 0x18, 0x18, 0x18, 0x3c,
    0x24, 0x66, 0x43,

    /* U+0059 "Y" */
    0xc3, 0xc3, 0x46, 0x66, 0x24, 0x3c, 0x18, 0x18,
    0x18, 0x18, 0x18,

    /* U+005A "Z" */
    0xfc, 0x30, 0x86, 0x10, 0xc2, 0x18, 0x43, 0xf,
    0xc0,

    /* U+005B "[" */
    0xfe, 0x31, 0x8c, 0x63, 0x18, 0xc6, 0x31, 0x8f,
    0x80,

    /* U+005C "\\" */
    0xc0, 0x81, 0x81, 0x3, 0x2, 0x4, 0xc, 0x8,
    0x18, 0x10, 0x20, 0x60, 0x40,

    /* U+005D "]" */
    0xf8, 0xc6, 0x31, 0x8c, 0x63, 0x18, 0xc6, 0x3f,
    0x80,

    /* U+005E "^" */
    0x30, 0xc7, 0x92, 0x4c,

    /* U+005F "_" */
    0xfe,

    /* U+0060 "`" */
    0x46, 0x62,

    /* U+0061 "a" */
    0x78, 0x18, 0x33, 0xec, 0xd9, 0xb5, 0x3b,

    /* U+0062 "b" */
    0xc1, 0x83, 0x7, 0xee, 0xf8, 0xf1, 0xe3, 0xc7,
    0x9b, 0xe0,

    /* U+0063 "c" */
    0x3c, 0xcf, 0x6, 0xc, 0x18, 0x18, 0x1f,

    /* U+0064 "d" */
    0x6, 0xc, 0x1b, 0xfe, 0xf8, 0xf1, 0xe3, 0xc7,
    0xdd, 0xd8,

    /* U+0065 "e" */
    0x3c, 0x9b, 0x1c, 0x3f, 0xf8, 0x10, 0x1e,

    /* U+0066 "f" */
    0xe, 0x18, 0x30, 0x30, 0x30, 0xfe, 0x30, 0x30,
    0x30, 0x30, 0x30,

    /* U+0067 "g" */
    0x7f, 0xdf, 0x1e, 0x3c, 0x78, 0xfb, 0xbb, 0x6,
    0xc, 0x33, 0xc0,

    /* U+0068 "h" */
    0xc3, 0xc, 0x3e, 0xcf, 0x3c, 0xf3, 0xcf, 0x3c,
    0xc0,

    /* U+0069 "i" */
    0x30, 0xc0, 0x3c, 0x30, 0xc3, 0xc, 0x30, 0xcf,
    0xc0,

    /* U+006A "j" */
    0x18, 0xc0, 0xf1, 0x8c, 0x63, 0x18, 0xc6, 0x31,
    0xbb, 0x80,

    /* U+006B "k" */
    0xc1, 0x83, 0x6, 0x3c, 0x78, 0xb3, 0x7c, 0xcd,
    0x8b, 0x18,

    /* U+006C "l" */
    0xf0, 0x60, 0xc1, 0x83, 0x6, 0xc, 0x18, 0x30,
    0x60, 0x78,

    /* U+006D "m" */
    0xff, 0xaf, 0x5e, 0xbd, 0x7a, 0xf5, 0xeb,

    /* U+006E "n" */
    0xfb, 0x3c, 0xf3, 0xcf, 0x3c, 0xf3,

    /* U+006F "o" */
    0x38, 0xdb, 0x1e, 0x3c, 0x78, 0xdb, 0x1c,

    /* U+0070 "p" */
    0xdd, 0x9f, 0x1e, 0x3c, 0x78, 0xf3, 0xee, 0xc1,
    0x83, 0x6, 0x0,

    /* U+0071 "q" */
    0x77, 0xdf, 0x1e, 0x3c, 0x78, 0xf9, 0xbf, 0x6,
    0xc, 0x18, 0x30,

    /* U+0072 "r" */
    0xee, 0x73, 0x63, 0x60, 0x60, 0x60, 0x60, 0xf8,

    /* U+0073 "s" */
    0x7f, 0xc, 0x3e, 0x7c, 0x30, 0xfe,

    /* U+0074 "t" */
    0x30, 0x60, 0xc7, 0xf3, 0x6, 0xc, 0x18, 0x30,
    0x60, 0x78,

    /* U+0075 "u" */
    0xcf, 0x3c, 0xf3, 0xcf, 0x3c, 0xdf,

    /* U+0076 "v" */
    0x87, 0x8b, 0x32, 0x64, 0x8f, 0xe, 0x18,

    /* U+0077 "w" */
    0x5b, 0x5b, 0x5a, 0x5a, 0x5a, 0x5a, 0x66, 0x66,

    /* U+0078 "x" */
    0xc5, 0x37, 0x8c, 0x31, 0xe4, 0xf1,

    /* U+0079 "y" */
    0xc7, 0x8d, 0x13, 0x22, 0xc5, 0x8e, 0xc, 0x18,
    0x63, 0x80, 0x0,

    /* U+007A "z" */
    0xfc, 0x31, 0x8c, 0x31, 0x8c, 0x3f,

    /* U+007B "{" */
    0x3b, 0x18, 0x63, 0x1b, 0xe, 0x31, 0x98, 0x63,
    0x80,

    /* U+007C "|" */
    0xff, 0xff, 0xff, 0xfc,

    /* U+007D "}" */
    0xc3, 0x8, 0xc6, 0x30, 0x6c, 0x63, 0xc, 0xee,
    0x0,

    /* U+007E "~" */
    0xe7, 0xbf, 0x38,

    /* U+F0F3 "" */
    0x18, 0x3c, 0x7e, 0x7e, 0x7e, 0x7e, 0xff, 0x0,
    0x18,

    /* U+F1EB "" */
    0x3c, 0xc3, 0x3c, 0x24, 0x0, 0x18,

    /* U+F240 "" */
    0xff, 0xff, 0xff, 0xff
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 131, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 131, .box_w = 2, .box_h = 11, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 131, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 7},
    {.bitmap_index = 6, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 16, .adv_w = 131, .box_w = 6, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 28, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 38, .adv_w = 131, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 49, .adv_w = 131, .box_w = 2, .box_h = 4, .ofs_x = 3, .ofs_y = 7},
    {.bitmap_index = 50, .adv_w = 131, .box_w = 5, .box_h = 13, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 59, .adv_w = 131, .box_w = 6, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 69, .adv_w = 131, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 75, .adv_w = 131, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 82, .adv_w = 131, .box_w = 2, .box_h = 5, .ofs_x = 3, .ofs_y = -4},
    {.bitmap_index = 84, .adv_w = 131, .box_w = 7, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 85, .adv_w = 131, .box_w = 2, .box_h = 2, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 86, .adv_w = 131, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 99, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 109, .adv_w = 131, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 118, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 128, .adv_w = 131, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 137, .adv_w = 131, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 148, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 158, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 168, .adv_w = 131, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 179, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 189, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 199, .adv_w = 131, .box_w = 2, .box_h = 8, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 201, .adv_w = 131, .box_w = 2, .box_h = 12, .ofs_x = 3, .ofs_y = -4},
    {.bitmap_index = 204, .adv_w = 131, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 211, .adv_w = 131, .box_w = 7, .box_h = 5, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 216, .adv_w = 131, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 223, .adv_w = 131, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 232, .adv_w = 131, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 243, .adv_w = 131, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 254, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 264, .adv_w = 131, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 275, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 285, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 295, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 305, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 315, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 325, .adv_w = 131, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 334, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 344, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 354, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 364, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 374, .adv_w = 131, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 383, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 393, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 403, .adv_w = 131, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 418, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 428, .adv_w = 131, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 437, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 447, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 457, .adv_w = 131, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 468, .adv_w = 131, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 479, .adv_w = 131, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 490, .adv_w = 131, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 501, .adv_w = 131, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 510, .adv_w = 131, .box_w = 5, .box_h = 13, .ofs_x = 3, .ofs_y = -1},
    {.bitmap_index = 519, .adv_w = 131, .box_w = 7, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 532, .adv_w = 131, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 541, .adv_w = 131, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 545, .adv_w = 131, .box_w = 7, .box_h = 1, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 546, .adv_w = 131, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 8},
    {.bitmap_index = 548, .adv_w = 131, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 555, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 565, .adv_w = 131, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 572, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 582, .adv_w = 131, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 589, .adv_w = 131, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 600, .adv_w = 131, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 611, .adv_w = 131, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 620, .adv_w = 131, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 629, .adv_w = 131, .box_w = 5, .box_h = 15, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 639, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 649, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 659, .adv_w = 131, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 666, .adv_w = 131, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 672, .adv_w = 131, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 679, .adv_w = 131, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 690, .adv_w = 131, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 701, .adv_w = 131, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 709, .adv_w = 131, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 715, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 725, .adv_w = 131, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 731, .adv_w = 131, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 738, .adv_w = 131, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 746, .adv_w = 131, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 752, .adv_w = 131, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = -4},
    {.bitmap_index = 763, .adv_w = 131, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 769, .adv_w = 131, .box_w = 5, .box_h = 13, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 778, .adv_w = 131, .box_w = 2, .box_h = 15, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 782, .adv_w = 131, .box_w = 5, .box_h = 13, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 791, .adv_w = 131, .box_w = 7, .box_h = 3, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 794, .adv_w = 131, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 803, .adv_w = 131, .box_w = 8, .box_h = 6, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 809, .adv_w = 131, .box_w = 8, .box_h = 4, .ofs_x = 0, .ofs_y = 3}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_1[] = {
    0x0, 0xf8, 0x14d
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 61683, .range_length = 334, .glyph_id_start = 96,
        .unicode_list = unicode_list_1, .glyph_id_ofs_list = NULL, .list_length = 3, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t cascadia_code_14 = {
#else
lv_font_t cascadia_code_14 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 17,          /*The maximum line height required by the font*/
    .base_line = 4,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if CASCADIA_CODE_14*/

