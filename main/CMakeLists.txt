file(GLOB FILES "*.c")
file(GLOB GAMES "games/*.c")

idf_component_register(SRCS
    ${FILES}
    ${GAMES}
    
    "drivers/keyboard_matrix.c"
    "drivers/screen_nokia5110.c"
    
    INCLUDE_DIRS ".")


# spiffs_create_partition_image(rom   ../filesystem/rom     FLASH_IN_PROJECT) # spifs / rom
# fatfs_create_rawflash_image(rom     ../filesystem/rom     FLASH_IN_PROJECT) # fatfs / rom
fatfs_create_spiflash_image(storage ../filesystem/storage FLASH_IN_PROJECT) # fatfs / storage