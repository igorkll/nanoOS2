#include "main.h"
#include "gfx.h"
#include "graphic.h"
#include "color.h"

void gfx_boxBlur(int x, int y, int sizeX, int sizeY, float boxSize) {
    int blurCrop = clamp(nRound((sizeX * sizeY * boxSize * (graphic_getCropX() * 2) * graphic_getCropY()) / 20000), 1, 6);
    int dSize = clamp((boxSize * graphic_getCropX()) / 2 / blurCrop, 3, 5);
    printf("blurCrop %i\n", blurCrop);
    int realSizeX = sizeX * graphic_getCropX();
    int realSizeY = sizeY * graphic_getCropY();
    uint32_t* img = graphic_rawDumpWithCustomCrop(x, y, realSizeX, realSizeY, blurCrop);
    int cropSizeX = ceil(((float)realSizeX) / blurCrop);
    int cropSizeY = ceil(((float)realSizeY) / blurCrop);
    for (int ix = 0; ix < cropSizeX; ix++) {
        for (int iy = 0; iy < cropSizeY; iy++) {
            uint16_t redSum = 0;
            uint16_t greenSum = 0;
            uint16_t blueSum = 0;
            uint8_t count = 0;

            void regColor(tcolor col) {
                redSum   += color_getRed(col);
                greenSum += color_getGreen(col);
                blueSum  += color_getBlue(col);
                count++;
            }

            regColor(graphic_dumpGet(img, ix, iy));
            for (int ox = nmax(-dSize, -ix); ox <= nmin(dSize, cropSizeX - ix - 1); ox++) {
                int px = ix + ox;
                for (int oy = nmax(-dSize, -iy); oy <= nmin(dSize, cropSizeY - iy - 1); oy++) {
                    int py = iy + oy;
                    regColor(graphic_dumpGet(img, px, py));
                }
            }
            for (int ox = 0; ox < blurCrop; ox++) {
                for (int oy = 0; oy < blurCrop; oy++) {
                    graphic_rawSet(x + (ix * blurCrop) + ox, y + (iy * blurCrop) + oy, color_pack(redSum / count, greenSum / count, blueSum / count));
                }
            }
        }
    }
    free(img);
}