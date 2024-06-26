// keyboard_matrix.c
#define KEYBOARD_X 4
#define KEYBOARD_INPUTS {0, 9, 11, 6}
#define KEYBOARD_Y 4
#define KEYBOARD_OUTPUTS {1, 2, 3, 10}

//leds_simple.c
#define LEDS_PINS {12, 13}

//screen_nokia5110.c
#define SCREEN_VOLTAGE    4 // 0 - 7
#define SCREEN_CONTRASTE  0 // 0 - 3
#define SCREEN_SPI        SPI2_HOST
#define SCREEN_SPI_SPEED  2*1000*1000 //use 10*1000*1000 with the gridient turned on (it won't work on all screens)
#define SCREEN_RST  7
#define SCREEN_DC   8
#define SCREEN_DIN  5
#define SCREEN_CLK  4
//#define SCREEN_GRIDIENT
//#define SCREEN_GRIDIENT_DELAY  2500