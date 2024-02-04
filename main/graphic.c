#include "main.h"
#include "graphic.h"
#include "drivers/screen.h"
#include "color.h"


int graphic_x() {
    return screen_x();
}

int graphic_y() {
    return screen_y();
}

void graphic_update() {
    screen_update();
}




int graphic_getFontSizeX() {
    return 4;
}

int graphic_getFontSizeY() {
    return 5;
}

int graphic_getTextSize(const char* text) {
    return strlen(text) * (graphic_getFontSizeX() + 1);
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

void graphic_clear(uint32_t color) {
    graphic_fillRect(0, 0, graphic_x(), graphic_y(), color);
}

void graphic_drawText(int x, int y, const char* text, uint32_t color) {
    FILE *file = fopen("/storage/font.bin", "rb");
    if (file != NULL) {
        for (int i = 0; i < strlen(text); i++) {
            uint8_t charByte = text[i];
            fseek(file, SEEK_SET, charByte * 3);

            uint8_t charData[3];
            fread(charData, sizeof(uint8_t), 3, file);

            int cx = x + (i * graphic_getFontSizeX());
            for (int i2 = 0; i2 < 3; i2++) {
                uint8_t charDataPart = charData[i2];
                for (int i3 = 0; i3 < 8; i3++) {
                    if (((charDataPart >> i3) & 1) == 1) {
                        int lx = cx + i3;
                        int ly = y + (i2 * 2);
                        if (i3 > 3) {
                            lx -= 4;
                            ly++;
                        }
                        graphic_drawPixel(lx, ly, color);
                    }
                }
            }
        }
        fclose(file);
    }
}