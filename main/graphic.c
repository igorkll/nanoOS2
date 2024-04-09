#include "main.h"
#include "graphic.h"
#include "color.h"
#include "filesystem.h"
#include "drivers/screen.h"

// ---------------------------------------------------- graphic parameters

#ifndef graphic_baseRotation
    #define graphic_baseRotation 0
#endif

#ifndef graphic_crop
    #define graphic_crop 1
#endif

#ifndef graphic_cropX
    #define graphic_cropX graphic_crop
#endif

#ifndef graphic_cropY
    #define graphic_cropY graphic_crop
#endif

#ifdef graphic_invertColors
    static tcolor processColor(tcolor color) {
        return 0xffffff - color;
    }

    static tcolor unprocessColor(tcolor color) {
        return 0xffffff - color;
    }
#else
    static tcolor processColor(tcolor color) {
        return color;
    }

    static tcolor unprocessColor(tcolor color) {
        return color;
    }
#endif


#ifdef graphic_flipX
    static int flipX(int x) {
        return screen_x() - x - 1;
    }
#else
    static int flipX(int x) {
        return x;
    }
#endif


#ifdef graphic_flipY
    static int flipY(int y) {
        return screen_y() - y - 1;
    }
#else
    static int flipY(int y) {
        return y;
    }
#endif

// ---------------------------------------------------- base code

static uint8_t rotation = graphic_baseRotation;
static uint8_t cropX = graphic_cropX;
static uint8_t cropY = graphic_cropY;
static tcolor lastClearColor = color_black;

static bool rangeCheck(int x, int y) {
    return x < 0 || y < 0 || x >= screen_x() || y >= screen_y();
}

static int rotateX(int x, int y) {
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

static int rotateY(int x, int y) {
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

static int processX(int x, int y) {
    return flipX(rotateX(x, y));
}

static int processY(int x, int y) {
    return flipY(rotateY(x, y));
}

static uint32_t* _dump(int x, int y, int zoneX, int zoneY, tcolor(*__get)(int, int)) {
    tcolor* dump = malloc((2 + (zoneX * zoneY)) * sizeof(uint32_t));
    dump[0] = zoneX;
    dump[1] = zoneY;
    int index = 2;
    for (int ix = x; ix < (x + zoneX); ix++) {
        for (int iy = y; iy < (y + zoneY); iy++) {
            dump[index] = __get(ix, iy);
            index++;
        }
    }
    return dump;
}

// ---------------------------------------------------- crop control

uint8_t graphic_getCropX() {
    return cropX;
}

uint8_t graphic_getCropY() {
    return cropY;
}

uint8_t graphic_getDefaultCropX() {
    return graphic_cropX;
}

uint8_t graphic_getDefaultCropY() {
    return graphic_cropY;
}

void graphic_resetCrop() {
    cropX = graphic_cropX;
    cropY = graphic_cropY;
}

void graphic_setCrop(uint8_t crop) {
    cropX = crop;
    cropY = crop;
}

void graphic_setCropXY(uint8_t x, uint8_t y) {
    cropX = x;
    cropY = y;
}

void graphic_setXCloserTo(uint16_t target) {
    cropX = graphic_rawX() / target;
    if (cropX < 1) cropX = 1;
    cropY = cropX;
}

void graphic_setYCloserTo(uint16_t target) {
    cropY = graphic_rawY() / target;
    if (cropY < 1) cropY = 1;
    cropX = cropY;
}

// ---------------------------------------------------- raw access

uint16_t graphic_rawX() {
    if (rotation % 2 == 0) {
        return screen_x();
    } else {
        return screen_y();
    }
}

uint16_t graphic_rawY() {
    if (rotation % 2 == 0) {
        return screen_y();
    } else {
        return screen_x();
    }
}

void graphic_rawSet(int x, int y, tcolor color) {
    int px = processX(x, y);
    int py = processY(x, y);
    if (rangeCheck(px, py)) return;
    screen_set(px, py, processColor(color));
}

tcolor graphic_rawGet(int x, int y) {
    int px = processX(x, y);
    int py = processY(x, y);
    if (rangeCheck(px, py)) return color_black;
    return unprocessColor(screen_get(px, py));
}

uint32_t* graphic_rawDump(int x, int y, int zoneX, int zoneY) {
    return _dump(x, y, zoneX, zoneY, graphic_rawGet);
}

uint32_t* graphic_rawDumpWithCustomCrop(int x, int y, int zoneX, int zoneY, uint8_t customCrop) {
    static uint8_t staticCustomCrop;
    staticCustomCrop = customCrop;
    tcolor __get(int x, int y) {
        x = x * staticCustomCrop;
        y = y * staticCustomCrop;
        int px = processX(x, y);
        int py = processY(x, y);
        if (rangeCheck(px, py)) return color_black;
        return unprocessColor(screen_get(px, py));
    }
    return _dump(x, y, zoneX, zoneY, __get);
}

// ---------------------------------------------------- base api

uint16_t graphic_x() {
    if (rotation % 2 == 0) {
        return screen_x() / cropX;
    } else {
        return screen_y() / cropY;
    }
}

uint16_t graphic_y() {
    if (rotation % 2 == 0) {
        return screen_y() / cropY;
    } else {
        return screen_x() / cropX;
    }
}

void graphic_drawPixel(int x, int y, tcolor color) {
    x = x * cropX;
    y = y * cropY;
    int px = processX(x, y);
    int py = processY(x, y);
    if (rangeCheck(px, py)) return;
    for (int ix = x; ix < (x + cropX); ix++) {
        for (int iy = y; iy < (y + cropY); iy++) {
            screen_set(ix, iy, processColor(color));
        }
    }
}

tcolor graphic_readPixel(int x, int y) {
    x = x * cropX;
    y = y * cropY;
    int px = processX(x, y);
    int py = processY(x, y);
    if (rangeCheck(px, py)) return color_black;
    return unprocessColor(screen_get(px, py));
}



void graphic_setRotation(uint8_t rotation) {
    rotation = (rotation + graphic_baseRotation) % 4;
}

void graphic_update() {
    screen_update();
}

// ---------------------------------------------------- image

#pragma pack(1)

struct BITMAPFILEHEADER_struct {
    char bfTypeB;
    char bfTypeM;
    int32_t bfSize;
    int16_t bfReserved1;
    int16_t bfReserved2;
    int32_t bfOffBits;
};

struct BITMAPCOREHEADER_struct {
    uint16_t bcWidth;
    uint16_t bcHeight;
    uint16_t bcPlanes;
    uint16_t bcBitCount;
};

struct BITMAPINFOHEADER_struct {
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

#pragma pack()

uint32_t* graphic_loadImage(const char* path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) return NULL;

    // check & read header
    struct BITMAPFILEHEADER_struct BITMAPFILEHEADER;
    fread(&BITMAPFILEHEADER, 1, sizeof(BITMAPFILEHEADER), file);
    if (BITMAPFILEHEADER.bfTypeB != 'B' || BITMAPFILEHEADER.bfTypeM != 'M') {
        fclose(file);
        return NULL;
    }

    // read info
    uint32_t bcSize;
    fread(&bcSize, sizeof(uint32_t), 1, file);

    uint16_t width = 0;
    uint16_t height = 0;
    uint8_t bits = 0;
    switch (bcSize) {
        case 12 : {
            struct BITMAPCOREHEADER_struct BITMAPINFO;
            fread(&BITMAPINFO, 1, sizeof(BITMAPINFO), file);
            width = BITMAPINFO.bcWidth;
            height = BITMAPINFO.bcHeight;
            bits = BITMAPINFO.bcBitCount;
            break;
        }

        case 40 : {
            struct BITMAPINFOHEADER_struct BITMAPINFO;
            fread(&BITMAPINFO, 1, sizeof(BITMAPINFO), file);
            width = BITMAPINFO.biWidth;
            height = BITMAPINFO.biHeight;
            bits = BITMAPINFO.biBitCount;
            break;
        }

        default : {
            fclose(file);
            return NULL;
        }
    }

    bool reverseLines = height < 0;
    height = abs(height);

    // parsing
    fseek(file, BITMAPFILEHEADER.bfOffBits - 16, SEEK_SET);
    uint32_t* image = malloc((2 + (width * height)) * sizeof(uint32_t));
    if (image == NULL) return NULL;
    image[0] = width;
    image[1] = height;
    for (int iy = reverseLines ? 0 : height - 1; reverseLines ? iy < height : iy >= 0; reverseLines ? iy++ : iy--) {
        for (int ix = 0; ix < width; ix++) {
            uint8_t red;
            uint8_t green;
            uint8_t blue;
            fread(&red, 1, 1, file);
            fread(&blue, 1, 1, file);
            fread(&green, 1, 1, file);
            image[2 + iy + (ix * height)] = color_pack(red, green, blue);
        }
    }
    fclose(file);
    return image;
}


// ---------------------------------------------------- advanced mathods

uint8_t graphic_getFontSizeX() {
    return 4;
}

uint8_t graphic_getFontSizeY() {
    return 5;
}

int graphic_getTextSize(const char* text) {
    return strlen(text) * (graphic_getFontSizeX() + 1);
}

void graphic_drawImage(int x, int y, const char* path) {
    uint32_t* img = graphic_loadImage(path);
    if (img == NULL) return;
    graphic_draw(x, y, img);
    free(img);
}

void graphic_drawRect(int x, int y, int sizeX, int sizeY, tcolor color) {
    for (int ix = 0; ix < sizeX; ix++) {
        for (int iy = 0; iy < sizeY; iy++) {
            if (ix == 0 || iy == 0 || ix == (sizeX - 1) || iy == (sizeY - 1)) {
                graphic_drawPixel(x + ix, y + iy, color);
            }
        }
    }
}

void graphic_fillRect(int x, int y, int sizeX, int sizeY, tcolor color) {
    for (int ix = 0; ix < sizeX; ix++) {
        for (int iy = 0; iy < sizeY; iy++) {
            graphic_drawPixel(x + ix, y + iy, color);
        }
    }
}

void graphic_clear(tcolor color) {
    lastClearColor = color;
    graphic_fillRect(0, 0, graphic_x(), graphic_y(), color);
}

void graphic_drawChar(int x, int y, char chr, tcolor color) {
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

void graphic_drawText(int x, int y, const char* text, tcolor color) {
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

void graphic_drawTextBox(int x, int y, int sizeX, int sizeY, const char* text, tcolor color) {
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

void graphic_drawConterTextBox(int x, int y, int sizeX, int sizeY, const char* text, tcolor color) {
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

void graphic_line(int x0, int y0, int x1, int y1, tcolor color) {
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

void graphic_drawInteger(int x, int y, int num, tcolor color) {
    char str[16];
    str[15] = '\0';
    itoa(num, str, 10);
    graphic_drawText(x, y, str, color);
}

uint32_t* graphic_dump(int x, int y, int zoneX, int zoneY) {
    return _dump(x, y, zoneX, zoneY, graphic_readPixel);
}

tcolor graphic_dumpGet(uint32_t* dump, uint16_t x, uint16_t y) {
    if (x >= dump[0] || y >= dump[1]) return color_black;
    return dump[(y + (x * dump[1])) + 2];
}

void graphic_dumpSet(uint32_t* dump, uint16_t x, uint16_t y, tcolor color) {
    if (x >= dump[0] || y >= dump[1]) return;
    dump[(y + (x * dump[1])) + 2] = color;
}

void graphic_draw(int x, int y, uint32_t* sprite) {
    int zoneX = sprite[0];
    int zoneY = sprite[1];
    int index = 2;
    for (int ix = x; ix < (x + zoneX); ix++) {
        for (int iy = y; iy < (y + zoneY); iy++) {
            graphic_drawPixel(ix, iy, sprite[index++]);
        }
    }
}

void graphic_copy(int x, int y, int zoneX, int zoneY, int offsetX, int offsetY) {
    uint32_t* dump = graphic_dump(x, y, zoneX, zoneY);
    graphic_draw(x + offsetX, y + offsetY, dump);
    free(dump);
}

// ---------------------------------------------------- term

static int termX, termY = 0;
static int rTermX, rTermY = 0;
static int termSizeX, termSizeY = 0;
static bool printed = false;

static void _mathTermSize() {
    termSizeX = graphic_x() / (graphic_getFontSizeX() + 1);
    termSizeY = graphic_y() / (graphic_getFontSizeY() + 1);
}

static void _mathRealPos() {
    rTermX = termX * (graphic_getFontSizeX() + 1);
    rTermY = termY * (graphic_getFontSizeY() + 1);
}

static void _newline() {
    termX = 0;
    termY = termY + 1;

    if (termY >= termSizeY) {
        graphic_copy(0, 0, graphic_x(), graphic_y(), 0, -(graphic_getFontSizeY() + 1));
        graphic_fillRect(0, graphic_y() - graphic_getFontSizeY() - 1, graphic_x(), graphic_getFontSizeY() + 1, lastClearColor);
        termY = termSizeY - 1;
    }
}

static void _newchar() {
    termX = termX + 1;

    if (termX >= termSizeX) {
        _newline();
    }
}

static void _print(const char* text, tcolor color, int newline) {
    _mathTermSize();

    if (newline == 2 || (newline == 3 && printed)) _newline();
    printed = true;
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
    if (newline == 1) _newline();
}

void graphic_resetCursor() {
    termX = 0;
    termY = 0;
    printed = false;
}

void graphic_setCursor(int x, int y) {
    termX = x;
    termY = y;
    printed = false;
}

int graphic_getCursorX() {
    return termX;
}

int graphic_getCursorY() {
    return termY;
}

void graphic_print(const char* text, tcolor color) {
    _print(text, color, 0);
}

void graphic_println(const char* text, tcolor color) {
    _print(text, color, 1);
}

void graphic_lnprint(const char* text, tcolor color) {
    _print(text, color, 2);
}

void graphic_sprint(const char* text, tcolor color) {
    _print(text, color, 3);
}