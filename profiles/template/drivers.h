/*
// keyboard_matrix.c
#define KEYBOARD_X 4
#define KEYBOARD_INPUTS {0, 9, 11, 6}
#define KEYBOARD_Y 4
#define KEYBOARD_OUTPUTS {1, 2, 3, 10}
*/

/*
//leds_simple.c
#define LEDS_PINS {12, 13}
//#define LEDS_INVERT
*/

/*
//screen_nokia5110.c
#define SCREEN_VOLTAGE    4 // 0 - 7
#define SCREEN_CONTRASTE  0 // 0 - 3
#define SCREEN_SPI        SPI2_HOST
#define SCREEN_SPI_SPEED  2*1000*1000 //use 10*1000*1000 with the gridient turned on (it won't work on all screens)
#define SCREEN_RST  7
#define SCREEN_DC   8
#define SCREEN_DIN  5
#define SCREEN_CLK  4
//#define SCREEN_CS   CHIPSELECT_PIN
//#define SCREEN_BL   BL_PIN
//#define SCREEN_INVERT_BL
//#define SCREEN_GRIDIENT
//#define SCREEN_GRIDIENT_DELAY  2500
//#define SCREEN_BASESPI //tells the driver to use a common SPI (in this case, CS pin is required)
*/

/*
//screen_ST7735.c
#define SCREEN_SPI       SPI2_HOST
#define SCREEN_SPI_SPEED 20000000
#define SCREEN_RESX      160
#define SCREEN_RESY      80
#define SCREEN_OFFSET_X  1
#define SCREEN_OFFSET_Y  26
//#define SCREEN_INVERT_COLORS
#define SCREEN_HARDWARE_INVERT_COLORS
//#define SCREEN_ALT_COLOR_ORDER
//#define SCREEN_HARDWARE_ALT_COLOR_ORDER
//#define SCREEN_LONG_INIT_DELAYS
#define SCREEN_DC   6
#define SCREEN_DIN  3
#define SCREEN_CS   7
#define SCREEN_CLK  2
#define SCREEN_RST  10
#define SCREEN_BL   11
//#define SCREEN_INVERT_BL
//#define SCREEN_BASESPI //tells the driver to use a base SPI (in this case, CS pin is required)
*/

/*
//keyboard_simple.c
#define KEYBOARD_ROTATE  0
//#define KEYBOARD_HIGH_LEVEL
#define KEYBOARD_UP      8
#define KEYBOARD_RIGHT   9
#define KEYBOARD_DOWN    13
#define KEYBOARD_LEFT    5
#define KEYBOARD_ACTIONS {4}
#define KEYBOARD_ENTER   4
//#define KEYBOARD_ESC     RST_PIN
*/