#include "../main/main.h"
#include "../main/color.h"
#include "../main/drivers/leds.h"
#include "driver/ledc.h"

int pins[] = {12, 13};
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (5) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_8_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (4095) // Set duty to 50%. ((2 ** 13) - 1) * 50% = 4095
#define LEDC_FREQUENCY          (5000) // Frequency in Hertz. Set frequency at 5 kHz

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
                .duty           = 0, // Set duty to 0%
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
    ledc_set_duty(LEDC_LOW_SPEED_MODE, index, color_getGray(color));
    ledc_update_duty(LEDC_LOW_SPEED_MODE, index);
}