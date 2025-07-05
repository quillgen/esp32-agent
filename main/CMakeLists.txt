set(SOURCES "src/led/fast_hsv2rgb_32bit.c"
    "src/led/rgb_led.cpp"
    "src/flash.cpp"
    "src/network.cpp"
    "src/audio/speaker.cpp"
    "src/audio/mic.cpp"
    "src/display/ssd1306_oled.cpp"
    "src/display/ssd1306_i2c.cpp"
    "src/display/wqy_st_14.c"
    "src/display/md_icons_14.c"
    "src/display/inconsolata_14.c"
    "src/display/ui.cpp"
    "src/application.cpp"
    "main.cpp"
)

idf_component_register(SRCS ${SOURCES}
    PRIV_REQUIRES spi_flash
    REQUIRES driver esp_timer esp_lcd nvs_flash esp_event esp_wifi esp_http_client
    INCLUDE_DIRS "include")
