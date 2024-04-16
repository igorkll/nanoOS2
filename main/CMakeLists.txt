# load profile
if(EXISTS "../profiles/current.txt")
else()
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ../profiles/default.txt ../profiles/current.txt)
endif()

file(READ ../profiles/current.txt profileName)
set(profilePath "../profiles/${profileName}/")
execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ../profiles/_current)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${profilePath} ../profiles/_current)

execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ../temp/filesystem)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ../filesystem ../temp/filesystem)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${profilePath}filesystem ../temp/filesystem)

# load drivers list
file(STRINGS "../profiles/${profileName}/drivers.txt" driversNames)
set(DRIVERS "")
foreach(line IN LISTS driversNames)
    list(APPEND DRIVERS "../drivers/${line}")
endforeach()

# main
file(GLOB_RECURSE FILES "*.c")

idf_component_register(SRCS
    ${FILES}
    ${DRIVERS}
    
    INCLUDE_DIRS ".")

fatfs_create_spiflash_image(storage ../temp/filesystem/storage FLASH_IN_PROJECT)