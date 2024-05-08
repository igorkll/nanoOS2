// ---- graphic
//#define graphic_flipX
//#define graphic_flipY
//#define graphic_invertColors
//#define graphic_force_blackwhite
//#define graphic_force_monochrome
//#define graphic_crop 1
//#define graphic_cropX 1
//#define graphic_cropY 1
//#define graphic_baseRotation 0

// ---- system
//#define SYSTEM_CUSTOMSHELL
//#define SYSTEM_DISABLELOGO

// ---- shell
//#define SHELL_TITLE "nanoOS"

// ---- sdcard
//#define SDCARD_ENABLE
//#define SDCARD_BASESPI //if you raise this flag, the SD card will work through the base SPI. in this case, the SDCARD_SPI, SDCARD_CLK, SDCARD_MOSI and SDCARD_MISO options are not required
//#define SDCARD_SPI  SPI2_HOST
//#define SDCARD_MOSI 12
//#define SDCARD_MISO 0
//#define SDCARD_CLK  1
//#define SDCARD_CS   2  //optional
//#define SDCARD_CD   X  //optional (card detect)
//#define SDCARD_WP   X  //optional (write protect)

// ---- base spi
//the basic SPI allows you to use 1 SPI for multiple peripherals
//all all screen drivers running on SPI support this
//#define BASESPI_SPI  SPI2_HOST
//#define BASESPI_SIZE 160*80*2  //the maximum package size should be calculated based on the screen size
//#define BASESPI_MOSI    X
//#define BASESPI_MISO    X
//#define BASESPI_CLK     X
//#define BASESPI_QUADWP  X
//#define BASESPI_QUADHD  X