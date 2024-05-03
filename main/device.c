#include "all.h"

static bool autoBacklight = true;
static uint32_t lastUpdateTime;
static uint8_t bl_value;

static void serviceTask(void* pvParameters) {
    while (true) {
        for (control_key key = 0; key < CONTROL_COUNT; key++) {
            if (control_rawGet(key)) {
                device_update();
                break;
            }
        }

        if (autoBacklight) {
            bool targetBlState = system_uptime() - lastUpdateTime <= 5000;
            bl_value = targetBlState ? sysconf_data.screen_light_active : sysconf_data.screen_light_idle;
            screen_setBacklightValue(bl_value);
        }

        wait(100);
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

void device_setBacklightValue(uint8_t value) {
    bl_value = value;
    device_setAutoBacklight(false);
    screen_setBacklightValue(value);
}

uint8_t device_getBacklightValue() {
    return bl_value;
}

esp_err_t device_init() {
    lastUpdateTime = system_uptime();
    if (xTaskCreate(serviceTask, NULL, 1000, NULL, 1, NULL) != pdPASS) {
        return ESP_FAIL;
    }
    return ESP_OK;
}