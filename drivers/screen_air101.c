#include "../main/main.h"
#include "../main/color.h"
#include "../main/drivers/screen.h"
#include "driver/spi_master.h"

#ifndef SCREEN_RESX
    #define SCREEN_RESX 160
#endif

#ifndef SCREEN_RESY
    #define SCREEN_RESY 80
#endif

uint16_t buffer[SCREEN_RESX * SCREEN_RESY];

// -------------------------------- SPI

screen_colormode screen_getColormode() {
    return screen_colored;
}

uint32_t screen_get(int x, int y) {
    return 0;
}

void screen_set(int x, int y, uint32_t color) {
    buffer[x + (y * SCREEN_RESX)] = color_to565(color);
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
    esp_err_t ret = ESP_OK;

    

    return ret;
}