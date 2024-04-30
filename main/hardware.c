#include "main.h"
#include "hardware.h"
#include "system.h"
#include <driver/ledc.h>

static uint8_t _getLedcChannel() {
    static uint8_t ledcChannel = -1;
    ledcChannel++;
    if (ledcChannel >= 8) ledcChannel = 0;
    return ledcChannel;
}

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES           LEDC_TIMER_8_BIT
#define LEDC_FREQUENCY          5000
uint8_t hardware_newLed(uint8_t pin) {
    static bool ledcInited = false;
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

    uint8_t channel = _getLedcChannel();
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

struct Button hardware_newButton() {
    struct Button button;
    button.pressTime = 0;
    button.changeTime = system_uptime();
    button.realState = false;
    button.state = false;
    return button;
}

#define FORCE_TIME 150
#define DEBOUNCE_TIME 50
#define AUTOPRESS_TIME 500
#define AUTOPRESS_TICK_TIME 200
int8_t hardware_checkButton(struct Button* button, bool state) {
    uint32_t time = system_uptime();
    bool oldState = button->state;
    if (time - button->changeTime >= FORCE_TIME) {
        button->realState = state;
        button->state = state;
        button->changeTime = time;
    } else if (state != button->realState) {
        button->realState = state;
        button->changeTime = time;
    }
    if (time - button->changeTime >= DEBOUNCE_TIME) {
        button->state = button->realState;
    }
    if (button->state && !oldState) {
        button->pressTime = time;
        return 1;
    } else if (!button->state && oldState) {
        button->pressTime = 0;
        return -1;
    } else if (button->state && time - button->pressTime > AUTOPRESS_TIME && time % AUTOPRESS_TICK_TIME == 0) {
        return 3;
    }
    return button->state ? 2 : 0;
}