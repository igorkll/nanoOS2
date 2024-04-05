#include "../../main.h"
#include "../../system.h"
#include "../screen.h"
#include <driver/ledc.h>

esp_err_t screen_init() {
    return ESP_FAIL;
}

int screen_x() {
    return 0;
}

int screen_y() {
    return 0;
}

void screen_set(int x, int y, uint32_t color) {}
void screen_update() {}
uint32_t screen_get(int x, int y) {
    return 0;
}

screen_colormode screen_getColormode() {
    return screen_blackwhite;
}

int bl_channel = -1;
void screen_setBacklightValue(uint8_t value) {
    // standard code backlight control
    #ifdef SCREEN_BL
        if (bl_channel < 0) bl_channel = system_newLedc(SCREEN_BL);
        if (bl_channel >= 0) {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, bl_channel, CRTValue(value));
        }
    #endif
}