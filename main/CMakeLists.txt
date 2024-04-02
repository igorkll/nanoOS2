# load profile
file(READ ../profiles/current.txt profileName)
set(profilePath "../profiles/${profileName}/")
execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ../profiles/_current)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${profilePath} ../profiles/_current)

# load drivers list
set(driversListPath "../profiles/_current/drivers.txt")
file(READ ${input_file} lines LINE_BY_LINE)
set(prepend_string "../drivers/")
set(DRIVERS "")
foreach(line IN LISTS lines)
    list(APPEND DRIVERS "${prepend_string}${line}\n")
endforeach()

# main
file(GLOB_RECURSE FILES "*.c")

idf_component_register(SRCS
    ${FILES}
    ${DRIVERS}
    
    "../drivers/keyboard_matrix.c"
    "../drivers/screen_nokia5110.c"
    "../drivers/leds_luat_esp32c3.c"
    # "../drivers/screen_air101.c"
    
    INCLUDE_DIRS ".")


# spiffs_create_partition_image(rom   ../filesystem/rom     FLASH_IN_PROJECT) # spifs / rom
# fatfs_create_rawflash_image(rom     ../filesystem/rom     FLASH_IN_PROJECT) # fatfs / rom
fatfs_create_spiflash_image(storage ../filesystem/storage FLASH_IN_PROJECT) # fatfs / storage