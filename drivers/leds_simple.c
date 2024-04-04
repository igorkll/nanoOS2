#include "../main/main.h"
#include "../main/color.h"
#include "../main/drivers/leds.h"
#include "driver/ledc.h"

uint8_t pins[] = LEDS_PINS;
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_8_BIT
#define LEDC_FREQUENCY          5000

esp_err_t leds_init() {
    esp_err_t ret = ESP_OK;
    esp_err_t lastret;

    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = LEDC_FREQUENCY,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    lastret = ledc_timer_config(&ledc_timer);
    if (lastret == ESP_OK) { 
        for (int i = 0; i < C_SIZE(pins); i++) {
            ledc_channel_config_t ledc_channel = {
                .speed_mode     = LEDC_MODE,
                .channel        = i,
                .timer_sel      = LEDC_TIMER,
                .intr_type      = LEDC_INTR_DISABLE,
                .gpio_num       = pins[i],
                #ifdef LEDS_INVERT
                    .duty           = 255,
                #else
                    .duty           = 0,
                #endif
                .hpoint         = 0
            };
            lastret = ledc_channel_config(&ledc_channel);
            if (lastret != ESP_OK) { 
                ret = lastret;
            }
            if (i >= 7) break;
        }
    } else {
        ret = lastret;
    }

    return ret;
}

int leds_getCount() {
    return C_SIZE(pins);
}

void leds_setColor(int index, uint32_t color) {
    uint8_t duty = color_getGray(color);
    #ifdef LEDS_INVERT
        ledc_set_duty(LEDC_LOW_SPEED_MODE, index, CRTValue(255 - duty));
    #else
        ledc_set_duty(LEDC_LOW_SPEED_MODE, index, CRTValue(duty));
    #endif
    ledc_update_duty(LEDC_LOW_SPEED_MODE, index);
}