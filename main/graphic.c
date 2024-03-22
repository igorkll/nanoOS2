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

void graphic_drawChar(int x, int y, char chr, uint32_t color) {
    FILE *file = fopen("/storage/font.bin", "rb");
    if (file == NULL) {
        return;
    }

    fseek(file, chr * 3, SEEK_SET);

    uint8_t charData[3];
    fread(charData, sizeof(uint8_t), 3, file);

    for (int i2 = 0; i2 < 3; i2++) {
        uint8_t charDataPart = charData[i2];
        for (int i3 = 0; i3 < 8; i3++) {
            if (((charDataPart >> i3) & 1) == 1) {
                int lx = x + i3;
                int ly = y + (i2 * 2);
                if (i3 > 3) {
                    lx -= 4;
                    ly++;
                }
                graphic_drawPixel(lx, ly, color);
            }
        }
    }
    
    fclose(file);
}

void graphic_drawText(int x, int y, const char* text, uint32_t color) {
    FILE *file = fopen("/storage/font.bin", "rb");
    if (file != NULL) {
        for (int i = 0; i < strlen(text); i++) {
            uint8_t charByte = text[i];
            fseek(file, charByte * 3, SEEK_SET);

            uint8_t charData[3];
            fread(charData, sizeof(uint8_t), 3, file);

            int cx = x + (i * (graphic_getFontSizeX() + 1));
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
    } else {
        graphic_drawRect(x, y, graphic_getTextSize(text), graphic_getFontSizeY(), color);
    }
}

void graphic_drawTextBox(int x, int y, int sizeX, int sizeY, const char* text, uint32_t color) {
    int fontX = graphic_getFontSizeX();
    int fontY = graphic_getFontSizeY();
    int px = 0;
    int py = 0;
    for (int i = 0; i < strlen(text); i++) {
        char chr = text[i];
        if (chr == '\n') {
            px = 0;
            py = py + 1;
        } else {
            int lx = px * (fontX + 1);
            int ly = py * (fontY + 1);
            if (sizeX > 0 && (lx + fontX) >= sizeX) {
                px = 0;
                py = py + 1;
                lx = px * (fontX + 1);
                ly = py * (fontY + 1);
            }
            if (sizeY > 0 && (ly + fontY) >= sizeY) break;
            graphic_drawChar(x + lx, y + ly, chr, color);
            px = px + 1;
        }
    }
}

void graphic_drawConterTextBox(int x, int y, int sizeX, int sizeY, const char* text, uint32_t color) {
    int fontX = graphic_getFontSizeX();
    int fontY = graphic_getFontSizeY();
    
    int px = 0;
    int py = 0;
    int lines = 1;
    for (int i = 0; i < strlen(text); i++) {
        char chr = text[i];
        if (chr == '\n') {
            px = 0;
            py = py + 1;
            lines = lines + 1;
        } else {
            int lx = px * (fontX + 1);
            int ly = py * (fontY + 1);
            if (sizeX > 0 && (lx + fontX) >= sizeX) {
                px = 0;
                py = py + 1;
                lines = lines + 1;
                lx = px * (fontX + 1);
                ly = py * (fontY + 1);
            }
            if (sizeY > 0 && (ly + fontY) >= sizeY) break;
            px = px + 1;
        }
    }
    
    px = 0;
    py = 0;
    for (int i = 0; i < strlen(text); i++) {
        char chr = text[i];
        if (chr == '\n') {
            px = 0;
            py = py + 1;
        } else {
            int lx = px * (fontX + 1);
            int ly = py * (fontY + 1);
            if (sizeX > 0 && (lx + fontX) >= sizeX) {
                px = 0;
                py = py + 1;
                lx = px * (fontX + 1);
                ly = py * (fontY + 1);
            }
            ly += (sizeY / lines / 2) - ((fontY + 1) / 2);
            if (sizeY > 0 && (ly + fontY) >= sizeY) break;
            graphic_drawChar(x + lx, y + ly, chr, color);
            px = px + 1;
        }
    }
}

void graphic_line(int x0, int y0, int x1, int y1, uint32_t color) {
    int sx, sy, e2, err;
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    sx = (x0 < x1) ? 1 : -1;
    sy = (y0 < y1) ? 1 : -1;
    err = dx - dy;
    while (true) {
        graphic_drawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) return;
        e2 = err<<1;
        if (e2 > -dy) { 
            err = err - dy; 
            x0 = x0 + sx; 
        }
        if (e2 < dx) {
            err = err + dx;
            y0 = y0 + sy;
        }
    }
}

void graphic_drawInteger(int x, int y, int num, uint32_t color) {
    char str[16];
    str[15] = '\0';
    itoa(num, str, 10);
    graphic_drawText(x, y, str, color);
}

// -------------------------- term

int termX, termY;
int rTermX, rTermY;
void static _mathRealPos() {
    rTermX, rTermY = termX * (graphic_getFontSizeX() + 1), termY * (graphic_getFontSizeY() + 1);
}


void graphic_setCursor(int x, int y) {
    termX, termY = x, y;
}

int graphic_getCursorX() {
    return termX;
}

int graphic_getCursorY() {
    return termY;
}

void graphic_print(const char* text, uint32_t color);
void graphic_println(const char* text, uint32_t color);