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
    C_CLEAR(counts);
    C_CLEAR(states);

    int rectSize = ((graphic_getFontSizeX() + 1) * 7) + 2;

    void drawButton(const char* title, int index, int x, int y, bool state) {
        uint32_t color1 = state ? color_white : color_black;
        uint32_t color2 = state ? color_black : color_white;

        graphic_fillRect(x, y, rectSize, graphic_getFontSizeY() + 2, color1);
        graphic_drawText(x + 1, y + 1, title, color2);
        graphic_drawInteger(x + rectSize - 10, y + 1, counts[index], color2);

        if (state != states[index]) {
            if (state) {
                counts[index]++;
                if (counts[index] >= 100) {
                    counts[index] = 0;
                }
            }
            states[index] = state;
        }
    }

    while (true) {
        graphic_clear(color_gray);
        drawButton("UP", 0, 2, 2, control_isMoveButton(CONTROL_UP));
        drawButton("X", 1, 2, 12, control_isEnter());
        drawButton("DOWN", 2, 2, 22, control_isMoveButton(CONTROL_DOWN));
        drawButton("LEFT", 3, 2 + rectSize + 2, 2, control_isMoveButton(CONTROL_LEFT));
        drawButton("RIGHT", 4, 2 + rectSize + 2, 12, control_isMoveButton(CONTROL_RIGHT));
        graphic_update();
    }
}