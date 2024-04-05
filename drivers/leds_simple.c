#include "../main/main.h"
#include "../main/color.h"
#include "../main/system.h"
#include "../main/drivers/leds.h"
#include <driver/ledc.h>

uint8_t pins[] = LEDS_PINS;
uint8_t channels[sizeof(pins)];

esp_err_t leds_init() {
    esp_err_t ret = ESP_OK;

    for (int i = 0; i < C_SIZE(pins); i++) {
        int channel = system_newLedc(pins[i]);
        if (channel > 0) {
            channels[i] = channel;
            #ifdef LEDS_INVERT
                ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, CRTValue(255));
            #else
                ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, CRTValue(0));
            #endif
        } else {
            ret = ESP_FAIL;
        }
    }

    return ret;
}

int leds_getCount() {
    return C_SIZE(pins);
}

void leds_setColor(int index, uint32_t color) {
    uint8_t duty = color_getGray(color);
    #ifdef LEDS_INVERT
        ledc_set_duty(LEDC_LOW_SPEED_MODE, channels[index], CRTValue(255 - duty));
    #else
        ledc_set_duty(LEDC_LOW_SPEED_MODE, channels[index], CRTValue(duty));
    #endif
    ledc_update_duty(LEDC_LOW_SPEED_MODE, channels[index]);
}