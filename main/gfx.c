#include "main.h"
#include "gfx.h"
#include "graphic.h"
#include "color.h"

void gfx_boxBlur(int x, int y, int sizeX, int sizeY) {
    uint32_t* img = graphic_dump(x, y, sizeX, sizeY);
    for (int ix = 0; ix < sizeX; ix++) {
        for (int iy = 0; iy < sizeY; iy++) {
            uint16_t redSum = 0;
            uint16_t greenSum = 0;
            uint16_t blueSum = 0;
            uint8_t count = 0;

            void regColor(tcolor col) {
                redSum = redSum + color_getRed(col);
                greenSum = greenSum + color_getGreen(col);
                blueSum = blueSum + color_getBlue(col);
                count++;
            }

            regColor(graphic_dumpGet(img, ix, iy));
            if (ix > 0 && iy > 0) regColor(graphic_dumpGet(img, ix - 1, iy - 1));
            if (iy > 0) regColor(graphic_dumpGet(img, ix, iy - 1));
            if (ix > 0) regColor(graphic_dumpGet(img, ix - 1, iy));
            if (ix < sizeX) regColor(graphic_dumpGet(img, ix + 1, iy));
            if (iy < sizeY) regColor(graphic_dumpGet(img, ix, iy + 1));
            if (ix < sizeX && iy < sizeY) regColor(graphic_dumpGet(img, ix + 1, iy + 1));
            if (ix < sizeX && iy > 0) regColor(graphic_dumpGet(img, ix + 1, iy - 1));
            if (ix > 0) regColor(graphic_dumpGet(img, ix - 1, iy));
            graphic_drawPixel(x + ix, y + iy, color_pack(redSum / count, greenSum / count, blueSum / count));
        }
    }
    free(img);
}