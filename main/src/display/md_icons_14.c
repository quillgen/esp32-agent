/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: --bpp 1 --size 16 --no-compress --font ./assets/MaterialSymbolsOutlined[FILL,GRAD,opsz,wght].ttf --range 0xf308,0xf063,0xe1ba,0xe1a7,0xe1a9,0xe002,0xf04e,0xe02b,0xe029 --format lvgl --force-fast-kern-format -o ./main/src/display/md_icons_14.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef MD_ICONS_14
#define MD_ICONS_14 1
#endif

#if MD_ICONS_14

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+E002 "" */
    0x0, 0x0, 0xc, 0x0, 0x30, 0x1, 0xe0, 0x4,
    0x80, 0x33, 0x1, 0xb6, 0x4, 0xc8, 0x33, 0x30,
    0x80, 0x46, 0x31, 0x90, 0x2, 0xff, 0xfc,

    /* U+E029 "" */
    0xc, 0xb, 0x5, 0x82, 0xc1, 0x60, 0xb2, 0x5b,
    0x19, 0xc1, 0xb1, 0x8f, 0x81, 0x0, 0x80,

    /* U+E02B "" */
    0x83, 0x6, 0x2c, 0x19, 0x60, 0x63, 0x1, 0x98,
    0x6, 0xc0, 0x9a, 0x84, 0x64, 0x31, 0xa0, 0xc6,
    0x3, 0xf8, 0x4, 0x60, 0x21, 0x80, 0x0,

    /* U+E1A7 "" */
    0x8, 0xc, 0xce, 0x6b, 0x3e, 0x1c, 0x8, 0x1c,
    0x3e, 0x6b, 0xce, 0xc, 0x8,

    /* U+E1A9 "" */
    0x82, 0xc, 0x30, 0x63, 0x83, 0x2c, 0x1b, 0x80,
    0xd0, 0x6, 0x0, 0x70, 0xf, 0x81, 0xac, 0x33,
    0xe0, 0x33, 0x2, 0x10,

    /* U+E1BA "" */
    0x7, 0xf0, 0x38, 0x1c, 0x61, 0x86, 0xcf, 0xf3,
    0x7f, 0xfe, 0x3f, 0xfc, 0x1f, 0xf8, 0xf, 0xf0,
    0x7, 0xe0, 0x3, 0xc0, 0x1, 0x80,

    /* U+F04E "" */
    0xf, 0x81, 0x83, 0x18, 0xc, 0x8e, 0x28, 0x88,
    0xc8, 0x26, 0x5d, 0x32, 0xe9, 0x91, 0xca, 0x44,
    0x99, 0xfc, 0x60, 0xc0, 0xf8, 0x0,

    /* U+F063 "" */
    0x7, 0xe0, 0x38, 0x1c, 0x60, 0x6, 0xc0, 0x3,
    0x60, 0x6, 0x30, 0x0, 0x18, 0x12, 0xc, 0x1c,
    0x6, 0xc, 0x3, 0xd6, 0x1, 0x80,

    /* U+F308 "" */
    0xff, 0xf7, 0xf8, 0xbf, 0xc5, 0xfe, 0x3f, 0xf1,
    0xff, 0x8f, 0xfc, 0x5f, 0xfe
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 256, .box_w = 14, .box_h = 13, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 23, .adv_w = 256, .box_w = 9, .box_h = 13, .ofs_x = 4, .ofs_y = 2},
    {.bitmap_index = 38, .adv_w = 256, .box_w = 13, .box_h = 14, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 61, .adv_w = 256, .box_w = 8, .box_h = 13, .ofs_x = 4, .ofs_y = 1},
    {.bitmap_index = 74, .adv_w = 256, .box_w = 12, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 94, .adv_w = 256, .box_w = 16, .box_h = 11, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 116, .adv_w = 256, .box_w = 13, .box_h = 13, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 138, .adv_w = 256, .box_w = 16, .box_h = 11, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 160, .adv_w = 256, .box_w = 13, .box_h = 8, .ofs_x = 1, .ofs_y = 4}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x27, 0x29, 0x1a5, 0x1a7, 0x1b8, 0x104c, 0x1061,
    0x1306
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 57346, .range_length = 4871, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 9, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
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
    .cmap_num = 1,
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
const lv_font_t md_icons_14 = {
#else
lv_font_t md_icons_14 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 14,          /*The maximum line height required by the font*/
    .base_line = -1,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = 0,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if MD_ICONS_14*/

