#include "main.h"
#include "graphic.h"
#include "color.h"
#include "gui.h"
#include "drivers/keyboard.h"

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

    while (keyboard_isEnter()) yield();
    while (!keyboard_isEnter()) yield();
}