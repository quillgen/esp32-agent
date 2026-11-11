# AGENTS.md

Smart toy car firmware. ESP32-S3 is the main controller (UI, audio, network); an STM32 will drive the 4WD chassis and a Linux host will eventually run the LLM. Only the ESP32 firmware exists in this repo — there is no STM32/host code or inter-board protocol yet.

## Stack / layout

- ESP-IDF 6.1, target `esp32s3`; the CMake project is named `walle`.
- On this machine, activate the toolchain with `source ~/.espressif/tools/activate_idf_v6.1.sh` before running `idf.py`.
- `sdkconfig.defaults` selects `partitions.csv` (factory 2 MiB + SPIFFS 1 MiB) and 8 MB flash; `sdkconfig` itself is gitignored.
- `main/main.cpp` is the only entrypoint: initializes NVS, mounts SPIFFS, then starts `Application::instance()`.
- State machine and tasks live in `main/src/application.cpp`: event-group bits wake `main_task`; `ui_task` owns LVGL.
- Dependencies come from `main/idf_component.yml` via the IDF Component Manager (LVGL 9.5, led_strip 3.1.x for IDF 6 compatibility). `managed_components/` is generated; `dependencies.lock` is tracked.
- IDF 6.1 removed the legacy I2S driver (`driver/i2s.h`); audio uses the `i2s_std` API and requires `esp_driver_i2s` in `main/CMakeLists.txt`.
- Sources are listed explicitly in `main/CMakeLists.txt` — add new `.cpp` files there.
- `doc/design-review.md` contains an approved refactor plan (event bus, services, RAII, DI). Follow it when restructuring `main/` instead of inventing a new architecture.

## Build / flash

```bash
source ~/.espressif/tools/activate_idf_v6.1.sh
idf.py set-target esp32s3   # sdkconfig is gitignored; needed once per checkout
idf.py build
idf.py -p /dev/cu.usbmodem14201 -b 115200 flash monitor
idf.py fullclean
```

- README assumes a `get_idf` command is available (local alias/script); if `idf.py` is missing, source your ESP-IDF `export.sh`.
- `sdkconfig.defaults` is applied automatically when `sdkconfig` is generated; it selects the custom partition table and 8 MB flash. If you change `partitions.csv`, delete `sdkconfig` and re-run `set-target`.
- PSRAM is not enabled by default (`CONFIG_SPIRAM`); enable it in `menuconfig` on boards that have PSRAM.
- SPIFFS is generated and flashed by CMake (`spiffs_create_partition_image(... FLASH_IN_PROJECT)`), so `idf.py flash` includes it. Do not use `write_spiffs_image.sh`: it writes a 0x210000-byte image into the 1 MiB `spiffs` partition.
- There is no real test/lint setup; use a successful `idf.py build` as verification.

## Architecture notes

- LVGL is not thread-safe: only `ui_task` may call LVGL. Send `UiStatus` through `ui_queue`; `OledDisplay::updateUi()` applies it.
- `network.cpp` uses WiFi credentials from Kconfig (`CONFIG_ESP_WIFI_SSID` / `CONFIG_ESP_WIFI_PASSWORD`, set via `idf.py menuconfig`) and Aliyun SNTP with TZ `CST-8`; it retries WiFi 3 times.
- Display is a 128x64 SSD1306 on I2C via esp_lcd + LVGL 9 I1 full-render buffer; a custom flush packs pixels into SSD1306 pages.
- Speaker uses the legacy I2S driver and only plays PCM WAV from `/spiffs`. `Application::start()` calls `speaker_->test()`, which plays hardcoded WAVs on every boot. MP3 / `esp_audio_codec` is not wired in; `doc/prompt-mp3-decoding.txt` contains API notes only.
- Mic only initializes I2S RX; there is no capture/ASR pipeline yet.

## Pins (source of truth: `main/include/device.h`)

- OLED: SDA 5, SCL 4, address 0x3C
- Speaker I2S TX: DOUT 7, BCLK 15, LRC 16 (`I2S_NUM_0`)
- Mic I2S RX: WS 3, BCLK 6, DATA 18 (`I2S_NUM_1`)
- RGB LED: GPIO 48 by default (`CONFIG_BLINK_GPIO`)
- Audio assets live in `spiffs_image/` and appear at `/spiffs`; that partition is 1 MiB. `doc/connection.tex` disagrees with `device.h` on some pins — treat `device.h` as the source of truth.

## Editing conventions

- C++ is in namespace `agent`; format with `.clang-format` (LLVM, C++11 style).
- Generated LVGL fonts in `main/src/display/*.c` are checked in. When UI text changes, add every new glyph/icon codepoint to `generate.sh` and regenerate with `lv_font_conv`, otherwise text renders blank.
- Keep the app within the 2 MiB `factory` partition in `partitions.csv`.
- Do not invent STM32/host interfaces: no UART/CAN/network protocol exists yet. When adding one, put it in a new module and document the wire format (see the `VehicleLink` sketch in `doc/design-review.md`).
