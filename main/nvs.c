#include "main.h"
#include "nvs.h"
#include <nvs_flash.h>

esp_err_t nvs_init() {
    esp_err_t ret = nvs_flash_init();
    if (ret != ESP_OK) {
        nvs_flash_erase();
        ret = nvs_flash_init();
    }
    return ret;
}