#include "main.h"
#include <driver/ledc.h>

int ledcChannel = -1;
int system_getLedcChannel() {
    ledcChannel++;
    if (ledcChannel >= 8) ledcChannel = 0;
    return ledcChannel;
}

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES           LEDC_TIMER_8_BIT
#define LEDC_FREQUENCY          5000
bool ledcInited = false;
int system_newLedc(uint8_t pin) {
    if (!ledcInited) {
        ledc_timer_config_t ledc_timer = {
            .speed_mode       = LEDC_MODE,
            .timer_num        = LEDC_TIMER,
            .duty_resolution  = LEDC_DUTY_RES,
            .freq_hz          = LEDC_FREQUENCY,
            .clk_cfg          = LEDC_AUTO_CLK
        };

        if (ledc_timer_config(&ledc_timer) != ESP_OK) return -1;
        ledcInited = true;
    }

    int channel = system_getLedcChannel();
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = channel,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = pin,
        .duty           = 0,
        .hpoint         = 0
    };
    if (ledc_channel_config(&ledc_channel) != ESP_OK) return -1;
    return channel;
}