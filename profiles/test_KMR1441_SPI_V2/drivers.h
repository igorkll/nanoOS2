//screen_ST7735.c
#define SCREEN_SPI       HSPI_HOST
#define SCREEN_SPI_SPEED 32000000 //if your screen doesn't work, try installing 10000000 here
#define SCREEN_RESX      128
#define SCREEN_RESY      128
#define SCREEN_OFFSET_X  32
#define SCREEN_OFFSET_Y  0
#define SCREEN_DC   33
#define SCREEN_DIN  23
#define SCREEN_CLK  18
#define SCREEN_RST  32

//keyboard_simple.c
#define KEYBOARD_ENTER 0
#define KEYBOARD_NO_PULL