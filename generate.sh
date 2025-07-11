# sudo npm i lv_font_conv -g
lv_font_conv --bpp 1 \
    --size 14 \
    --no-compress \
    --font ./assets/WenQuanYi.Bitmap.Song.12px.ttf \
    --symbols 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,:网络初始化正在同步时间系统加载中就绪错误. \
    --format lvgl \
    -o ./main/src/display/wqy_st_14.c

lv_font_conv --bpp 1 \
    --size 14 \
    --no-compress \
    --font ./assets/Inconsolata-VariableFont_wdth,wght.ttf \
    --symbols 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz: \
    --format lvgl \
    -o ./main/src/display/inconsolata_14.c

lv_font_conv --bpp 1 \
    --size 16 \
    --no-compress \
    --font ./assets/MaterialSymbolsOutlined[FILL,GRAD,opsz,wght].ttf \
    --range 0xf308,0xf063,0xe1ba,0xe1a7,0xe1a9,0xe002,0xf04e,0xe02b,0xe029 \
    --format lvgl \
    --force-fast-kern-format \
    -o ./main/src/display/md_icons_14.c

lv_font_conv --bpp 4 \
    --size 32 \
    --no-compress \
    --font ./assets/InterVariable.ttf \
    --symbols PSDRN \
    --format lvgl \
    -o ./main/src/display/inter_32.c