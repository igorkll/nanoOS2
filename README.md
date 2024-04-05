# the project is INCOMPLETE
# currently, there is often non-working code in the repository

### minimum system requirements
* RAM    - 200KB
* FLASH  - 4MB

### supported boards
* esp32c3
* esp32

### main project structure
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

### warnings
* after changing the profile, clear the project with the "Full Clean" button
* if you have problems, try deleting the following objects: sdkconfig, build, profiles/_current
* most of the standard profiles are examples of devices with settings for esp32c3. If you have a different board, there is no guarantee that the profile will work on it