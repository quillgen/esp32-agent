# sudo npm i lv_font_conv -g
lv_font_conv --bpp 1 \
    --size 12 \
    --no-compress \
    --font WenQuanYi.Bitmap.Song.12px.ttf \
    --symbols 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz人类的悲欢并不相通，我只是觉得他们吵闹。——鲁迅\
    --format lvgl \
    -o ../main/src/display/wqy_st_12.c

lv_font_conv --bpp 1 \
    --size 12 \
    --no-compress \
    --font CaskaydiaMonoNerdFontMono-Regular.ttf \
    --range 32-126,0xF1EB,0xF240,0xF0F3 \
    --format lvgl \
    --force-fast-kern-format \         # Improves monospace consistency
    -o ../main/src/display/cascadia_code_14.c