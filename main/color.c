#include "main.h"
#include "color.h"
#include "drivers/screen.h"

uint32_t color_pack(uint8_t red, uint8_t green, uint8_t blue) {
    return (red << 16) | (green << 8) | blue;
}

uint8_t color_getRed(uint32_t red) {
    return (red >> 16) % 256;
}

uint8_t color_getGreen(uint32_t green) {
    return (green >> 8) % 256;
}

uint8_t color_getBlue(uint32_t blue) {
    return blue % 256;
}

uint8_t color_getGray(uint32_t color) {
    return (color_getRed(color) + color_getGreen(color) + color_getBlue(color)) / 3;
}

uint16_t color_to565(uint32_t color) {
    uint16_t result;
    result = (color_getRed(color) >> 3) << 11;
    result |= (color_getGreen(color) >> 2) << 5;
    result |= (color_getBlue(color) >> 3);
    return result;
}

uint32_t color_from565(uint16_t color) {
    return color_pack(
        (((color >> 11) & 0x1F) * 255 + 15) / 31,
        (((color >> 5)  & 0x3F) * 255 + 31) / 63,
        (((color)       & 0x1F) * 255 + 15) / 31
    );
}

uint32_t color_swap(uint32_t color) {
    return color_pack(color_getBlue(color), color_getGreen(color), color_getRed(color));
}

uint32_t color_random() {
    return color_pack((uint8_t)esp_random(), (uint8_t)esp_random(), (uint8_t)esp_random());
}

uint32_t color_randomGray() {
    uint8_t gray = (uint8_t)esp_random();
    return color_pack(gray, gray, gray);
}

uint32_t color_randomBlackwhite() {
    if (esp_random() % 2 == 0) {
        return color_black;
    } else {
        return color_white;
    }
}

uint32_t color_select(uint32_t blackwhite, uint32_t monochrome, uint32_t colored) {
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

uint32_t color_mselect(uint32_t blackwhite_monochrome, uint32_t colored) {
    return color_select(blackwhite_monochrome, blackwhite_monochrome, colored);
}

uint32_t color_wmselect(uint32_t colored) {
    return color_mselect(color_white, colored);
}

uint32_t color_bmselect(uint32_t colored) {
    return color_mselect(color_black, colored);
}