#include "main.h"
#include "color.h"

uint32_t color_pack(uint8_t red, uint8_t green, uint8_t blue) {
    return (red << 16) | (green << 8) | blue;
}