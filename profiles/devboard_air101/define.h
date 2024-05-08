//#define graphic_flipX
//#define graphic_flipY
//#define graphic_invertColors
//#define graphic_baseRotation 0
#define graphic_crop 1

#define SDCARD_ENABLE
#define SDCARD_BASESPI
#define SDCARD_CS   1

#define BASESPI_SPI   SPI2_HOST
#define BASESPI_SIZE  160*80*2  //the maximum package size should be calculated based on the screen size
#define BASESPI_MOSI  3 //SCREEN DIN / SDCARD MOSI
#define BASESPI_MISO  0 //SDCARD MISO
#define BASESPI_CLK   2