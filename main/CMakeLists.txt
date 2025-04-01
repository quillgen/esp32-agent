set(SOURCES "src/led/fast_hsv2rgb_32bit.c"
    "src/led/rgb_led.cpp"
    "src/application.cpp"
    "main.cpp"
)

idf_component_register(SRCS ${SOURCES}
    PRIV_REQUIRES spi_flash
    REQUIRES driver esp_timer esp_lcd
    INCLUDE_DIRS "include")
