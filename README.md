# ESP32S3 Agent

AI Agent client powered by ESP32S3.

## Prerequisites

* ESP32S3 board
* ESP-IDF configured
* ESP-ADF

## Get started
For vscode esp-idf extension, you must run "Set Espressif device target" to esp32s3.

Build the project:
```bash
get_idf

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

Mp3 source:
https://voicewiki.cn/wiki/%E7%81%B0%E7%86%8A%E5%9D%A6%E5%85%8B%EF%BC%88%E7%BA%A2%E8%89%B2%E8%AD%A6%E6%88%922%EF%BC%89

Generate 16kHz left channel wav file:
```bash
ffmpeg -i Vgramoc.mp3 -ar 16000 -ac 1 -map_channel 0.0.0 Vgramoc.mono.wav
ffmpeg -i Vgramoc.mp3 -ar 16000 -ac 2 -acodec pcm_s16le Vgramoc.wav
```