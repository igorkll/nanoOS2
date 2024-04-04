#include "main.h"
#include "color.h"

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
    uint8_t r = (uint8_t)(((color >> 11) & 0x1F) << 3);
    uint8_t g = (uint8_t)(((color >> 5) & 0x3F) << 2);
    uint8_t b = (uint8_t)((color & 0x1F) << 3);
    return color_pack(r, g, b);
}