# ESP32S3 Agent

AI Agent client powered by ESP32S3.

## Prerequisites

* ESP32S3 board
* ESP-IDF configured

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