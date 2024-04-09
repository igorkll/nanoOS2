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
    uint8_t curX  = graphic_getCursorX();
    uint8_t curY  = graphic_getCursorY();
    graphic_resetCrop();
    graphic_resetCursor();
    graphic_clear(color_black);
    app();
    graphic_setCropXY(cropX, cropY);
    graphic_setCursor(curX, curY);
}

void system_xApp(int stack, int fps, int tps, void(*draw)(int, float), bool(*tick)(int, float)) {
    static int fpsTime; fpsTime = nRound((1.0 / fps) * 1000);
    static int tpsTime; tpsTime = nRound((1.0 / tps) * 1000);
    static bool exit; exit = false;
    static bool end1; end1 = false;
    static bool end2; end2 = false;

    void drawTask(void* pvParameters) {
        void(*func)(int, float) = pvParameters;
        uint32_t oldTime = uptime();
        bool first = true;
        while (!exit) {
            uint32_t startTime = uptime();
            uint32_t delta = startTime - oldTime;
            float mul;
            if (!first) {
                mul = (float)delta / fpsTime;
            } else {
                mul = 1;
                delta = fpsTime;
            }
            first = false;
            func(delta, mul);
            int needWait = fpsTime - (uptime() - startTime);
            if (needWait > 0) wait(needWait);
            oldTime = startTime;
        }

        end1 = true;
        vTaskDelete(NULL);
    }

    void tickTask(void* pvParameters) {
        bool(*func)(int, float) = pvParameters;
        uint32_t oldTime = uptime();
        bool first = true;
        while (!exit) {
            uint32_t startTime = uptime();
            uint32_t delta = startTime - oldTime;
            float mul;
            if (!first) {
                mul = (float)delta / tpsTime;
            } else {
                mul = 1;
                delta = tpsTime;
            }
            first = false;
            if (func(delta, mul)) exit = true;
            int needWait = tpsTime - (uptime() - startTime);
            if (needWait > 0) wait(needWait);
            oldTime = startTime;
        }

        end2 = true;
        vTaskDelete(NULL);
    }

    xTaskCreate(drawTask, NULL, stack, (void*)draw, 1, NULL);
    xTaskCreate(tickTask, NULL, stack, (void*)tick, 1, NULL);
    while (!exit || !end1 || !end2) yield();
}