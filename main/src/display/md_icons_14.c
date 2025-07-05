/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: --bpp 1 --size 16 --no-compress --font ./assets/MaterialSymbolsOutlined[FILL,GRAD,opsz,wght].ttf --range 0xf8ae,0xf8af,0xf8b0,0xf13b,0xf155,0xf063,0xe1ba,0xe1a7,0xe1a9,0xe002 --format lvgl --force-fast-kern-format -o ./main/src/display/md_icons_14.c
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

    /* U+F063 "" */
    0x7, 0xe0, 0x38, 0x1c, 0x60, 0x6, 0xc0, 0x3,
    0x60, 0x6, 0x30, 0x0, 0x18, 0x12, 0xc, 0x1c,
    0x6, 0xc, 0x3, 0xd6, 0x1, 0x80,

    /* U+F13B "" */
    0x7f, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff,
    0xbf, 0xfc,

    /* U+F155 "" */
    0x7f, 0xfa, 0x1, 0xf0, 0xf, 0x80, 0x74, 0x3,
    0xbf, 0xfc,

    /* U+F8AE "" */
    0x7f, 0xfa, 0x0, 0x70, 0x3, 0x80, 0x14, 0x0,
    0xbf, 0xfc,

    /* U+F8AF "" */
    0x7f, 0xfa, 0xf, 0xf0, 0x7f, 0x83, 0xf4, 0x1f,
    0xbf, 0xfc,

    /* U+F8B0 "" */
    0x7f, 0xfa, 0x3f, 0xf1, 0xff, 0x8f, 0xf4, 0x7f,
    0xbf, 0xfc
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 256, .box_w = 14, .box_h = 13, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 23, .adv_w = 256, .box_w = 8, .box_h = 13, .ofs_x = 4, .ofs_y = 1},
    {.bitmap_index = 36, .adv_w = 256, .box_w = 12, .box_h = 13, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 56, .adv_w = 256, .box_w = 16, .box_h = 11, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 78, .adv_w = 256, .box_w = 16, .box_h = 11, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 100, .adv_w = 256, .box_w = 13, .box_h = 6, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 110, .adv_w = 256, .box_w = 13, .box_h = 6, .ofs_x = 2, .ofs_y = 5},
    {.bitmap_index = 120, .adv_w = 256, .box_w = 13, .box_h = 6, .ofs_x = 2, .ofs_y = 5},
    {.bitmap_index = 130, .adv_w = 256, .box_w = 13, .box_h = 6, .ofs_x = 2, .ofs_y = 5},
    {.bitmap_index = 140, .adv_w = 256, .box_w = 13, .box_h = 6, .ofs_x = 2, .ofs_y = 5}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x1a5, 0x1a7, 0x1b8, 0x1061, 0x1139, 0x1153, 0x18ac,
    0x18ad, 0x18ae
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 57346, .range_length = 6319, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 10, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
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

