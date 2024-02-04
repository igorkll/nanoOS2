#include "main.h"
#include "functions.h"

void wait(int time) {
    vTaskDelay(time / portTICK_PERIOD_MS);
}

void yeild() {
    vTaskDelay(1);
}

void loop() {
    while (true) {
        yeild();
    }
}

bool file_exists(const char *filename) {
  struct stat buffer;   
  return (stat(filename, &buffer) == 0);
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