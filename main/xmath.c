#include "main.h"
#include "math.h"

uint32_t lastFpsReadTime = 0;
uint32_t sum = 0;
uint16_t count = 0;
int xmath_fpsCount(int dt) {
    uint32_t time = uptime();
    if (time - lastFpsReadTime >= 1000) {
        lastFpsReadTime = time;
        int fps = sum / count;
        sum = 0;
        count = 0;
        return fps;
    }

    sum += nRound((1.0 / dt) * 1000.0);
    count++;
}