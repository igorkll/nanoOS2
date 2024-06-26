#include "main.h"
#include "graphic.h"
#include "color.h"
#include "filesystem.h"
#include "system.h"
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

#if defined(graphic_force_blackwhite)
    static tcolor (*preProcessColor)(tcolor) = graphic_preprocessor_blackwhite;
#elif defined(graphic_force_monochrome)
    static tcolor (*preProcessColor)(tcolor) = graphic_preprocessor_monochrome;
#else
    static tcolor (*preProcessColor)(tcolor) = graphic_preprocessor_normal;
#endif

#ifdef graphic_invertColors
    static tcolor processColor(tcolor color) {
        return color_invert(preProcessColor(color));
    }

    static tcolor unprocessColor(tcolor color) {
        return 0xffffff - color;
    }
#else
    static tcolor processColor(tcolor color) {
        return preProcessColor(color);
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

// ---------------------------------------------------- preprocessor

tcolor graphic_preprocessor_normal(tcolor color) {
    return color;
}

tcolor graphic_preprocessor_blackwhite(tcolor color) {
    if (color != color_black) return color_white;
    return color;
}

tcolor graphic_preprocessor_monochrome(tcolor color) {
    uint8_t gray = color_getGray(color);
    return color_pack(gray, gray, gray);
}

void graphic_setPreprocessor(tcolor(*preprocessor)(tcolor)) {
    preProcessColor = preprocessor;
}

void graphic_resetPreprocessor() {
    #if defined(graphic_force_blackwhite)
        preProcessColor = graphic_preprocessor_blackwhite;
    #elif defined(graphic_force_monochrome)
        preProcessColor = graphic_preprocessor_monochrome;
    #else
        preProcessColor = graphic_preprocessor_normal;
    #endif
}

// ---------------------------------------------------- base code

static uint16_t canvasX = 0;
static uint16_t canvasY = 0;
static uint8_t rotation = graphic_baseRotation;
static uint8_t cropX = graphic_cropX;
static uint8_t cropY = graphic_cropY;
static tcolor lastClearColor = color_black;

static bool _rangeCheck(int x, int y) {
    return x < 0 || y < 0 || x >= screen_x() || y >= screen_y();
}

static int _rotateX(int x, int y) {
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

static int _rotateY(int x, int y) {
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

static int _processX(int x, int y) {
    return flipX(_rotateX(x, y));
}

static int _processY(int x, int y) {
    return flipY(_rotateY(x, y));
}

static void _dumpTo(uint32_t* dump, int x, int y, int zoneX, int zoneY, tcolor(*__get)(int, int)) {
    dump[0] = zoneX;
    dump[1] = zoneY;
    int index = 2;
    for (int ix = x; ix < (x + zoneX); ix++) {
        for (int iy = y; iy < (y + zoneY); iy++) {
            dump[index] = __get(ix, iy);
            index++;
        }
    }
}

static uint32_t* _dump(int x, int y, int zoneX, int zoneY, tcolor(*__get)(int, int)) {
    uint32_t* dump = malloc(graphic_dumpSize(x, y, zoneX, zoneY));
    if (dump == NULL) return NULL;
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

static TaskHandle_t currentGraphicTask = NULL;
static void _begin() {
    TaskHandle_t requestGraphicTask = xTaskGetCurrentTaskHandle();
    while (currentGraphicTask && requestGraphicTask != currentGraphicTask) yield();
    currentGraphicTask = requestGraphicTask;
}

static uint16_t _rawX() {
    if (rotation % 2 == 0) {
        return screen_x();
    } else {
        return screen_y();
    }
}

static uint16_t _rawY() {
    if (rotation % 2 == 0) {
        return screen_y();
    } else {
        return screen_x();
    }
}

static uint16_t _rawXcrop() {
    return _rawX() / cropX;
}

static uint16_t _rawYcrop() {
    return _rawY() / cropY;
}

static void _canvasPos(int* x, int* y, uint8_t* localCropX, uint8_t* localCropY) {
    int cx = *x;
    int cy = *y;
    *x = rmap(*x, 0, canvasX - 1, 0, _rawX() - 1);
    *y = rmap(*y, 0, canvasY - 1, 0, _rawY() - 1);
    if (localCropX != NULL) {
        *localCropX = rmap(cx + 1, 0, canvasX - 1, 0, _rawX() - 1) - *x;
        *localCropY = rmap(cy + 1, 0, canvasY - 1, 0, _rawY() - 1) - *y;
        if (*localCropX < 1) *localCropX = 1;
        if (*localCropY < 1) *localCropY = 1;
        if (rotation % 2 == 1) {
            uint8_t t = *localCropX;
            *localCropX = *localCropY;
            *localCropY = t;
        }
    }
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

void* graphic_saveCrop() {
    uint8_t* ptr = malloc(2);
    ptr[0] = graphic_getCropX();
    ptr[1] = graphic_getCropY();
    return ptr;
}

void graphic_restoreCrop(void* ptr) {
    uint8_t* _ptr = ptr;
    graphic_setCropXY(_ptr[0], _ptr[1]);
    free(ptr);
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
    cropX = _rawX() / target;
    if (cropX < 1) cropX = 1;
    cropY = cropX;
}

void graphic_setYCloserTo(uint16_t target) {
    cropY = _rawY() / target;
    if (cropY < 1) cropY = 1;
    cropX = cropY;
}

void graphic_setXYCloserTo(uint16_t targetX, uint16_t targetY) {
    if (targetX <= targetY) {
        graphic_setYCloserTo(targetY);
    } else {
        graphic_setXCloserTo(targetX);
    }
}

// ---------------------------------------------------- canvas

void graphic_setCanvas(uint16_t x, uint16_t y) {
    canvasX = x;
    canvasY = y;
}

bool graphic_isCanvas() {
    return canvasX != 0;
}

void graphic_resetCanvas() {
    graphic_setCanvas(0, 0);
}

uint16_t graphic_getCanvasX() {
    return canvasX;
}

uint16_t graphic_getCanvasY() {
    return canvasY;
}

// ---------------------------------------------------- math

int graphic_centerX(int width) {
    return nRound((graphic_x() / 2.0) - (width / 2.0));
}

int graphic_centerY(int height) {
    return nRound((graphic_y() / 2.0) - (height / 2.0));
}

// ---------------------------------------------------- base api

screen_colormode graphic_getColormode() {
    if (preProcessColor == graphic_preprocessor_blackwhite) {
        return screen_blackwhite;
    } else if (preProcessColor == graphic_preprocessor_monochrome) {
        return screen_monochrome;
    }
    return screen_getColormode();
}

bool graphic_isColor(){
    return graphic_getColormode() == screen_colored;
}

uint16_t graphic_x() {
    if (graphic_isCanvas()) return canvasX;
    return _rawXcrop();
}

uint16_t graphic_y() {
    if (graphic_isCanvas()) return canvasY;
    return _rawYcrop();
}

void graphic_drawPixel(int x, int y, tcolor color) {
    uint8_t localCropX = cropX;
    uint8_t localCropY = cropY;
    if (graphic_isCanvas()) {
        _canvasPos(&x, &y, &localCropX, &localCropY);
    } else {
        x = x * cropX;
        y = y * cropY;
    }
    int px = _processX(x, y);
    int py = _processY(x, y);
    if (_rangeCheck(px, py)) return;
    uint8_t alpha = color_getAlpha(color);
    if (alpha == 255) {
        return;
    } else if (alpha > 0) {
        color = color_combine(color_atof(alpha), color, unprocessColor(screen_get(x, y)));
    }
    _begin();
    uint16_t scrX = screen_x();
    uint16_t scrY = screen_y();
    color = processColor(color);
    for (int ix = px; ix < (px + localCropX); ix++) {
        if (ix < scrX) {
            for (int iy = py; iy < (py + localCropY); iy++) {
                if (iy >= scrY) break;
                screen_set(ix, iy, color);
            }
        }
    }
}

tcolor graphic_readPixel(int x, int y) {
    if (graphic_isCanvas()) {
        _canvasPos(&x, &y, NULL, NULL);
    } else {
        x = x * cropX;
        y = y * cropY;
    }
    int px = _processX(x, y);
    int py = _processY(x, y);
    if (_rangeCheck(px, py)) return color_black;
    _begin();
    return unprocessColor(screen_get(px, py));
}

void graphic_setRotation(uint8_t _rotation) {
    rotation = (_rotation + graphic_baseRotation) % 4;
}

uint8_t graphic_getRotation() {
    return (rotation - graphic_baseRotation) % 4;
}

void graphic_update() {
    if (system_debugMode > 0) {
        void* ptr = graphic_saveCrop();
        graphic_setYCloserTo(nRound(graphic_getFontSizeY() * 4.0));
        graphic_drawChar(graphic_x() - 2, 1, '!', color_red);
        graphic_restoreCrop(ptr);
    }

    _begin();
    screen_update();
    graphic_end();
}

void graphic_end() {
    currentGraphicTask = NULL;
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

struct BITMAPV4HEADER_struct {
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
    uint32_t bV4RedMask;
    uint32_t bV4GreenMask;
    uint32_t bV4BlueMask;
    uint32_t bV4AlphaMask;
    uint32_t bV4CSType;
    uint32_t stub1;
    uint32_t stub2;
    uint32_t stub3;
    uint32_t stub4;
    uint32_t stub5;
    uint32_t stub6;
    uint32_t stub7;
    uint32_t stub8;
    uint32_t stub9;
    uint32_t bV4GammaRed;
    uint32_t bV4GammaGreen;
    uint32_t bV4GammaBlue;
};

struct BITMAPV5HEADER_struct {
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
    uint32_t bV4RedMask;
    uint32_t bV4GreenMask;
    uint32_t bV4BlueMask;
    uint32_t bV4AlphaMask;
    uint32_t bV4CSType;
    uint32_t stub1;
    uint32_t stub2;
    uint32_t stub3;
    uint32_t stub4;
    uint32_t stub5;
    uint32_t stub6;
    uint32_t stub7;
    uint32_t stub8;
    uint32_t stub9;
    uint32_t bV4GammaRed;
    uint32_t bV4GammaGreen;
    uint32_t bV4GammaBlue;
    uint32_t bV5Intent;
    uint32_t bV5ProfileData;
    uint32_t bV5ProfileSize;
    uint32_t bV5Reserved;
};

#pragma pack()

bool graphic_parseImage(const char* path, bool(*dot)(uint16_t x, uint16_t y, uint16_t sy, uint16_t sx, tcolor, void* param), void* param) {
    FILE *file = filesystem_open(path, "rb");
    if (file == NULL) return false;

    // check & read header
    struct BITMAPFILEHEADER_struct BITMAPFILEHEADER;
    fread(&BITMAPFILEHEADER, 1, sizeof(BITMAPFILEHEADER), file);
    if (BITMAPFILEHEADER.bfTypeB != 'B' || BITMAPFILEHEADER.bfTypeM != 'M') {
        printf("BMP ERROR: invalid bmp signature: %c%c\n", BITMAPFILEHEADER.bfTypeB, BITMAPFILEHEADER.bfTypeM);
        fclose(file);
        return false;
    }

    // read info
    int32_t width;
    int32_t height;
    uint8_t bits;
    uint32_t bcSize;
    fread(&bcSize, sizeof(uint32_t), 1, file);
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

        case 108 : {
            struct BITMAPV4HEADER_struct BITMAPINFO;
            fread(&BITMAPINFO, 1, sizeof(BITMAPINFO), file);
            width = BITMAPINFO.biWidth;
            height = BITMAPINFO.biHeight;
            bits = BITMAPINFO.biBitCount;
            break;
        }

        case 124 : {
            struct BITMAPV5HEADER_struct BITMAPINFO;
            fread(&BITMAPINFO, 1, sizeof(BITMAPINFO), file);
            width = BITMAPINFO.biWidth;
            height = BITMAPINFO.biHeight;
            bits = BITMAPINFO.biBitCount;
            break;
        }

        default : {
            printf("BMP ERROR: unsupported BITMAPINFO: %li\n", bcSize);
            fclose(file);
            return false;
        }
    }

    if (dot != NULL) {
        bool reverseLines = height < 0;
        height = abs(height);

        // parsing
        fseek(file, BITMAPFILEHEADER.bfOffBits, SEEK_SET);
        for (int iy = reverseLines ? 0 : height - 1; reverseLines ? iy < height : iy >= 0; reverseLines ? iy++ : iy--) {
            for (int ix = 0; ix < width; ix++) {
                uint8_t red = 0;
                uint8_t green = 0;
                uint8_t blue = 0;
                uint8_t alpha = 255;
                fread(&blue, 1, 1, file);
                fread(&green, 1, 1, file);
                fread(&red, 1, 1, file);
                if (bits == 32) {
                    fread(&alpha, 1, 1, file);
                    if (alpha == 0) {
                        red = 0;
                        green = 0;
                        blue = 0;
                    }
                }
                if (dot(ix, iy, width, height, color_packAlpha(red, green, blue, 255 - alpha), param)) {
                    fclose(file);
                    return false;
                }
            }
        }
    }

    fclose(file);
    return true;
}

static bool _imageDot(uint16_t x, uint16_t y, uint16_t width, uint16_t height, tcolor col, void* _image) {
    uint32_t** image = _image;
    if (*image == NULL) {
        *image = malloc((2 + (width * height)) * sizeof(uint32_t));
        if (*image == NULL) return true;
        (*image)[0] = width;
        (*image)[1] = height;
    }
    (*image)[2 + y + (x * height)] = col;
    return false;
}

uint32_t* graphic_loadImage(const char* path) {
    uint32_t* image = NULL;
    if (!graphic_parseImage(path, _imageDot, &image)) {
        if (image != NULL) free(image);
        return NULL;
    }
    return image;
}

int32_t graphic_getImageWidth(const char* path) {
    int32_t var = -1;
    bool _imageParams(uint16_t x, uint16_t y, uint16_t width, uint16_t height, tcolor col, void* ptr) {
        int32_t* var = ptr;
        *var = width;
        return true;
    }
    graphic_parseImage(path, _imageParams, &var);
    return var;
}

int32_t graphic_getImageHeight(const char* path) {
    int32_t var = -1;
    bool _imageParams(uint16_t x, uint16_t y, uint16_t width, uint16_t height, tcolor col, void* ptr) {
        int32_t* var = ptr;
        *var = height;
        return true;
    }
    graphic_parseImage(path, _imageParams, &var);
    return var;
}

// ---------------------------------------------------- font methods

static uint8_t fontWidth;
static uint8_t fontHeight;
static uint8_t charCount;
static uint8_t charBytes;
static bool fontInfoLoaded = false;
static FILE* fontFile = NULL;
static void _loadFontInfo() {
    if (fontInfoLoaded) return;
    if (fontFile == NULL) fclose(fontFile);
    fontFile = graphic_openFontFile();
    if (fontFile == NULL) {
        fontWidth = 0;
        fontWidth = 0;
        charCount = 0;
        charBytes = 0;
        return;
    }
    fread(&fontWidth, 1, 1, fontFile);
    fread(&fontHeight, 1, 1, fontFile);
    fread(&charCount, 1, 1, fontFile);
    charBytes = ceil((fontWidth * fontHeight) / 8.0);
    fontInfoLoaded = true;
}

static void _setCharIndex(uint8_t charIndex) {
    fseek(fontFile, 3 + (charIndex * (charBytes + 1)), SEEK_SET);
}

static bool _checkChar(char chr) {
    char realChr = '\0';
    fread(&realChr, 1, 1, fontFile);
    return realChr == chr;
}

static bool _findCharPos(char chr) {
    for (uint16_t i = 0; i < charCount; i++) {
        _setCharIndex((char)i);
        if (_checkChar(chr)) return true;
    }
    return false;
}

static void _drawChar(int x, int y, char chr, tcolor color) {
    _loadFontInfo();
    if (fontFile != NULL && _findCharPos(chr)) {
        uint8_t charData[charBytes];
        fread(charData, 1, charBytes, fontFile);

        for (int i2 = 0; i2 < charBytes; i2++) {
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
    } else {
        graphic_drawRect(x, y, fontWidth, fontHeight, color);
    }
}

int graphic_getTextSize(const char* text) {
    return (strlen(text) * (graphic_getFontSizeX() + 1)) - 1;
}

uint8_t graphic_getFontSizeX() {
    _loadFontInfo();
    return fontWidth;
}

uint8_t graphic_getFontSizeY() {
    _loadFontInfo();
    return fontHeight;
}

FILE* graphic_openFontFile() {
    return filesystem_open("font.fnt", "rb");
}

// ---------------------------------------------------- graphic methods

static bool _drawImageDot(uint16_t x, uint16_t y, uint16_t width, uint16_t height, tcolor col, void* ptr) {
    uint16_t* offsets = (uint16_t*)ptr;
    graphic_drawPixel(x + offsets[0], y + offsets[1], col);
    return false;
}

void graphic_drawImage(int x, int y, const char* path) {
    uint16_t offsets[2];
    offsets[0] = x;
    offsets[1] = y;
    graphic_parseImage(path, _drawImageDot, offsets);
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
    uint8_t alpha = color_getAlpha(color);
    if (alpha == 0) {
        _begin();
        color = processColor(color);
        for (int ix = 0; ix < screen_x(); ix++) {
            for (int iy = 0; iy < screen_y(); iy++) {
                screen_set(ix, iy, color);
            }
        }
    } else if (alpha < 255) {
        graphic_fillRect(0, 0, graphic_x(), graphic_y(), color);
    }
}

void graphic_drawChar(int x, int y, char chr, tcolor color) {
    _drawChar(x, y, chr, color);
}

void graphic_drawText(int x, int y, const char* text, tcolor color) {
    uint16_t len = strlen(text);
    for (int i = 0; i < len; i++) {
        int cx = x + (i * (graphic_getFontSizeX() + 1));
        _drawChar(cx, y, text[i], color);
    }
}

uint16_t graphic_drawTextBox(int x, int y, int sizeX, int sizeY, const char* text, tcolor color) {
    uint8_t fontX = graphic_getFontSizeX();
    uint8_t fontY = graphic_getFontSizeY();
    uint16_t px = 0;
    uint16_t py = 0;
    uint16_t len = strlen(text);
    for (int i = 0; i < len; i++) {
        char chr = text[i];
        if (chr == '\n') {
            px = 0;
            py = py + 1;
        } else {
            uint16_t lx = px * (fontX + 1);
            uint16_t ly = py * (fontY + 1);
            if (sizeX > 0 && (lx + fontX) >= sizeX) {
                px = 0;
                py = py + 1;
                lx = px * (fontX + 1);
                ly = py * (fontY + 1);
            }
            if (sizeY > 0 && (ly + fontY) >= sizeY) break;
            _drawChar(x + lx, y + ly, chr, color);
            px = px + 1;
        }
    }
    return py;
}

void graphic_drawCenterTextLine(int x, int y, int sizeX, const char* text, tcolor color) {
    graphic_drawText(nRound((x + (sizeX / 2)) - ((strlen(text) * (graphic_getFontSizeX() + 1)) / 2.0)) - 1, y, text, color);
}

uint16_t graphic_drawCenterTextBox(int x, int y, int sizeX, int sizeY, const char* text, tcolor color) {
    uint16_t linesCount = 1;
    uint16_t len = strlen(text);

    for (int i = 0; i < len; i++) {
        char chr = text[i];
        if (chr == '\n') {
            linesCount = linesCount + 1;
        }
    }

    char* lines[linesCount];
    uint16_t currentLine = 0;
    uint16_t currentChar = 0;
    for (int i = 0; i < len; i++) {
        char chr = text[i];
        if (chr == '\n') {
            lines[currentLine] = malloc(currentChar + 1);
            lines[currentLine][currentChar] = '\0';
            currentLine++;
            currentChar = 0;
        } else {
            currentChar++;
        }
    }
    lines[currentLine] = malloc(currentChar + 1);
    lines[currentLine][currentChar] = '\0';

    currentLine = 0;
    currentChar = 0;
    for (int i = 0; i < len; i++) {
        char chr = text[i];
        if (chr == '\n') {
            currentLine++;
            currentChar = 0;
        } else {
            lines[currentLine][currentChar] = chr;
            currentChar++;
        }
    }

    uint8_t aFontY = graphic_getFontSizeY() + 1;
    float zoneCenter = y + (sizeY / 2);
    float startPos = zoneCenter - ((linesCount * aFontY) / 2.0);
    for (int i = 0; i <= currentLine; i++) {
        graphic_drawCenterTextLine(x, nRound(startPos + (i * aFontY)), sizeX, lines[i], color);
        free(lines[i]);
    }

    return currentLine;
}

void graphic_fullscreenTextBox(const char* text, tcolor color) {
    graphic_drawCenterTextBox(0, 0, graphic_x(), graphic_y(), text, color);
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

uint32_t graphic_dumpSize(int x, int y, int zoneX, int zoneY) {
    return (2 + (zoneX * zoneY)) * sizeof(uint32_t);
}

uint32_t graphic_fullscreenSize() {
    return graphic_dumpSize(0, 0, graphic_x(), graphic_y());
}

uint32_t* graphic_dump(int x, int y, int zoneX, int zoneY) {
    return _dump(x, y, zoneX, zoneY, graphic_readPixel);
}

uint32_t* graphic_fullscreenDump() {
    return _dump(0, 0, graphic_x(), graphic_y(), graphic_readPixel);
}

void graphic_dumpTo(uint32_t* dump, int x, int y, int zoneX, int zoneY) {
    _dumpTo(dump, x, y, zoneX, zoneY, graphic_readPixel);
}

void graphic_fullscreenDumpTo(uint32_t* dump) {
    _dumpTo(dump, 0, 0, graphic_x(), graphic_y(), graphic_readPixel);
}

tcolor graphic_dumpGet(uint32_t* dump, uint16_t x, uint16_t y) {
    if (x >= dump[0] || y >= dump[1]) return color_black;
    return dump[(y + (x * dump[1])) + 2];
}

void graphic_dumpSet(uint32_t* dump, uint16_t x, uint16_t y, tcolor color) {
    if (x >= dump[0] || y >= dump[1]) return;
    dump[(y + (x * dump[1])) + 2] = color;
}

void graphic_advancedDraw(int x, int y, uint32_t* sprite, bool xFlip, bool yFlip) {
    uint32_t zoneX = sprite[0];
    uint32_t zoneY = sprite[1];
    for (int ix = 0; ix < zoneX; ix++) {
        for (int iy = 0; iy < zoneY; iy++) {
            tcolor color = sprite[2 + iy + (ix * zoneY)];
            if (color_getAlpha(color) == 0) {
                int cix = ix;
                int ciy = iy;
                if (xFlip) cix = zoneX - ix - 1;
                if (yFlip) ciy = zoneY - iy - 1;
                graphic_drawPixel(x + cix, y + ciy, color);
            }
        }
    }
}

void graphic_draw(int x, int y, uint32_t* sprite) {
    graphic_advancedDraw(x, y, sprite, false, false);
}

void graphic_copy(int x, int y, int zoneX, int zoneY, int offsetX, int offsetY) {
    uint32_t* dump = graphic_dump(x, y, zoneX, zoneY);
    graphic_draw(x + offsetX, y + offsetY, dump);
    free(dump);
}

uint32_t* graphic_resize(uint32_t* dump, uint16_t newSizeX, uint16_t newSizeY) {
    uint16_t oldSizeX = dump[0];
    uint16_t oldSizeY = dump[1];
    uint32_t* newdump = malloc((2 + (newSizeX * newSizeY)) * sizeof(uint32_t));
    newdump[0] = newSizeX;
    newdump[1] = newSizeY;
    for (uint16_t ix = 0; ix < newSizeX; ix++) {
        for (uint16_t iy = 0; iy < newSizeY; iy++) {
            graphic_dumpSet(newdump, ix, iy, graphic_dumpGet(dump, rmap(ix, 0, newSizeX - 1, 0, oldSizeX - 1), rmap(iy, 0, newSizeY - 1, 0, oldSizeY - 1)));
        }
    }
    return newdump;
}

void graphic_colorChange(uint32_t* dump, tcolor(*colorChanger)(uint16_t, uint16_t, tcolor)) {
    for (int ix = 0; ix < dump[0]; ix++) {
        for (int iy = 0; iy < dump[1]; iy++) {
            graphic_dumpSet(dump, ix, iy, colorChanger(ix, iy, graphic_dumpGet(dump, ix, iy)));
        }
    }
}

void graphic_simpleColorChange(uint32_t* dump, tcolor(*colorChanger)(tcolor)) {
    for (int ix = 0; ix < dump[0]; ix++) {
        for (int iy = 0; iy < dump[1]; iy++) {
            graphic_dumpSet(dump, ix, iy, colorChanger(graphic_dumpGet(dump, ix, iy)));
        }
    }
}

#define primaryDepth 128
tcolor graphic_dumpPrimaryColor(uint32_t* dump) {
    uint32_t uses[primaryDepth] = {0};
    uint32_t colors[primaryDepth];
    for (uint16_t ix = 0; ix < dump[0]; ix++) {
        for (uint16_t iy = 0; iy < dump[1]; iy++) {
            tcolor color = graphic_dumpGet(dump, ix, iy);
            if (color_getAlpha(color) < 255) {
                color = color_noAlpha(color);
                uint16_t index = map(color, color_black, color_white, 0, primaryDepth - 1);
                uses[index]++;
                colors[index] = color;
            }
        }
    }
    tcolor primary = color_alpha;
    uint32_t lastUses = 0;
    for (uint32_t i = 0; i < primaryDepth; i++) {
        if (uses[i] > lastUses) {
            primary = colors[i];
            lastUses = uses[i];
        }
    } 
    return primary;
}

// ---------------------------------------------------- term methods

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
            _drawChar(rTermX, rTermY, chr, color);
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