# sudo npm i lv_font_conv -g
lv_font_conv --bpp 1 \
    --size 14 \
    --no-compress \
    --font ./assets/WenQuanYi.Bitmap.Song.12px.ttf \
    --symbols 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,网络初始化正在同步时间系统加载中. \
    --format lvgl \
    -o ./main/src/display/wqy_st_14.c