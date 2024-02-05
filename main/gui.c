#include "main.h"
#include "graphic.h"
#include "color.h"
#include "gui.h"
#include "drivers/keyboard.h"

bool enterState = false;
bool gui_isEnter() {
    bool currentState = keyboard_isEnter();
    bool resultState = !enterState && currentState;
    if (!currentState) {
        enterState = true;
    }
    return resultState;
}


void gui_status(const char* text) {
    graphic_clear(color_black);
    graphic_drawRect(2, 2, graphic_x() - 4, graphic_y() - 4, color_white);
    graphic_drawTextBox(4, 4, graphic_x() - 8, graphic_y() - 8, text, color_white);
    graphic_update();
}

void gui_splash(const char* text) {
    int offset = graphic_getFontSizeY() + 1;

    graphic_clear(color_black);
    graphic_drawRect(2, 2, graphic_x() - 4, graphic_y() - 4 - offset, color_white);
    graphic_drawTextBox(4, 4, graphic_x() - 8, graphic_y() - 8 - offset, text, color_white);
    graphic_drawText(2, graphic_y() - (offset + 1), "Press Enter", color_white);
    graphic_update();

    while (!gui_isEnter()) yield();
}

int gui_menu(const char* title, int pointsCount, char* points[]) {
    int fontY = graphic_getFontSizeY();
    int lineY = fontY + 2;
    int len = graphic_x() - 1;
    int menu = 0;

    void draw() {
        graphic_clear(color_black);
        graphic_drawText(1, 1, title, color_white);
        graphic_line(0, lineY, len, lineY, color_white);
        for (int i = 0; i < pointsCount; i++) {
            int pos = (lineY * i) + 1;
            graphic_fillRect(0, pos, len, fontY + 2, i == menu ? color_white : color_black);
            graphic_drawText(1, pos + 1, points[i], i == menu ? color_black : color_white);
        }
        graphic_update();
    }
    draw();

    while (true) {

        draw();

        yield();
    }
}