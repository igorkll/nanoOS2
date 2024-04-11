#include "main.h"
#include "color.h"
#include "vec.h"
#include "drivers/screen.h"

tcolor color_pack(uint8_t red, uint8_t green, uint8_t blue) {
    return (red << 16) | (green << 8) | blue;
}

tcolor color_packAlpha(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    return (red << 16) | (green << 8) | blue | (alpha << 24);
}

uint8_t color_getRed(tcolor red) {
    return (red >> 16) % 256;
}

uint8_t color_getGreen(tcolor green) {
    return (green >> 8) % 256;
}

uint8_t color_getBlue(tcolor blue) {
    return blue % 256;
}

uint8_t color_getGray(tcolor color) {
    return (color_getRed(color) + color_getGreen(color) + color_getBlue(color)) / 3;
}

uint8_t color_getAlpha(tcolor color) {
    return (color >> 24) % 256;
}

uint16_t color_to565(tcolor color) {
    uint16_t result;
    result = (color_getRed(color) >> 3) << 11;
    result |= (color_getGreen(color) >> 2) << 5;
    result |= (color_getBlue(color) >> 3);
    return result;
}

tcolor color_from565(uint16_t color) {
    return color_pack(
        (((color >> 11) & 0x1F) * 255 + 15) / 31,
        (((color >> 5)  & 0x3F) * 255 + 31) / 63,
        (((color)       & 0x1F) * 255 + 15) / 31
    );
}

tcolor color_swap(tcolor color) {
    return color_pack(color_getBlue(color), color_getGreen(color), color_getRed(color));
}

tcolor color_hsv(uint8_t hue, uint8_t saturation, uint8_t value) {
    float h = hue / 255.0;
    float s = saturation / 255.0;
    float v = value / 255.0;

    float r = 0;
    float g = 0;
    float b = 0;

    int i = h * 6;

    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch (i % 6) {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        case 5:
            r = v;
            g = p;
            b = q;
            break;
    }

    return color_pack(r * 255, g * 255, b * 255);
}

tcolor color_random() {
    return color_pack((uint8_t)esp_random(), (uint8_t)esp_random(), (uint8_t)esp_random());
}

tcolor color_randomGray() {
    uint8_t gray = (uint8_t)esp_random();
    return color_pack(gray, gray, gray);
}

tcolor color_randomBlackwhite() {
    if (esp_random() % 2 == 0) {
        return color_black;
    } else {
        return color_white;
    }
}

tcolor color_select(tcolor blackwhite, tcolor monochrome, tcolor colored) {
    switch (screen_getColormode()) {
        case screen_blackwhite:
            return blackwhite;
        case screen_monochrome:
            return monochrome;
        case screen_colored:
            return colored;
    }
    return color_black;
}

tcolor color_mselect(tcolor blackwhite_monochrome, tcolor colored) {
    return color_select(blackwhite_monochrome, blackwhite_monochrome, colored);
}

tcolor color_wmselect(tcolor colored) {
    return color_mselect(color_white, colored);
}

tcolor color_bmselect(tcolor colored) {
    return color_mselect(color_black, colored);
}

tcolor color_combine(float v, tcolor color1, tcolor color2) {
    float mul1 = 1;
    float mul2 = 1;
    if (v > 0.5) {
        mul1 = 1 - ((v - 0.5) * 2);
    } else if (v < 0.5) {
        mul2 = v * 2;
    }

    uint8_t r1 = nRound(color_getRed(color1) * mul1);
    uint8_t g1 = nRound(color_getGreen(color1) * mul1);
    uint8_t b1 = nRound(color_getBlue(color1) * mul1);
    uint8_t r2 = nRound(color_getRed(color2) * mul2);
    uint8_t g2 = nRound(color_getGreen(color2) * mul2);
    uint8_t b2 = nRound(color_getBlue(color2) * mul2);

    struct Vec vec;
    vec_new(&vec, 3);
    vec_set(&vec, 0, (r1 + r2) / 2);
    vec_set(&vec, 1, (g1 + g2) / 2);
    vec_set(&vec, 2, (b1 + b2) / 2);
    return color_pack(vec_get(&vec, 0), vec_get(&vec, 1), vec_get(&vec, 2));
}