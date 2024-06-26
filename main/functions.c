#include "main.h"
#include "control.h"
#include "system.h"

void wait(int time) {
    vTaskDelay(time / portTICK_PERIOD_MS);
}

void yield() {
    vTaskDelay(1);
}

unsigned long yieldTime = 0;
void mYield() {
    unsigned long t = system_uptime();
    if (t - yieldTime > 3000) {
        yield();
        yieldTime = t;
    }
}

void loop() {
    while (true) {
        yield();
    }
}

int nRound(float num) {
    return (int)(num < 0 ? (num - 0.5) : (num + 0.5));
}

float clamp(float n, float lower, float upper) {
    return fmax(lower, fmin(n, upper));
}

int rmap(int value, int low, int high, int low_2, int high_2) {
    return nRound(fmap(value, low, high, low_2, high_2));
}

int map(int value, int low, int high, int low_2, int high_2) {
    return fmap(value, low, high, low_2, high_2);
}

float fmap(float value, float low, float high, float low_2, float high_2) {
    float relative_value = (value - low) / (high - low);
    float scaled_value = low_2 + (high_2 - low_2) * relative_value;
    return scaled_value;
}

uint8_t CRTValue(uint8_t val) {
  return (0.0003066 * pow(val, 2.46));
}



esp_err_t pin_up(uint16_t pin, uint8_t mode) {
    gpio_config_t conf = {
        .pin_bit_mask = (1ULL<<pin),
        .mode = mode,
        .pull_up_en = GPIO_PULLUP_ENABLE, 
        .pull_down_en = GPIO_PULLUP_DISABLE
    };

    return gpio_config(&conf);
}

esp_err_t pin_down(uint16_t pin, uint8_t mode) {
    gpio_config_t conf = {
        .pin_bit_mask = (1ULL<<pin),
        .mode = mode,
        .pull_up_en = GPIO_PULLUP_DISABLE, 
        .pull_down_en = GPIO_PULLUP_ENABLE
    };

    return gpio_config(&conf);
}

esp_err_t pin(uint16_t pin, uint8_t mode) {
    gpio_config_t conf = {
        .pin_bit_mask = (1ULL<<pin),
        .mode = mode,
        .pull_up_en = GPIO_PULLUP_DISABLE, 
        .pull_down_en = GPIO_PULLUP_DISABLE
    };

    return gpio_config(&conf);
}


bool waitUntil(int time, bool(*until)()) {
    int ticksTime = time / portTICK_PERIOD_MS;
    while (true) {
        if (until()) return true;
        vTaskDelay(1);
        if (--ticksTime <= 0) return false;
    }
}

bool waitUntilWithControlBegin(int time, bool(*until)()) {
    int ticksTime = time / portTICK_PERIOD_MS;
    while (true) {
        control_begin();
        if (until()) return true;
        vTaskDelay(1);
        if (--ticksTime <= 0) return false;
    }
}