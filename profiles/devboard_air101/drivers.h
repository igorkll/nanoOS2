//screen_ST7735.c
#define SCREEN_SPI       SPI2_HOST
#define SCREEN_SPI_SPEED 20000000
#define SCREEN_RESX      160
#define SCREEN_RESY      80
#define SCREEN_OFFSET_X  1
#define SCREEN_OFFSET_Y  2
#define SCREEN_COLORFIX
#define SCREEN_DC   6
#define SCREEN_DIN  3
#define SCREEN_CS   7
#define SCREEN_CLK  2
#define SCREEN_RST  10

//leds_simple.c
#define LEDS_PINS {12, 13}

//keyboard_joystick.c
#define KEYBOARD_UP      8
#define KEYBOARD_RIGHT   9
#define KEYBOARD_DOWN    13
#define KEYBOARD_LEFT    5
#define KEYBOARD_ACTION  4