# sudo npm i lv_font_conv -g
lv_font_conv --bpp 1 \
    --size 12 \
    --no-compress \
    --font WenQuanYi.Bitmap.Song.12px.ttf \
    --symbols 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz海内存知己，天涯若比邻！ \
    --format lvgl \
    -o ../main/src/display/wqy_st_12.c