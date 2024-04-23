#include "../main/main.h"
#include "../main/color.h"
#include "../main/drivers/screen.h"


#ifndef SCREEN_RESX
    #define SCREEN_RESX 320
#endif

#ifndef SCREEN_RESY
    #define SCREEN_RESY 240
#endif

static uint8_t buffer[SCREEN_RESX * SCREEN_RESY * 2];
static uint8_t bus_pins[] = SCREEN_BUS;

// --------------------------------

screen_colormode screen_getColormode() {
    return screen_colored;
}

tcolor screen_get(int x, int y) {
    int index = (x + (y * SCREEN_RESX)) * 2;
    uint16_t color565 = (buffer[index] << 8) + buffer[index+1];
    #ifdef SCREEN_INVERT_COLORS
        color565 = 0xffff - color565;
    #endif
    tcolor color = color_from565(color565);
    #ifdef SCREEN_ALT_COLOR_ORDER
        color = color_swap(color);
    #endif
    return color;
}

void screen_set(int x, int y, tcolor color) {
    #ifdef SCREEN_ALT_COLOR_ORDER
        color = color_swap(color);
    #endif
    uint16_t color565 = color_to565(color);
    #ifdef SCREEN_INVERT_COLORS
        color565 = 0xffff - color565;
    #endif
    int index = (x + (y * SCREEN_RESX)) * 2;
    buffer[index] = color565 >> 8;
    buffer[index+1] = color565 % 256;
}

void screen_update() {
}

int screen_x() {
    return SCREEN_RESX;
}

int screen_y() {
    return SCREEN_RESY;
}

esp_err_t screen_init() {
    pin(SCREEN_DC, GPIO_MODE_DEF_OUTPUT);
    

    return ESP_OK;
}