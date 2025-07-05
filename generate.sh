# sudo npm i lv_font_conv -g
lv_font_conv --bpp 1 \
    --size 14 \
    --no-compress \
    --font ./assets/WenQuanYi.Bitmap.Song.12px.ttf \
    --symbols 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,网络初始化正在同步时间系统加载中. \
    --format lvgl \
    -o ./main/src/display/wqy_st_14.c

lv_font_conv --bpp 1 \
    --size 16 \
    --no-compress \
    --font ./assets/MaterialSymbolsOutlined[FILL,GRAD,opsz,wght].ttf \
    --range 0xf8ae,0xf8af,0xf8b0,0xf13b,0xf155,0xf063,0xe1ba,0xe1a7,0xe1a9,0xe002 \
    --format lvgl \
    --force-fast-kern-format \
    -o ./main/src/display/md_icons_14.c