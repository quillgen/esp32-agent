idf_component_register(SRCS "src/fast_hsv2rgb_32bit.c" "main.cpp"
                       PRIV_REQUIRES spi_flash
                       REQUIRES driver
                       INCLUDE_DIRS "include")
