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