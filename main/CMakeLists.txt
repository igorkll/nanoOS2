# load profile
file(READ ../profiles/current.txt profileName)
set(profilePath "../profiles/${profileName}/")

execute_process (COMMAND ${CMAKE_COMMAND} -E remove_directory ../profiles/_current)
execute_process (COMMAND ${CMAKE_COMMAND} -E copy_directory ${profilePath} ../profiles/_current)

# main
file(GLOB_RECURSE FILES "*.c")

idf_component_register(SRCS
    ${FILES}
    
    "../drivers/keyboard_matrix.c"
    "../drivers/screen_nokia5110.c"
    # "../drivers/screen_air101.c"
    
    INCLUDE_DIRS ".")


# spiffs_create_partition_image(rom   ../filesystem/rom     FLASH_IN_PROJECT) # spifs / rom
# fatfs_create_rawflash_image(rom     ../filesystem/rom     FLASH_IN_PROJECT) # fatfs / rom
fatfs_create_spiflash_image(storage ../filesystem/storage FLASH_IN_PROJECT) # fatfs / storage