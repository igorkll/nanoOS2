#include "main.h"

void wait(int time) {
    vTaskDelay(time / portTICK_PERIOD_MS);
}

void yield() {
    vTaskDelay(1);
}

unsigned long uptime() {
    return xTaskGetTickCount() * portTICK_PERIOD_MS;
}

unsigned long yieldTime = 0;
void mYield() {
    unsigned long t = uptime();
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

int nRound(double num) {
    return (int)(num < 0 ? (num - 0.5) : (num + 0.5));
}

float clamp(float n, float lower, float upper) {
    return fmax(lower, fmin(n, upper));
}

float map(float value, float low, float high, float low_2, float high_2) {
    float relative_value = (value - low) / (high - low);
    float scaled_value = low_2 + (high_2 - low_2) * relative_value;
    return scaled_value;
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