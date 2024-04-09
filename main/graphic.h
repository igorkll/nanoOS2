#include "color.h"

// crop (scale)
uint8_t graphic_getCropX();
uint8_t graphic_getCropY();
uint8_t graphic_getDefaultCropX();
uint8_t graphic_getDefaultCropY();
void graphic_resetCrop();
void graphic_setCrop(uint8_t crop);
void graphic_setCropXY(uint8_t x, uint8_t y);
void graphic_setXCloserTo(uint16_t);
void graphic_setYCloserTo(uint16_t);

// raw access (skip crop)
uint16_t graphic_rawX();
uint16_t graphic_rawY();
void graphic_rawSet(int x, int y, tcolor color);
tcolor graphic_rawGet(int x, int y);
uint32_t* graphic_rawDump(int x, int y, int zoneX, int zoneY);
uint32_t* graphic_rawDumpWithCustomCrop(int x, int y, int zoneX, int zoneY, uint8_t customCrop);

// base
uint16_t graphic_x();
uint16_t graphic_y();
void graphic_setRotation(uint8_t rotation);
void graphic_drawPixel(int x, int y, tcolor color);
tcolor graphic_readPixel(int x, int y);
void graphic_update();

// font
int graphic_getTextSize(const char* text);
uint8_t graphic_getFontSizeX();
uint8_t graphic_getFontSizeY();

// graphic
void graphic_clear(tcolor color);
void graphic_drawChar(int x, int y, char chr, tcolor color);
void graphic_drawText(int x, int y, const char* text, tcolor color);
void graphic_drawInteger(int x, int y, int num, tcolor color);
void graphic_drawTextBox(int x, int y, int sizeX, int sizeY, const char* text, tcolor color);
void graphic_drawConterTextBox(int x, int y, int sizeX, int sizeY, const char* text, tcolor color);
void graphic_drawImage(int x, int y, const char* path);
void graphic_drawRect(int x, int y, int sizeX, int sizeY, tcolor color);
void graphic_fillRect(int x, int y, int sizeX, int sizeY, tcolor color);
void graphic_line(int x0, int y0, int x1, int y1, tcolor color);
void graphic_draw(int x, int y, uint32_t* sprite);

// filesystem
uint32_t* graphic_loadImage(const char* path); //use graphic_draw to draw

// dump
uint32_t* graphic_dump(int x, int y, int zoneX, int zoneY);
tcolor graphic_dumpGet(uint32_t* dump, uint16_t x, uint16_t y);
void graphic_dumpSet(uint32_t* dump, uint16_t x, uint16_t y, tcolor color);
void graphic_copy(int x, int y, int zoneX, int zoneY, int offsetX, int offsetY);

// console
void graphic_resetCursor();
void graphic_setCursor(int x, int y);
int graphic_getCursorX();
int graphic_getCursorY();
void graphic_print(const char* text, tcolor color);
void graphic_println(const char* text, tcolor color); //print - newline
void graphic_lnprint(const char* text, tcolor color); //newline - print
void graphic_sprint(const char* text, tcolor color); //newline(except for the first print) - print
#define graphic_printf(color, ...) { char buffer[64]; snprintf(buffer, sizeof(buffer), __VA_ARGS__);  graphic_sprint(buffer, color); }