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

uint32_t color_pack(uint8_t red, uint8_t green, uint8_t blue);
uint8_t color_getRed(uint32_t);
uint8_t color_getGreen(uint32_t);
uint8_t color_getBlue(uint32_t);
uint8_t color_getGray(uint32_t);
uint16_t color_to565(uint32_t);
uint32_t color_from565(uint16_t);
uint32_t color_swap(uint32_t);
uint32_t color_random();
uint32_t color_randomGray();
uint32_t color_randomBlackwhite();
uint32_t color_select(uint32_t blackwhite, uint32_t monochrome, uint32_t colored);
uint32_t color_mselect(uint32_t blackwhite_monochrome, uint32_t colored);
uint32_t color_wmselect(uint32_t colored);
uint32_t color_bmselect(uint32_t colored);