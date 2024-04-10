#include "main.h"
#include "color.h"
#include "drivers/screen.h"

tcolor color_pack(uint8_t red, uint8_t green, uint8_t blue) {
    return (red << 16) | (green << 8) | blue;
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
    float r, g, b;

    int i = h * 6;

    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    i = i % 6;

    switch (i) {
        case 0:
            r, g, b = v, t, p;
            break;
        case 1:
            r, g, b = q, v, p;
            break;
        case 2:
            r, g, b = p, v, t;
            break;
        case 3:
            r, g, b = p, q, v;
            break;
        case 4:
            r, g, b = t, p, v;
            break;
        case 5:
            r, g, b = v, p, q;
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