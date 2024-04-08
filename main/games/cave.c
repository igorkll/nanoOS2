#include "../all.h"

void cave_run() {
    graphic_setYCloserTo(40);

    void drawCallback(int dt, float mul) {
        printf("draw %i %f\n", dt, mul);
    }

    bool tickCallback(int dt, float mul) {
        printf("tick %i %f\n", dt, mul);
        return false;
    }

    system_xApp(16000, 25, 5, drawCallback, tickCallback);
}