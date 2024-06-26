#include "color.h"
#include "drivers/screen.h"

// preprocessors
tcolor graphic_preprocessor_normal(tcolor);
tcolor graphic_preprocessor_blackwhite(tcolor);
tcolor graphic_preprocessor_monochrome(tcolor);
void graphic_setPreprocessor(tcolor(*)(tcolor));
void graphic_resetPreprocessor();

// crop (scale)
uint8_t graphic_getCropX();
uint8_t graphic_getCropY();
uint8_t graphic_getDefaultCropX();
uint8_t graphic_getDefaultCropY();
void graphic_resetCrop();
void* graphic_saveCrop();
void graphic_restoreCrop(void* ptr);
void graphic_setCrop(uint8_t crop);
void graphic_setCropXY(uint8_t x, uint8_t y);
void graphic_setXCloserTo(uint16_t target);
void graphic_setYCloserTo(uint16_t target);
void graphic_setXYCloserTo(uint16_t targetX, uint16_t targetY);

// canvas
void graphic_setCanvas(uint16_t x, uint16_t y);
bool graphic_isCanvas();
void graphic_resetCanvas();
uint16_t graphic_getCanvasX();
uint16_t graphic_getCanvasY();

// math
int graphic_centerX(int width);
int graphic_centerY(int height);

// base
screen_colormode graphic_getColormode();
bool graphic_isColor();
uint16_t graphic_x();
uint16_t graphic_y();
void graphic_setRotation(uint8_t rotation);
uint8_t graphic_getRotation();
void graphic_drawPixel(int x, int y, tcolor color);
tcolor graphic_readPixel(int x, int y);
void graphic_update();
void graphic_end();

// font
int graphic_getTextSize(const char* text);
uint8_t graphic_getFontSizeX();
uint8_t graphic_getFontSizeY();
FILE* graphic_openFontFile();

// graphic
void graphic_clear(tcolor color);
void graphic_drawChar(int x, int y, char chr, tcolor color);
void graphic_drawText(int x, int y, const char* text, tcolor color);
void graphic_drawInteger(int x, int y, int num, tcolor color);
uint16_t graphic_drawTextBox(int x, int y, int sizeX, int sizeY, const char* text, tcolor color);
void graphic_drawCenterTextLine(int x, int y, int sizeX, const char* text, tcolor color);
uint16_t graphic_drawCenterTextBox(int x, int y, int sizeX, int sizeY, const char* text, tcolor color);
void graphic_fullscreenTextBox(const char* text, tcolor color);
void graphic_drawImage(int x, int y, const char* path);
void graphic_drawRect(int x, int y, int sizeX, int sizeY, tcolor color);
void graphic_fillRect(int x, int y, int sizeX, int sizeY, tcolor color);
void graphic_line(int x0, int y0, int x1, int y1, tcolor color);
void graphic_advancedDraw(int x, int y, uint32_t* sprite, bool xFlip, bool yFlip);
void graphic_draw(int x, int y, uint32_t* sprite);

// filesystem
bool graphic_parseImage(const char* path, bool(*dot)(uint16_t x, uint16_t y, uint16_t sy, uint16_t sx, tcolor, void* param), void* param);
uint32_t* graphic_loadImage(const char* path); //use graphic_draw to draw
int32_t graphic_getImageWidth(const char* path);
int32_t graphic_getImageHeight(const char* path);

// dump
uint32_t graphic_dumpSize(int x, int y, int zoneX, int zoneY);
uint32_t graphic_fullscreenSize();
void graphic_dumpTo(uint32_t* dump, int x, int y, int zoneX, int zoneY);
void graphic_fullscreenDumpTo(uint32_t* dump);
uint32_t* graphic_dump(int x, int y, int zoneX, int zoneY);
uint32_t* graphic_fullscreenDump();
tcolor graphic_dumpGet(uint32_t* dump, uint16_t x, uint16_t y);
void graphic_dumpSet(uint32_t* dump, uint16_t x, uint16_t y, tcolor color);
void graphic_copy(int x, int y, int zoneX, int zoneY, int offsetX, int offsetY);
uint32_t* graphic_resize(uint32_t* dump, uint16_t newSizeX, uint16_t newSizeY);
void graphic_colorChange(uint32_t*, tcolor(*)(uint16_t, uint16_t, tcolor));
void graphic_simpleColorChange(uint32_t*, tcolor(*)(tcolor));
tcolor graphic_dumpPrimaryColor(uint32_t* dump);

// console
void graphic_resetCursor();
void graphic_setCursor(int x, int y);
int graphic_getCursorX();
int graphic_getCursorY();
void graphic_print(const char* text, tcolor color);
void graphic_println(const char* text, tcolor color); //print - newline
void graphic_lnprint(const char* text, tcolor color); //newline - print
void graphic_sprint(const char* text, tcolor color); //newline(except for the first print) - print
#define graphic_printf(color, ...) { char buffer[512]; snprintf(buffer, sizeof(buffer), __VA_ARGS__);  graphic_sprint(buffer, color); }