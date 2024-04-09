#include "main.h"
#include "math.h"

static uint32_t lastFpsReadTime = 0;
static float sum = 0;
static uint16_t count = 0;
static int fps = -1;

void xmath_fpsCountReset() {
    lastFpsReadTime = 0;
    sum = 0;
    count = 0;
    fps = -1;
}

int xmath_fpsCount(int dt) {
    uint32_t time = uptime();
    if (time - lastFpsReadTime >= 1000) {
        lastFpsReadTime = time;
        fps = -1;
        if (count != 0) fps = nRound(sum / count);
        sum = 0;
        count = 0;
    }

    float currentFPS = (1.0 / dt) * 1000.0;
    sum += currentFPS;
    count++;
    if (fps == -1) fps = nRound(currentFPS);
    return fps;
}