# ESP32S3 Agent

AI Agent client powered by ESP32S3.

## Prerequisites

* ESP32S3 board (PSRAM recommended; enable `CONFIG_SPIRAM` in menuconfig if present)
* ESP-IDF v6.1 configured (`source ~/.espressif/tools/activate_idf_v6.1.sh` on this machine)
* Managed component dependencies are fetched automatically

## Get started

For the VS Code esp-idf extension, you must run "Set Espressif device target" to esp32s3.
On the command line, set the target once per checkout (`sdkconfig` is gitignored;
`sdkconfig.defaults` supplies the custom partition table and 8 MB flash size):

```bash
source ~/.espressif/tools/activate_idf_v6.1.sh

idf.py set-target esp32s3
idf.py build
```

Find ESP32 device:

```bash
ls /dev/cu.*
```

flash and monitor:

```bash
idf.py -p /dev/cu.usbmodem14201 -b 115200 flash
idf.py -p /dev/cu.usbmodem14201 monitor
```

Clean all:

```bash
idf.py fullclean
```

## Configuration

Run `idf.py menuconfig` and open "🤖 AGENT configuration":

* `ESP_WIFI_SSID` / `ESP_WIFI_PASSWORD` — WiFi credentials
* `BLINK_GPIO` — RGB LED GPIO (default 48)
* `AGENT_PLAY_BOOT_SOUND` — play demo WAV sounds from SPIFFS at boot (disabled by default)

## Audio assets

Mp3 source:
https://voicewiki.cn/wiki/%E7%81%B0%E7%86%8A%E5%9D%A6%E5%85%8B%EF%BC%88%E7%BA%A2%E8%89%B2%E8%AD%A6%E6%88%922%EF%BC%89

Generate 16kHz left channel wav file:

```bash
ffmpeg -i Vgramoc.mp3 -ar 16000 -ac 1 -map_channel 0.0.0 Vgramoc.mono.wav
ffmpeg -i Vgramoc.mp3 -ar 16000 -ac 2 -acodec pcm_s16le Vgramoc.wav
```
