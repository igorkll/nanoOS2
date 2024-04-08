#include "../all.h"

void cave_run() {
    graphic_setYCloserTo(40);

    void drawCallback(int dt, float mul) {
        graphic_clear(color_bmselect(0x0d0064));
        
        graphic_update();
    }

    bool tickCallback(int dt, float mul) {
        printf("tick %i %f\n", dt, mul);
        return control_isEnterPressed();
    }

    system_xApp(16000, 25, 5, drawCallback, tickCallback);
}