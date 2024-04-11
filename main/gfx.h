#include "color.h"

void gfx_boxBlur(int x, int y, int sizeX, int sizeY, int boxSize);
void gfx_light(int x, int y, int dx, int dy, int steps, float baseConeX, float baseConeY, float addConeX, float addConeY, float light, float lightDrop, tcolor color);
void gfx_coneBack(int x, int y, int dx, int dy, int steps, float baseConeX, float baseConeY, float addConeX, float addConeY, uint32_t* dump);
void gfx_coneBackLight(int x, int y, int dx, int dy, int steps, float baseConeX, float baseConeY, float addConeX, float addConeY, uint32_t* dump, float light, float lightDrop, tcolor color);
void gfx_fillBack(int x, int y, int sizeX, int sizeY, uint32_t* dump);
void gfx_fillLight(int x, int y, int sizeX, int sizeY, float light, tcolor color);