#include "../main/main.h"
#include "../main/color.h"
#include "../main/drivers/leds.h"
#include "driver/ledc.h"

int pins[] = {12, 13};

esp_err_t leds_init() {
    esp_err_t ret = ESP_OK;
    esp_err_t lastret;

    for (int i = 0; i < C_SIZE(pins); i++) {
        lastret = ledc_set_pin(pins[i], LEDC_LOW_SPEED_MODE, i);
        if (lastret == ESP_OK) { 
            gpio_set_level(pins[i], 0);
        } else {
            ret = lastret;
        }
        if (i >= 7) {
            break;
        }
    }

    return ret;
}

int leds_getCount() {
    return C_SIZE(pins);
}

void leds_setColor(int index, uint32_t color) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, index, color_getGray(color));
}