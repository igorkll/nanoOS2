#include "all.h"

static bool autoBacklight = true;
static uint32_t lastUpdateTime;

static void serviceTask(void* pvParameters) {
    while (true) {
        if (autoBacklight) {
            bool targetBlState = system_uptime() - lastUpdateTime <= 5000;
            screen_setBacklightValue(targetBlState ? 255 : 96);
        }
        yield();
    }
}


void device_setAutoBacklight(bool state) {
    autoBacklight = state;
    lastUpdateTime = system_uptime();
    screen_setBacklightValue(255);
}

bool device_isAutoBacklight() {
    return autoBacklight;
}

void device_update() {
    lastUpdateTime = system_uptime();
}

esp_err_t device_init() {
    lastUpdateTime = system_uptime();
    if (xTaskCreate(serviceTask, NULL, 1000, NULL, 1, NULL) != pdPASS) {
        return ESP_FAIL;
    }
    return ESP_OK;
}