#include "../../main.h"
#include "../screen.h"

esp_err_t screen_init() {
    return ESP_OK;
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