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

uint8_t buffer[(SCREEN_RESX * SCREEN_RESY) * 2];

// -------------------------------- SPI

screen_colormode screen_getColormode() {
    return screen_colored;
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