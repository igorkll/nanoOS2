#include "../all.h"

void cave_run() {
    graphic_setYCloserTo(40);

    void drawCallback(float dt) {
        printf("draw %f\n", dt);
    }

    bool tickCallback(float dt) {
        printf("tick %f\n", dt);
        return false;
    }

    system_xApp(16000, 25, 5, drawCallback, tickCallback);
}