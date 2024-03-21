#define color_black   0x000000
#define color_white   0xffffff
#define color_red     0xff0000
#define color_green   0x00ff00
#define color_blue    0x0000ff
#define color_yellow  0xffff00
#define color_orange  0xffAA00

uint32_t color_pack(uint8_t red, uint8_t green, uint8_t blue);
uint8_t color_getRed(uint8_t);
uint8_t color_getGreen(uint8_t);
uint8_t color_getBlue(uint8_t);
uint8_t color_getGray(uint32_t);