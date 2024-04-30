#include "main.h"
#include "system.h"
#include "graphic.h"
#include "xmath.h"
#include "filesystem.h"
#include "storage.h"
#include "control.h"

static uint32_t vars[sys_var_count];

uint32_t system_getVar(sys_var index) {
    return vars[index];
}

void system_setVar(sys_var index, uint32_t value) {
    vars[index] = value;
}

void system_printVars() {
    printf("-------- vars\n");
    for (int i = 0; i < sys_var_count; i++) {
        printf("var %i: %li\n", i, system_getVar(i));
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
}

void system_runApp(void(*app)()) {
    uint8_t cropX = graphic_getCropX();
    uint8_t cropY = graphic_getCropY();
    uint8_t curX  = graphic_getCursorX();
    uint8_t curY  = graphic_getCursorY();
    system_reset();
    app();
    filesystem_defaultDirectory();
    if (sysconf_data.cropX != cropX || sysconf_data.cropY != cropY) {
        graphic_setCropXY(sysconf_data.cropX, sysconf_data.cropY);
    } else {
        graphic_setCropXY(cropX, cropY);
    }
    graphic_setCursor(curX, curY);
}

bool system_isLittleEndian() {
    uint16_t value = 0x1234;
    uint8_t *byte = (uint8_t*)&value;
    return *byte == 0x34;
}

static bool debugMode = false;

bool system_isDebug() {
    return debugMode;
}

void system_setDebug(bool state) {
    debugMode = state;
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
            uint32_t startTime = uptime();
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
            int needWait = tunnelData->fpsTime - (uptime() - startTime);
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
            uint32_t startTime = uptime();
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
            int needWait = tunnelData->tpsTime - (uptime() - startTime);
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