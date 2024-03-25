# minimum system requirements
* RAM    - 200KB
* FLASH  - 4MB

# main project structure
* /drivers               - drivers in C
* /filesystem            - initial images of filesystems
* /profiles              - device-specific settings
* /sdkconfig.defaults    - standard espidf settings
* /partitions.csv        - partitions table

# internal project structure
* /main/apps             - applications
* /main/games            - games
* /main/drivers          - basic driver APIs (only headers here) all drivers must comply with them