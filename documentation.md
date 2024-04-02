# the project is INCOMPLETE
# currently, there is often non-working code in the repository

# minimum system requirements
* RAM    - 200KB
* FLASH  - 4MB

# main project structure
* /drivers               - drivers in C
* /drivers/base.h        - basic header for drivers
* /filesystem            - initial images of filesystems
* /filesystem/storage    - changeable filesystem partition
* /profiles              - device-specific settings
* /sdkconfig.defaults    - standard espidf settings
* /partitions.csv        - partitions table

# internal project structure
* /main/apps             - applications
* /main/games            - games
* /main/drivers          - basic driver APIs (only headers here) all drivers must comply with them