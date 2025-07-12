# or use idf.py partition-table
# parttool.py --partition-table-file partitions.csv get_partition_info --partition-name storage --info offset

mkspiffs -c spiffs_image -b 4096 -p 256 -s 0x210000 build/storage.bin
esptool.py --chip esp32s3 \
    --port /dev/cu.usbmodem14201 \
    --baud 460800 \
    write_flash 0x210000 build/storage.bin