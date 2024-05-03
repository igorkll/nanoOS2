#include "main.h"
#include "color.h"
#include "vec.h"
#include "graphic.h"
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
    return color_packAlpha(color_getBlue(color), color_getGreen(color), color_getRed(color), color_getAlpha(color));
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
    switch (graphic_getColormode()) {
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
    float r1 = color_getRed(color1);
    float g1 = color_getGreen(color1);
    float b1 = color_getBlue(color1);

    float r2 = color_getRed(color2);
    float g2 = color_getGreen(color2);
    float b2 = color_getBlue(color2);

    return color_pack(
        r1 + v * (r2 - r1),
        g1 + v * (g2 - g1),
        b1 + v * (b2 - b1)
    );
}

tcolor color_invert(tcolor color) {
    return color_packAlpha(
        255 - color_getRed(color),
        255 - color_getGreen(color),
        255 - color_getBlue(color),
        color_getAlpha(color)
    );
}

tcolor color_noAlpha(tcolor color) {
    return color_pack(color_getRed(color), color_getGreen(color), color_getBlue(color));
}

float color_atof(uint8_t arg){
    return arg / 255.0;
}

uint8_t color_ftoa(float arg) {
    return nRound(arg * 255);
}

tcolor color_mul(tcolor color, float mul) {
    int32_t red = nRound(color_getRed(color) * mul);
    int32_t green = nRound(color_getGreen(color) * mul);
    int32_t blue = nRound(color_getBlue(color) * mul);
    if (red < 0) red = 0;
    if (green < 0) green = 0;
    if (blue < 0) blue = 0;
    if (red > 255) red = 255;
    if (green > 255) green = 255;
    if (blue > 255) blue = 255;
    return color_packAlpha(red, green, blue, color_getAlpha(color));
}