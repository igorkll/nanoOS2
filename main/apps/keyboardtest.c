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

    void drawButton(const char* title, int index, int x, int y, bool state) {
        uint32_t color1 = state ? color_white : color_black;
        uint32_t color2 = state ? color_black : color_white;
        
        graphic_fillRect(x, y, 16, graphic_getFontSizeY() + 2, color1);
        graphic_drawText(x + 1, y + 1, title, color2);
        graphic_drawInteger(x + 8, y + 1, counts[index], color2);

        if (state != states[index]) {
            counts[index]++;
            states[index] = state;
        }
    }

    while (true) {
        graphic_clear(color_black);
        drawButton(const char* title, int index, int x, int y, bool state);
        graphic_update();
    }
}