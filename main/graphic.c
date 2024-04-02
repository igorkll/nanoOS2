#include "main.h"
#include "graphic.h"
#include "drivers/screen.h"
#include "color.h"

// ---------------------------------------------------- graphic parameters

#ifndef graphic_baseRotation
    #define graphic_baseRotation 0
#endif


#ifdef graphic_invertColors
    uint32_t processColor(uint32_t color) {
        return 0xffffff - color;
    }

    uint32_t unprocessColor(uint32_t color) {
        return 0xffffff - color;
    }
#else
    uint32_t processColor(uint32_t color) {
        return color;
    }

    uint32_t unprocessColor(uint32_t color) {
        return color;
    }
#endif


#ifdef graphic_flipX
    int flipX(int x) {
        return screen_x() - x - 1;
    }
#else
    int flipX(int x) {
        return x;
    }
#endif


#ifdef graphic_flipY
    int flipY(int y) {
        return screen_y() - y - 1;
    }
#else
    int flipY(int y) {
        return y;
    }
#endif

// ---------------------------------------------------- base code

uint8_t rotation = graphic_baseRotation;

bool rangeCheck(int x, int y) {
    return x < 0 || y < 0 || x >= screen_x() || y >= screen_y();
}

int rotateX(int x, int y) {
    if (rotation == 0) {
        return x;
    } else if (rotation == 1) {
        return screen_x() - y - 1;
    } else if (rotation == 2) {
        return screen_x() - x - 1;
    } else if (rotation == 3) {
        return y;
    }
    return -1;
}

int rotateY(int x, int y) {
    if (rotation == 0) {
        return y;
    } else if (rotation == 1) {
        return x;
    } else if (rotation == 2) {
        return screen_y() - y - 1;
    } else if (rotation == 3) {
        return screen_y() - x - 1;
    }
    return -1;
}

int processX(int x, int y) {
    return flipX(rotateX(x, y));
}

int processY(int x, int y) {
    return flipY(rotateY(x, y));
}

// ---------------------------------------------------- base api

int graphic_x() {
    if (rotation % 2 == 0) {
        return screen_x();
    } else {
        return screen_y();
    }
}

int graphic_y() {
    if (rotation % 2 == 0) {
        return screen_y();
    } else {
        return screen_x();
    }
}

void graphic_setRotation(uint8_t rotation) {
    rotation = (rotation + graphic_baseRotation) % 4;
}

void graphic_drawPixel(int x, int y, uint32_t color) {
    int px = processX(x, y);
    int py = processY(x, y);
    if (rangeCheck(px, py)) return;
    screen_set(px, py, processColor(color));
}

uint32_t graphic_readPixel(int x, int y) {
    int px = processX(x, y);
    int py = processY(x, y);
    if (rangeCheck(px, py)) return 0;
    return unprocessColor(screen_get(px, py));
}

void graphic_update() {
    screen_update();
}

// ---------------------------------------------------- advanced mathods

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

void graphic_drawRect(int x, int y, int sizeX, int sizeY, uint32_t color) {
    for (int ix = 0; ix < sizeX; ix++) {
        for (int iy = 0; iy < sizeY; iy++) {
            if (ix == 0 || iy == 0 || ix == (sizeX - 1) || iy == (sizeY - 1)) {
                graphic_drawPixel(x + ix, y + iy, color);
            }
        }
    }
}

void graphic_fillRect(int x, int y, int sizeX, int sizeY, uint32_t color) {
    for (int ix = 0; ix < sizeX; ix++) {
        for (int iy = 0; iy < sizeY; iy++) {
            graphic_drawPixel(x + ix, y + iy, color);
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

uint32_t* graphic_dump(int x, int y, int zoneX, int zoneY) {
    uint32_t* dump = malloc((2 + (zoneX * zoneY)) * sizeof(uint32_t));
    dump[0] = zoneX;
    dump[1] = zoneY;
    int index = 2;
    for (int ix = x; ix < (x + zoneX); ix++) {
        for (int iy = y; iy < (y + zoneY); iy++) {
            dump[index] = graphic_readPixel(ix, iy);
            index++;
        }
    }
    return dump;
}

void graphic_drawDump(int x, int y, uint32_t* dump) {
    int zoneX = dump[0];
    int zoneY = dump[1];
    int index = 2;
    for (int ix = x; ix < (x + zoneX); ix++) {
        for (int iy = y; iy < (y + zoneY); iy++) {
            graphic_drawPixel(ix, iy, dump[index]);
            index++;
        }
    }
}

void graphic_copy(int x, int y, int zoneX, int zoneY, int offsetX, int offsetY) {
    uint32_t* dump = graphic_dump(x, y, zoneX, zoneY);
    graphic_drawDump(x + offsetX, y + offsetY, dump);
    free(dump);
}

// ---------------------------------------------------- term

int termX, termY = 0;
int rTermX, rTermY = 0;
int termSizeX, termSizeY = 0;

void _mathTermSize() {
    termSizeX = graphic_x() / (graphic_getFontSizeX() + 1);
    termSizeY = graphic_y() / (graphic_getFontSizeY() + 1);
}

void static _mathRealPos() {
    rTermX = termX * (graphic_getFontSizeX() + 1);
    rTermY = termY * (graphic_getFontSizeY() + 1);
}

void static _newline() {
    termX = 0;
    termY = termY + 1;

    if (termY >= termSizeY) {
        graphic_copy(0, 0, graphic_x(), graphic_y(), 0, -(graphic_getFontSizeY() + 1));
        termY = termSizeY - 1;
    }
}

void static _newchar() {
    termX = termX + 1;

    if (termX >= termSizeX) {
        _newline();
    }
}

void static _print(const char* text, uint32_t color, bool newline) {
    _mathTermSize();

    for (int i = 0; i < strlen(text); i++) {
        char chr = text[i];

        if (chr == '\n') {
            _newline();
        } else {
            _mathRealPos();
            graphic_drawChar(rTermX, rTermY, chr, color);
            _newchar();
        }
    }

    if (newline) {
        _newline();
    }
}


void graphic_setCursor(int x, int y) {
    termX = x;
    termY = y;
}

int graphic_getCursorX() {
    return termX;
}

int graphic_getCursorY() {
    return termY;
}

void graphic_print(const char* text, uint32_t color) {
    _print(text, color, false);
}

void graphic_println(const char* text, uint32_t color) {
    _print(text, color, true);
}