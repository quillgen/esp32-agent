/*******************************************************************************
 * Size: 14 px
 * Bpp: 1
 * Opts: --bpp 1 --size 14 --no-compress --font ./assets/WenQuanYi.Bitmap.Song.12px.ttf --symbols 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,网络初始化正在同步时间系统加载中.嘟比快翎智能奎尔科技 --format lvgl -o ./main/src/display/wqy_st_12.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef WQY_ST_12
#define WQY_ST_12 1
#endif

#if WQY_ST_12

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+002C "," */
    0xd8,

    /* U+002E "." */
    0xc0,

    /* U+0030 "0" */
    0x74, 0x63, 0x18, 0xc6, 0x2e,

    /* U+0031 "1" */
    0xe1, 0x8, 0x42, 0x10, 0x9f,

    /* U+0032 "2" */
    0x74, 0x42, 0x11, 0x11, 0x1f,

    /* U+0033 "3" */
    0xf0, 0x42, 0xe0, 0x84, 0x3e,

    /* U+0034 "4" */
    0x18, 0xa4, 0x92, 0x8b, 0xf0, 0x82,

    /* U+0035 "5" */
    0xe4, 0x21, 0xc0, 0x84, 0x3c,

    /* U+0036 "6" */
    0x74, 0x21, 0xe8, 0xc6, 0x2e,

    /* U+0037 "7" */
    0xf8, 0x42, 0x21, 0x10, 0x84,

    /* U+0038 "8" */
    0x74, 0x62, 0xe8, 0xc6, 0x2e,

    /* U+0039 "9" */
    0x74, 0x63, 0x17, 0x86, 0x2e,

    /* U+0041 "A" */
    0x10, 0x20, 0xa1, 0x44, 0x4f, 0xa0, 0xc1,

    /* U+0042 "B" */
    0xfa, 0x18, 0x7e, 0x86, 0x18, 0x7e,

    /* U+0043 "C" */
    0x72, 0x18, 0x20, 0x82, 0x8, 0x5c,

    /* U+0044 "D" */
    0xf9, 0xa, 0xc, 0x18, 0x30, 0x61, 0x7c,

    /* U+0045 "E" */
    0xfc, 0x21, 0xf8, 0x42, 0x1f,

    /* U+0046 "F" */
    0xfc, 0x21, 0xe8, 0x42, 0x10,

    /* U+0047 "G" */
    0x72, 0x18, 0x20, 0x9e, 0x18, 0x5f,

    /* U+0048 "H" */
    0x86, 0x18, 0x7f, 0x86, 0x18, 0x61,

    /* U+0049 "I" */
    0xe9, 0x24, 0x97,

    /* U+004A "J" */
    0x24, 0x92, 0x49, 0x38,

    /* U+004B "K" */
    0x8c, 0xa9, 0x8c, 0x52, 0x51,

    /* U+004C "L" */
    0x84, 0x21, 0x8, 0x42, 0x1f,

    /* U+004D "M" */
    0xc7, 0x8e, 0xad, 0x59, 0x32, 0x60, 0xc1,

    /* U+004E "N" */
    0x87, 0x1a, 0x65, 0x8e, 0x18, 0x61,

    /* U+004F "O" */
    0x38, 0x8a, 0xc, 0x18, 0x30, 0x51, 0x1c,

    /* U+0050 "P" */
    0xf4, 0x63, 0x1f, 0x42, 0x10,

    /* U+0051 "Q" */
    0x38, 0x8a, 0xc, 0x18, 0x30, 0x51, 0x1c, 0x4,

    /* U+0052 "R" */
    0xf2, 0x28, 0xa2, 0xf2, 0x48, 0xa1,

    /* U+0053 "S" */
    0x7c, 0x20, 0xe0, 0x84, 0x3e,

    /* U+0054 "T" */
    0xfe, 0x20, 0x40, 0x81, 0x2, 0x4, 0x8,

    /* U+0055 "U" */
    0x86, 0x18, 0x61, 0x86, 0x18, 0x5c,

    /* U+0056 "V" */
    0x83, 0x5, 0x12, 0x22, 0x85, 0x4, 0x8,

    /* U+0057 "W" */
    0x89, 0x89, 0x89, 0x55, 0x55, 0x55, 0x22, 0x22,

    /* U+0058 "X" */
    0x86, 0x14, 0x8c, 0x49, 0x28, 0x61,

    /* U+0059 "Y" */
    0x82, 0x88, 0xa0, 0x81, 0x2, 0x4, 0x8,

    /* U+005A "Z" */
    0xfe, 0x8, 0x20, 0x82, 0x8, 0x20, 0x7f,

    /* U+0061 "a" */
    0x74, 0x5f, 0x18, 0xbc,

    /* U+0062 "b" */
    0x84, 0x3d, 0x18, 0xc6, 0x3e,

    /* U+0063 "c" */
    0x78, 0x88, 0x87,

    /* U+0064 "d" */
    0x8, 0x5f, 0x18, 0xc6, 0x2f,

    /* U+0065 "e" */
    0x74, 0x7f, 0x8, 0x3c,

    /* U+0066 "f" */
    0x73, 0xc9, 0x24,

    /* U+0067 "g" */
    0x7c, 0x63, 0x18, 0xbc, 0x2e,

    /* U+0068 "h" */
    0x84, 0x3d, 0x18, 0xc6, 0x31,

    /* U+0069 "i" */
    0xbf,

    /* U+006A "j" */
    0x45, 0x55, 0x70,

    /* U+006B "k" */
    0x84, 0x25, 0x4c, 0x52, 0x51,

    /* U+006C "l" */
    0xff,

    /* U+006D "m" */
    0xed, 0x26, 0x4c, 0x99, 0x32, 0x40,

    /* U+006E "n" */
    0xf4, 0x63, 0x18, 0xc4,

    /* U+006F "o" */
    0x74, 0x63, 0x18, 0xb8,

    /* U+0070 "p" */
    0xf4, 0x63, 0x18, 0xfa, 0x10,

    /* U+0071 "q" */
    0x7c, 0x63, 0x18, 0xbc, 0x21,

    /* U+0072 "r" */
    0xf2, 0x49, 0x0,

    /* U+0073 "s" */
    0x74, 0x58, 0x28, 0xb8,

    /* U+0074 "t" */
    0x93, 0xc9, 0x23,

    /* U+0075 "u" */
    0x8c, 0x63, 0x18, 0xbc,

    /* U+0076 "v" */
    0x8c, 0x54, 0xa2, 0x10,

    /* U+0077 "w" */
    0x93, 0x26, 0xad, 0x54, 0x48, 0x80,

    /* U+0078 "x" */
    0x8a, 0x88, 0xa8, 0xc4,

    /* U+0079 "y" */
    0x8c, 0x54, 0xa2, 0x11, 0x8,

    /* U+007A "z" */
    0xf8, 0x44, 0x44, 0x7c,

    /* U+4E2D "中" */
    0x8, 0x7f, 0xe2, 0x31, 0x18, 0x8f, 0xfe, 0x22,
    0x10, 0x8, 0x4, 0x0,

    /* U+521D "初" */
    0x40, 0x3e, 0xf1, 0x14, 0x5, 0x69, 0x6c, 0x52,
    0x94, 0x89, 0x24, 0x4a, 0x20,

    /* U+52A0 "加" */
    0x20, 0x3f, 0x72, 0x54, 0x95, 0x25, 0x49, 0x52,
    0x55, 0x15, 0x55, 0xe2, 0x50,

    /* U+5316 "化" */
    0x12, 0x4, 0x82, 0x29, 0x8c, 0xa2, 0x9, 0x82,
    0xa0, 0x89, 0x22, 0x48, 0x70,

    /* U+540C "同" */
    0xff, 0xc0, 0x6f, 0xb3, 0xd9, 0x2c, 0x96, 0x7b,
    0x25, 0x80, 0xc0, 0xc0,

    /* U+561F "嘟" */
    0x9, 0xff, 0xda, 0xd6, 0xdf, 0xa5, 0x7b, 0x51,
    0xd4, 0xb7, 0xd, 0x3, 0x40,

    /* U+5728 "在" */
    0x8, 0x3f, 0xf1, 0x0, 0x88, 0x62, 0x2b, 0xe2,
    0x20, 0x88, 0x22, 0xf, 0xf0,

    /* U+594E "奎" */
    0x8, 0xf, 0xf8, 0x44, 0x6f, 0xb0, 0x40, 0xff,
    0x81, 0x1, 0xfc, 0x4, 0xf, 0xf8,

    /* U+59CB "始" */
    0x21, 0x3e, 0xa2, 0xa4, 0xbf, 0x48, 0x14, 0xf3,
    0x24, 0xa9, 0x4b, 0xe0, 0x90,

    /* U+5C14 "尔" */
    0x20, 0x2f, 0xe1, 0x40, 0x82, 0x61, 0x29, 0x14,
    0x89, 0x84, 0x8e, 0x0,

    /* U+5FEB "快" */
    0x22, 0xd, 0xea, 0xaa, 0x8a, 0xbf, 0xc8, 0x82,
    0x20, 0x94, 0x28, 0x8c, 0x10,

    /* U+6280 "技" */
    0x22, 0x3f, 0xf2, 0x20, 0xbe, 0x34, 0x99, 0x2a,
    0x30, 0x88, 0x23, 0x3b, 0x30,

    /* U+65F6 "时" */
    0xf0, 0x93, 0xfe, 0x42, 0x4a, 0x4f, 0x29, 0x25,
    0x24, 0x24, 0x84, 0xf0, 0x80, 0x70,

    /* U+667A "智" */
    0x40, 0x3d, 0xff, 0x26, 0x74, 0xcd, 0xf8, 0x84,
    0x7e, 0x21, 0x1f, 0x80,

    /* U+6B63 "正" */
    0xff, 0xc1, 0x2, 0x40, 0x9e, 0x24, 0x9, 0x2,
    0x40, 0x90, 0xff, 0xc0,

    /* U+6B65 "步" */
    0x4, 0x4, 0xf8, 0x90, 0x7f, 0xf0, 0x40, 0x28,
    0x89, 0x22, 0x8, 0x6, 0x1f, 0x0,

    /* U+6BD4 "比" */
    0x84, 0x42, 0xa1, 0x1e, 0xc8, 0x44, 0x22, 0x11,
    0x69, 0xc4, 0xc1, 0xc0,

    /* U+79D1 "科" */
    0x18, 0x98, 0x6f, 0x88, 0x8a, 0x31, 0x9a, 0x3a,
    0x18, 0x9a, 0x20, 0x88, 0x20,

    /* U+7CFB "系" */
    0x3, 0xfe, 0x9f, 0x81, 0x1, 0x17, 0xfc, 0x22,
    0x54, 0x49, 0x4c, 0x40,

    /* U+7EDC "络" */
    0x22, 0x19, 0xe5, 0x73, 0xa4, 0x23, 0x13, 0x3f,
    0x78, 0x12, 0x34, 0xb1, 0xe0,

    /* U+7EDF "统" */
    0x21, 0x5, 0xfd, 0x8, 0x77, 0xf2, 0x2a, 0x85,
    0x3c, 0xa0, 0x15, 0x34, 0xb9, 0x1c,

    /* U+7F51 "网" */
    0xff, 0xc0, 0x75, 0x74, 0x59, 0x2d, 0x6f, 0x17,
    0x11, 0x80, 0xc0, 0xc0,

    /* U+7FCE "翎" */
    0x17, 0xd1, 0x7a, 0x3c, 0x45, 0x7b, 0x43, 0x75,
    0x1c, 0x85, 0x11, 0x44, 0xf0,

    /* U+80FD "能" */
    0x21, 0x9, 0x28, 0xc4, 0xbe, 0xf4, 0x40, 0xfa,
    0x91, 0x43, 0xe9, 0x45, 0x29, 0xbc,

    /* U+8F7D "载" */
    0x11, 0xf, 0xa7, 0xff, 0x90, 0x8f, 0xd2, 0xa2,
    0x9f, 0x20, 0x85, 0xfd, 0x62, 0x44,

    /* U+95F4 "间" */
    0x5f, 0x90, 0x60, 0x33, 0xd9, 0xec, 0x96, 0x4b,
    0x3d, 0x80, 0xc1, 0xc0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 90, .box_w = 2, .box_h = 3, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 1, .adv_w = 75, .box_w = 1, .box_h = 2, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 2, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 7, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 12, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 17, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 22, .adv_w = 105, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 28, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 33, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 38, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 43, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 48, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 53, .adv_w = 119, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 60, .adv_w = 105, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 66, .adv_w = 105, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 72, .adv_w = 119, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 79, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 84, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 89, .adv_w = 105, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 95, .adv_w = 105, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 101, .adv_w = 60, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 104, .adv_w = 60, .box_w = 3, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 108, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 113, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 118, .adv_w = 119, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 125, .adv_w = 105, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 131, .adv_w = 119, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 138, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 143, .adv_w = 119, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 151, .adv_w = 105, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 157, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 162, .adv_w = 119, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 169, .adv_w = 105, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 175, .adv_w = 119, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 182, .adv_w = 149, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 190, .adv_w = 105, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 196, .adv_w = 119, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 203, .adv_w = 119, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 210, .adv_w = 90, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 214, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 219, .adv_w = 75, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 222, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 227, .adv_w = 90, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 231, .adv_w = 60, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 234, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 239, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 244, .adv_w = 30, .box_w = 1, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 245, .adv_w = 45, .box_w = 2, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 248, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 253, .adv_w = 30, .box_w = 1, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 254, .adv_w = 119, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 260, .adv_w = 90, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 264, .adv_w = 90, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 268, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 273, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 278, .adv_w = 60, .box_w = 3, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 90, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 285, .adv_w = 60, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 288, .adv_w = 90, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 292, .adv_w = 90, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 296, .adv_w = 119, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 302, .adv_w = 90, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 306, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 311, .adv_w = 90, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 315, .adv_w = 179, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 327, .adv_w = 179, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 340, .adv_w = 179, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 353, .adv_w = 179, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 366, .adv_w = 179, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 378, .adv_w = 179, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 391, .adv_w = 179, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 404, .adv_w = 179, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 418, .adv_w = 179, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 431, .adv_w = 179, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 443, .adv_w = 179, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 456, .adv_w = 179, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 469, .adv_w = 179, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 483, .adv_w = 179, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 495, .adv_w = 179, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 507, .adv_w = 179, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 521, .adv_w = 179, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 533, .adv_w = 179, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 546, .adv_w = 179, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 558, .adv_w = 179, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 571, .adv_w = 179, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 585, .adv_w = 179, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 597, .adv_w = 179, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 610, .adv_w = 179, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 624, .adv_w = 179, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 638, .adv_w = 179, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = -1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint8_t glyph_id_ofs_list_0[] = {
    0, 0, 1, 0, 2, 3, 4, 5,
    6, 7, 8, 9, 10, 11
};

static const uint16_t unicode_list_3[] = {
    0x0, 0x3f0, 0x473, 0x4e9, 0x5df, 0x7f2, 0x8fb, 0xb21,
    0xb9e, 0xde7, 0x11be, 0x1453, 0x17c9, 0x184d, 0x1d36, 0x1d38,
    0x1da7, 0x2ba4, 0x2ece, 0x30af, 0x30b2, 0x3124, 0x31a1, 0x32d0,
    0x4150, 0x47c7
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 44, .range_length = 14, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = glyph_id_ofs_list_0, .list_length = 14, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL
    },
    {
        .range_start = 65, .range_length = 26, .glyph_id_start = 13,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 97, .range_length = 26, .glyph_id_start = 39,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 20013, .range_length = 18376, .glyph_id_start = 65,
        .unicode_list = unicode_list_3, .glyph_id_ofs_list = NULL, .list_length = 26, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
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
    .cmap_num = 4,
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
const lv_font_t wqy_st_12 = {
#else
lv_font_t wqy_st_12 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 11,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
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



#endif /*#if WQY_ST_12*/

