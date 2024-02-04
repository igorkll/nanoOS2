#include <stdint.h>

int graphic_x();
int graphic_y();

void graphic_drawImage(int x, int y, const char* path);
void graphic_drawPixel(int x, int y, uint32_t color);
void graphic_drawRect(int x, int y, int sizeX, int sizeY, uint32_t color);
void graphic_fillRect(int x, int y, int sizeX, int sizeY, uint32_t color);