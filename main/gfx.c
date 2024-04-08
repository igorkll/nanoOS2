#include "main.h"
#include "gfx.h"
#include "graphic.h"
#include "color.h"

void gfx_boxBlur(int x, int y, int sizeX, int sizeY, float boxSize) {
    int dSize = (boxSize * graphic_getCropX()) / 2;
    int realSizeX = sizeX * graphic_getCropX();
    int realSizeY = sizeY * graphic_getCropY();
    uint32_t* img = graphic_rawDump(x, y, realSizeX, realSizeY);
    for (int ix = 0; ix < realSizeX; ix++) {
        for (int iy = 0; iy < realSizeY; iy++) {
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
            for (int ox = -dSize; ox <= dSize; ox++) {
                for (int oy = -dSize; oy <= dSize; oy++) {
                    int px = ix + ox;
                    int py = iy + oy;
                    if (px >= 0 && py >= 0 && px < realSizeX && py < realSizeY) regColor(graphic_dumpGet(img, px, py));
                }
            }
            graphic_rawSet(x + ix, y + iy, color_pack(redSum / count, greenSum / count, blueSum / count));
        }
    }
    free(img);
}