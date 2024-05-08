#include "main.h"
#include "gfx.h"
#include "graphic.h"
#include "color.h"

void gfx_boxBlurWithDump(int x, int y, int sizeX, int sizeY, int boxSize, uint32_t* img) {
    for (int ix = 0; ix < sizeX; ix++) {
        for (int iy = 0; iy < sizeY; iy++) {
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

            for (int ox = nmax(-boxSize, -ix); ox <= nmin(boxSize, sizeX - ix - 1); ox++) {
                int px = ix + ox;
                for (int oy = nmax(-boxSize, -iy); oy <= nmin(boxSize, sizeY - iy - 1); oy++) {
                    int py = iy + oy;
                    regColor(graphic_dumpGet(img, px, py));
                }
            }
            graphic_drawPixel(x + ix, y + iy, color_pack(redSum / count, greenSum / count, blueSum / count));
        }
    }
}

void gfx_boxBlur(int x, int y, int sizeX, int sizeY, int boxSize) {
    uint32_t* img = graphic_dump(x, y, sizeX, sizeY);
    gfx_boxBlurWithDump(x, y, sizeX, sizeY, boxSize, img);
    free(img);
}

void gfx_light(int x, int y, int dx, int dy, int steps, float baseConeX, float baseConeY, float addConeX, float addConeY, float light, float lightDrop, tcolor color) {
    int rx = graphic_x();
    int ry = graphic_y();

    for (int i = 0; i < steps; i++) {
        int sx = floor(baseConeX);
        int sy = floor(baseConeY);
        for (int ix = x - sx; ix <= x + sx; ix++) {
            for (int iy = y - sy; iy <= y + sy; iy++) {
                graphic_drawPixel(ix, iy, color_combine(light, graphic_readPixel(ix, iy), color));
            }
        }

        x += dx;
        y += dy;

        baseConeX += addConeX;
        baseConeY += addConeY;
        if (baseConeX < 0 || baseConeY < 0) break;

        light -= lightDrop;
        if (light <= 0) break;
    }
}

void gfx_coneBack(int x, int y, int dx, int dy, int steps, float baseConeX, float baseConeY, float addConeX, float addConeY, uint32_t* dump) {
    int rx = graphic_x();
    int ry = graphic_y();

    for (int i = 0; i < steps; i++) {
        int sx = floor(baseConeX);
        int sy = floor(baseConeY);
        for (int ix = x - sx; ix <= x + sx; ix++) {
            for (int iy = y - sy; iy <= y + sy; iy++) {
                graphic_drawPixel(ix, iy, graphic_dumpGet(dump, ix, iy));
            }
        }

        x += dx;
        y += dy;

        baseConeX += addConeX;
        baseConeY += addConeY;
        if (baseConeX < 0 || baseConeY < 0) break;
    }
}

void gfx_coneBackLight(int x, int y, int dx, int dy, int steps, float baseConeX, float baseConeY, float addConeX, float addConeY, uint32_t* dump, float light, float lightDrop, tcolor color) {
    gfx_coneBack(x, y, dx, dy, steps, baseConeX, baseConeY, addConeX, addConeY, dump);
    gfx_light(x, y, dx, dy, steps, baseConeX, baseConeY, addConeX, addConeY, light, lightDrop, color);
}

void gfx_fillBack(int x, int y, int sizeX, int sizeY, uint32_t* dump) {
    for (int ix = x; ix < x + sizeX; ix++) {
        for (int iy = y; iy < y + sizeY; iy++) {
            graphic_drawPixel(ix, iy, graphic_dumpGet(dump, ix, iy));
        }
    }
}