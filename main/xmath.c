#include "main.h"
#include "math.h"

static uint32_t lastFpsReadTime = 0;
static uint32_t sum = 0;
static uint16_t count = 0;
static int fps = 0;
int xmath_fpsCount(int dt) {
    uint32_t time = uptime();
    if (time - lastFpsReadTime >= 1000) {
        lastFpsReadTime = time;
        fps = sum / count;
        sum = 0;
        count = 0;
    }

    sum += nRound((1.0 / dt) * 1000.0);
    count++;
    return fps;
}