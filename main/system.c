#include "main.h"
#include "system.h"
#include "graphic.h"
#include "xmath.h"
#include "filesystem.h"
#include "storage.h"
#include "control.h"
#include "device.h"

uint32_t system_vars[sys_var_count];
uint8_t system_debugMode = 0;

void system_printVars() {
    printf("-------- vars\n");
    for (int i = 0; i < sys_var_count; i++) {
        printf("var %i: %li\n", i, system_vars[i]);
    }
    printf("--------\n\n");
}

void system_reset() {
    filesystem_defaultDirectory();
    xmath_fpsCountReset();
    graphic_setCropXY(sysconf_data.cropX, sysconf_data.cropY);
    graphic_resetCursor();
    graphic_clear(color_black);
    graphic_end();
    device_setAutoBacklight(true);
}

static void _app_info(bool end) {
    if (system_debugMode > 0) {
        printf(end ? "application end\n" : "application start\n");
        printf("free memory: %li\n", esp_get_free_heap_size());
        printf("----\n");
        if (end) printf("\n\n");
    }
}

void system_runApp(void(*app)()) {
    _app_info(false);
    uint8_t cropX = graphic_getCropX();
    uint8_t cropY = graphic_getCropY();
    uint8_t curX  = graphic_getCursorX();
    uint8_t curY  = graphic_getCursorY();
    system_reset();
    app();
    system_reset();
    if (sysconf_data.cropX != cropX || sysconf_data.cropY != cropY) {
        graphic_setCropXY(sysconf_data.cropX, sysconf_data.cropY);
    } else {
        graphic_setCropXY(cropX, cropY);
    }
    graphic_setCursor(curX, curY);
    _app_info(true);
}

bool system_isLittleEndian() {
    uint16_t value = 0x1234;
    uint8_t *byte = (uint8_t*)&value;
    return *byte == 0x34;
}

// if you set tps to 0, one task will be used for rendering and processing
void system_xApp(int32_t stack, uint8_t fps, uint8_t tps, void(*draw)(int, float, void*), bool(*tick)(int, float, void*), void* param) {
    struct tunnel {
        int fpsTime;
        int tpsTime;
        bool exit;
        bool end1;
        bool end2;
        void* param;
        void(*draw)(int, float, void*);
        bool(*tick)(int, float, void*);
    };
    struct tunnel tunnelData = {
        .fpsTime = nRound((1.0 / fps) * 1000),
        .exit = false,
        .end1 = false,
        .end2 = false,
        .param = param,
        .draw = draw,
        .tick = tick
    };
    if (tps > 0) {
        tunnelData.tpsTime = nRound((1.0 / tps) * 1000);
    } else {
        tunnelData.tpsTime = -1;
        tunnelData.end2 = true;
    }

    void drawTask(void* pvParameters) {
        struct tunnel* tunnelData = (struct tunnel*)pvParameters;

        uint32_t oldTime = 0;
        bool first = true;
        while (!tunnelData->exit) {
            uint32_t startTime = system_uptime();
            uint32_t delta = startTime - oldTime;
            float mul;
            if (!first) {
                mul = (float)delta / tunnelData->fpsTime;
            } else {
                mul = 1;
                delta = tunnelData->fpsTime;
            }
            first = false;
            if (tunnelData->tpsTime < 0 && tunnelData->tick(delta, mul, tunnelData->param)) tunnelData->exit = true;
            if (tunnelData->exit) break;
            tunnelData->draw(delta, mul, tunnelData->param);
            int needWait = tunnelData->fpsTime - (system_uptime() - startTime);
            if (needWait > 0) wait(needWait);
            oldTime = startTime;
        }

        tunnelData->end1 = true;
        vTaskDelete(NULL);
    }

    void tickTask(void* pvParameters) {
        struct tunnel* tunnelData = (struct tunnel*)pvParameters;

        uint32_t oldTime = 0;
        bool first = true;
        while (!tunnelData->exit) {
            uint32_t startTime = system_uptime();
            uint32_t delta = startTime - oldTime;
            float mul;
            if (!first) {
                mul = (float)delta / tunnelData->tpsTime;
            } else {
                mul = 1;
                delta = tunnelData->tpsTime;
            }
            first = false;
            control_begin();
            if (tunnelData->tick(delta, mul, tunnelData->param)) tunnelData->exit = true;
            int needWait = tunnelData->tpsTime - (system_uptime() - startTime);
            if (needWait > 0) wait(needWait);
            oldTime = startTime;
        }

        tunnelData->end2 = true;
        vTaskDelete(NULL);
    }

    xTaskCreate(drawTask, NULL, stack, &tunnelData, 1, NULL);
    if (tps > 0) xTaskCreate(tickTask, NULL, stack, &tunnelData, 1, NULL);
    while (!tunnelData.exit || !tunnelData.end1 || !tunnelData.end2) yield();
}



static uint32_t currentTime;

static void serviceTask(void* pvParameters) {
    while (true) {
        currentTime = xTaskGetTickCount();
        vTaskDelay(1);
    }
}

uint32_t system_uptime() {
    return currentTime * portTICK_PERIOD_MS;
}

esp_err_t system_init() {
    if (xTaskCreate(serviceTask, NULL, 1000, NULL, 1, NULL) != pdPASS) {
        return ESP_FAIL;
    }
    return ESP_OK;
}