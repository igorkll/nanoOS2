file(GLOB FILES "*.c")
file(GLOB DRIVERS "drivers/*.c")

idf_component_register(SRCS
    ${FILES}
    ${DRIVERS}
    
    "../libs/lua/onelua.c"
    "../libs/libbmp/libbmp.c"
    
    INCLUDE_DIRS ".")


# spiffs_create_partition_image(rom   ../filesystem/rom     FLASH_IN_PROJECT) # spifs / rom
# fatfs_create_rawflash_image(rom     ../filesystem/rom     FLASH_IN_PROJECT) # fatfs / rom
fatfs_create_spiflash_image(storage ../filesystem/storage FLASH_IN_PROJECT) # fatfs / storage