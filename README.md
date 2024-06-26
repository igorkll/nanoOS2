# the project is INCOMPLETE
# currently, there is often non-working code in the repository
# until I make a release, you should not rely on the project.

## this project is a multifunctional interactive shell for creating portable devices based on esp32 and esp32c3
## nanoOS are just the names of this interactive shell, in fact it is based on FreeRTOS and esp-idf
## the same principle exists for macOS, which is based on unix
## a profile is created for each device, where drivers and their configuration are specified (as well as the configuration of the entire system)
## before starting work, you should download the 'nanoOS2' release and select the profile you are interested in
## you should only select profiles whose name begins with "devboard_" or "device_"
## profiles with other names are templates or created for debugging
## each profile assumes a specific physical configuration of the device and the connected peripherals, and will not work with another device

### minimum system requirements
* RAM    - 200KB
* FLASH  - 4MB

### supported boards
* esp32c3
* esp32

### supported formats
* bmp - image format
* lua - the executable file of the lua machine

### main project structure
* /tools                 - tools
* /data                  - files that project need to build
* /temp                  - temporary files are created during compilation, the folder must exist but the contents can be deleted
* /drivers               - drivers in C
* /filesystem            - initial images of filesystems
* /filesystem/storage    - changeable filesystem partition
* /profiles              - device-specific settings
* /profiles/current.txt  - the currently selected profile
* /profiles/template     - an example of a profile and device settings
* /sdkconfig.defaults    - standard espidf settings
* /partitions.csv        - partitions table

### internal project structure
* /main/apps             - applications
* /main/tests            - applications for testing
* /main/games            - games
* /main/drivers          - basic driver APIs (only headers here) all drivers must comply with them
* /main/drivers/stubs    - here are the default methods for drivers (stubs), they are divided into files to have a low priority due to compiler optimizations in order to avoid "multiple definition" errors

### creating a profile
* if there is no suitable profile for your hardware filling, then you need to create your own
* to do this, first copy the /profiles/template folder
* change the name of the profile directory, the name should start as devboard_ if you are making a debugging board or device_ if it is a finished device
* change the configuration for your device
* after that, delete the "build" folder or full clean up the project
* specify the name of your profile in the /profiles/current.txt file
* assemble the project, flash the device and check the profile operation
* if there is no suitable driver in the list of drivers, then you should write your own using existing ones as an example.
* your own driver should be placed in the drivers directory inside the profile
* i recommend creating your own repository for your profile, with a description of your device and a connection diagram
* then you can publish this repository with a link to 'nanoOS2' so that other people can download your profile and build a project with it

### warnings
* after changing the profile, clear the project with the "Full Clean" button
* if you have problems, try deleting the following objects: sdkconfig, build, temp/currentProfile, temp/filesystem
* most of the standard profiles are examples of devices with settings for esp32c3. If you have a different board, there is no guarantee that the profile will work on it
* the project can only be compiled by the GCC compiler

### other
* unlock GPIO 11 on esp32c3(first, make sure that this can be done on your board. THE PROCEDURE IS IRREVERSIBLE!): espefuse -p COMx burn_efuse VDD_SPI_AS_GPIO 1
* to compile the project, you need esp-idf-5 and it is advisable to use the "vs code" plugin
* if we do not have an ESC button on the device, then the "enter+down" combination will be used instead, or if there is no down button, the "enter+right" combination will be used. this combination does not simulate pressing ESC, it only serves to exit the application without this button.
* in cases where the keyboard shortcut is used instead of ESC. The direction button should be pressed first(or should they be pressed simultaneously)