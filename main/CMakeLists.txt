# load profile
if(EXISTS "../profiles/current.txt")
else()
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ../data/default.txt ../profiles/current.txt)
endif()

file(READ ../profiles/current.txt profileName)
set(profilePath "../profiles/${profileName}/")
execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ../temp/currentProfile)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${profilePath} ../temp/currentProfile)

if(EXISTS "../profiles/${profileName}/font_name.txt")
    file(READ ../profiles/${profileName}/font_name.txt fontName)
else()
    set(fontName "font4x5.fnt")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ../temp/filesystem)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ../filesystem ../temp/filesystem)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ../data/fonts/${fontName} ../temp/filesystem/storage/font.fnt)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${profilePath}filesystem ../temp/filesystem)

# load drivers list
file(STRINGS "../profiles/${profileName}/drivers.txt" driversNames)
set(DRIVERS "")
foreach(line IN LISTS driversNames)
    if(EXISTS "../profiles/${profileName}/drivers/${line}")
        list(APPEND DRIVERS "../profiles/${profileName}/drivers/${line}")
    else()
        list(APPEND DRIVERS "../drivers/${line}")
    endif()
endforeach()

# main
file(GLOB_RECURSE LIBS "../libs/*/*.c")
file(GLOB_RECURSE FILES "*.c")

idf_component_register(SRCS
    ${LIBS}
    ${DRIVERS}
    ${FILES}
    
    INCLUDE_DIRS ".")

fatfs_create_spiflash_image(storage ../temp/filesystem/storage FLASH_IN_PROJECT)