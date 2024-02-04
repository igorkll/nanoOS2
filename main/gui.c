#include "main.h"
#include "graphic.h"
#include "color.h"
#include "gui.h"

void gui_status(const char* text) {
    graphic_clear(color_black);
    graphic_drawRect(2, 2, graphic_x() - 4, graphic_y() - 4, color_white);
    graphic_drawTextBox(3, 3, graphic_x() - 4, graphic_y() - 4, text, color_white);
    graphic_update();
}