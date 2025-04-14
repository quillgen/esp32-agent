# sudo npm i lv_font_conv -g
lv_font_conv --bpp 1 \
    --size 14 \
    --no-compress \
    --font ./assets/WenQuanYi.Bitmap.Song.12px.ttf \
    --symbols 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz系统加载中.嘟比快翎智能奎尔科技 \
    --format lvgl \
    -o ./main/src/display/wqy_st_12.c

lv_font_conv --bpp 1 \
    --size 14 \
    --no-compress \
    --font ./assets/CaskaydiaMonoNerdFontMono-Regular.ttf \
    --range 32-126,0xF1EB,0xF240,0xF0F3 \
    --format lvgl \
    --force-fast-kern-format \
    -o ./main/src/display/cascadia_code_14.c

python LVGLImage.py --ofmt C --cf I1 \
    -o ./main/src/display/ \
    ./assets/duby_logo.png