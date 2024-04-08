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

void system_xApp(int stack, int fps, int tps, void(*draw)(float), bool(*tick)(float)) {
    int fpsTime = nRound((1.0 / fps) * 1000);
    int tpsTime = nRound((1.0 / tps) * 1000);

    TaskHandle_t drawTaskHandle;
    TaskHandle_t tickTaskHandle;
    bool exit = false;

    void drawTask(void* pvParameters) {
        uint32_t oldTime = uptime();
        while (true) {
            uint32_t startTime = uptime();
            uint32_t delta = startTime - oldTime;
            uint32_t needWait = fpsTime - delta;
            if (needWait > 0) wait(needWait);
            draw(delta);
            oldTime = startTime;
        }
    }

    void tickTask(void* pvParameters) {
        uint32_t oldTime = uptime();
        while (true) {
            uint32_t startTime = uptime();
            uint32_t delta = startTime - oldTime;
            uint32_t needWait = tpsTime - delta;
            if (needWait > 0) wait(needWait);
            if (tick(delta)) {
                vTaskDelete(drawTaskHandle);
                vTaskDelete(tickTaskHandle);
                exit = true;
            }
            oldTime = startTime;
        }
    }

    xTaskCreate(drawTask, NULL, stack, NULL, 1, &drawTaskHandle);
    xTaskCreate(tickTask, NULL, stack, NULL, 1, &tickTaskHandle);

    while (!exit) {
        wait(100);
    }
}