#include "keyboardtest.h"
#include "../main.h"
#include "../gui.h"
#include "../graphic.h"
#include "../functions.h"
#include "../control.h"
#include "../color.h"

void keyboardtest_run() {
    int counts[5];
    bool states[5];
    memset(counts, 0, sizeof(counts));
    memset(states, 0, sizeof(states));

    void drawButton(const char* title, int index, int x, int y, bool state) {
        uint32_t color1 = state ? color_white : color_black;
        uint32_t color2 = state ? color_black : color_white;
        int rectSize = graphic_x() / 2;

        graphic_fillRect(x, y, rectSize, graphic_getFontSizeY() + 2, color1);
        graphic_drawText(x + 1, y + 1, title, color2);
        graphic_drawInteger(x + rectSize - 12, y + 1, counts[index], color2);

        if (state != states[index]) {
            counts[index]++;
            states[index] = state;
        }
    }

    while (true) {
        graphic_clear(color_gray);
        drawButton("UP", 0, 4, 4, control_isMoveButton(CONTROL_UP));
        drawButton("ACTION", 1, 4, 14, control_isEnter());
        drawButton("DOWN", 2, 4, 24, control_isMoveButton(CONTROL_DOWN));
        graphic_update();
    }
}