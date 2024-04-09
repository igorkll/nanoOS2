#include "main.h"
#include "math.h"

static uint32_t lastFpsReadTime = 0;
static float sum = 0;
static uint16_t count = 0;
static int fps = 0;

void xmath_fpsCountReset() {
    lastFpsReadTime = 0;
    sum = 0;
    count = 0;
    fps = 0;
}

int xmath_fpsCount(int dt) {
    uint32_t time = uptime();
    if (time - lastFpsReadTime >= 1000) {
        lastFpsReadTime = time;
        fps = -1;
        if (count == 0) fps = nRound(sum / count);
        sum = 0;
        count = 0;
    }

    sum += (1.0 / dt) * 1000.0;
    count++;

    if (fps == -1) return nRound((1.0 / dt) * 1000.0);
    return fps;
}