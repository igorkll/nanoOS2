#include "graphic.h"
#include "main.h"
#include "drivers/screen.h"
#include "color.h"


int graphic_x() {
    return screen_x();
}

int graphic_y() {
    return screen_y();
}

void graphic_drawImage(int x, int y, const char* path) {

}

void graphic_drawPixel(int x, int y, uint32_t color) {
    screen_set(x, y, color);
}

void graphic_drawRect(int x, int y, int sizeX, int sizeY, uint32_t color) {
    for (int ix = 0; ix < sizeX; ix++) {
        for (int iy = 0; iy < sizeY; iy++) {
            if (ix == 0 || iy == 0 || ix == (sizeX - 1) || iy == (sizeY - 1)) {
                screen_set(x + ix, y + iy, color);
            }
        }
    }
}

void graphic_fillRect(int x, int y, int sizeX, int sizeY, uint32_t color) {
    for (int ix = 0; ix < sizeX; ix++) {
        for (int iy = 0; iy < sizeY; iy++) {
            screen_set(x + ix, y + iy, color);
        }
    }
}