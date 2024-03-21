#include "main.h"
#include "color.h"

uint32_t color_pack(uint8_t red, uint8_t green, uint8_t blue) {
    return (red << 16) | (green << 8) | blue;
}

uint8_t color_getRed(uint8_t red) {
    return (red >> 16) % 256;
}

uint8_t color_getGreen(uint8_t green) {
    return (green >> 8) % 256;
}

uint8_t color_getBlue(uint8_t blue) {
    return blue % 256;
}

uint8_t color_getGray(uint32_t color) {
    return (color_getRed(color) + color_getGreen(color) + color_getBlue(color)) / 3;
}