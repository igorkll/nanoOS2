#include "test_keyboard.h"
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
    int rectSizeY = graphic_getFontSizeY() + 2;

    void drawButton(const char* title, int index, int x, int y, bool state) {
        tcolor color1 = state ? color_white : color_black;
        tcolor color2 = state ? color_black : color_white;

        graphic_fillRect(x, y, rectSize, rectSizeY, color1);
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
        graphic_clear(color_mselect(color_black, color_gray));
        bool isEnter = control_isEnter();
        bool isDown = control_isMoveButton(CONTROL_DOWN);
        drawButton("UP", 0, 2, 2, control_isMoveButton(CONTROL_UP));
        drawButton("X", 1, 2, 2 + rectSizeY, isEnter);
        drawButton("DOWN", 2, 2, 2 + (rectSizeY * 2), isDown);
        drawButton("LEFT", 3, 2 + rectSize + 2, 2, control_isMoveButton(CONTROL_LEFT));
        drawButton("RIGHT", 4, 2 + rectSize + 2, 2 + rectSizeY, control_isMoveButton(CONTROL_RIGHT));
        graphic_drawText(1, graphic_y() - graphic_getFontSizeY() - 1, "X & DOWN - EXIT", color_mselect(color_white, color_red));
        graphic_update();
        if (isEnter && isDown) return;
    }
}