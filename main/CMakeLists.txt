idf_component_register(SRCS
"color.c"
"fs.c"
"ftp.c"
"functions.c"
"graphic.c"
"lua.c"
"nvs.c"
"wifi.c"

"drivers/keyboard_matrix.c"
"drivers/screen_nokia5110.c"

"main.c"

"../libs/lua/onelua.c"
"../libs/libbmp/libbmp.c" INCLUDE_DIRS ".")


# spiffs_create_partition_image(rom   ../fs/rom     FLASH_IN_PROJECT) # spifs / rom
# fatfs_create_rawflash_image(rom     ../fs/rom     FLASH_IN_PROJECT) # fatfs / rom
fatfs_create_spiflash_image(storage ../filesystem/storage FLASH_IN_PROJECT) # fatfs / storage