#include "main.h"
#include "system.h"
#include "graphic.h"

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

void system_runApp(void(*app)()) {
    uint8_t cropX = graphic_getCropX();
    uint8_t cropY = graphic_getCropY();
    graphic_resetCrop();
    app();
    graphic_setCropXY(cropX, cropY);
}

void system_xApp(int stack, int fps, int tps, void(*draw)(int, float), bool(*tick)(int, float)) {
    static int fpsTime; fpsTime = nRound((1.0 / fps) * 1000);
    static int tpsTime; tpsTime = nRound((1.0 / tps) * 1000);
    static TaskHandle_t drawTaskHandle;
    static TaskHandle_t tickTaskHandle;
    static bool exit; exit = false;

    void drawTask(void* pvParameters) {
        void(*func)(int, float) = pvParameters;
        uint32_t oldTime = uptime();
        bool first = true;
        while (true) {
            uint32_t startTime = uptime();
            uint32_t delta = startTime - oldTime;
            float mul = 1;
            if (!first) mul = fpsTime / delta;
            first = false;
            func(delta, mul);
            uint32_t needWait = fpsTime - (uptime() - startTime);
            if (needWait > 0) wait(needWait);
            oldTime = startTime;
        }
    }

    void tickTask(void* pvParameters) {
        bool(*func)(int, float) = pvParameters;
        uint32_t oldTime = uptime();
        bool first = true;
        while (true) {
            uint32_t startTime = uptime();
            uint32_t delta = startTime - oldTime;
            float mul = 1;
            if (!first) mul = tpsTime / delta;
            first = false;
            if (func(delta, mul)) {
                vTaskDelete(drawTaskHandle);
                vTaskDelete(tickTaskHandle);
                exit = true;
            }
            uint32_t needWait = tpsTime - (uptime() - startTime);
            if (needWait > 0) wait(needWait);
            oldTime = startTime;
        }
    }

    xTaskCreate(drawTask, NULL, stack, (void*)draw, 1, &drawTaskHandle);
    xTaskCreate(tickTask, NULL, stack, (void*)tick, 1, &tickTaskHandle);

    while (!exit) {
        wait(100);
    }
}