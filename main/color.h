#include <stdint.h>

typedef uint32_t tcolor;
#define color_black   0x000000
#define color_gray    0x4c4c4c
#define color_blue    0x0000ff
#define color_green   0x00ff00
#define color_yellow  0xffff00
#define color_orange  0xff6600
#define color_red     0xff0000
#define color_wgray   0xbdbdbd
#define color_white   0xffffff
#define color_all_colors {color_black, color_gray, color_blue, color_green, color_yellow, color_orange, color_red, color_wgray, color_white}
#define color_all_grays  {color_black, color_gray, color_wgray, color_white}

tcolor color_pack(uint8_t red, uint8_t green, uint8_t blue);
tcolor color_packAlpha(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
uint8_t color_getRed(tcolor);
uint8_t color_getGreen(tcolor);
uint8_t color_getBlue(tcolor);
uint8_t color_getGray(tcolor);
uint8_t color_getAlpha(tcolor);
uint16_t color_to565(tcolor);
tcolor color_from565(uint16_t);
tcolor color_swap(tcolor);
tcolor color_hsv(uint8_t hue, uint8_t saturation, uint8_t value);
tcolor color_random();
tcolor color_randomGray();
tcolor color_randomBlackwhite();
tcolor color_select(tcolor blackwhite, tcolor monochrome, tcolor colored);
tcolor color_mselect(tcolor blackwhite_monochrome, tcolor colored);
tcolor color_wmselect(tcolor colored);
tcolor color_bmselect(tcolor colored);