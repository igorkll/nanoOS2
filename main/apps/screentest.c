#include "screentest.h"
#include "../main.h"
#include "../gui.h"
#include "../graphic.h"
#include "../color.h"
#include "../functions.h"

void screentest_run() {
    gui_splash("screen test");

    for (int y = 0; y < graphic_y();y++) {
        int col = map(y, 0, graphic_y() - 1, 0, 255);
        graphic_line(0, y, graphic_x(), y, color_pack(col, col, col));
    }
    graphic_update();
    gui_waitEnter();

    for (int x = 0; x < graphic_x();x++) {
        int col = map(x, 0, graphic_x() - 1, 0, 255);
        graphic_line(x, 0, x, graphic_y(), color_pack(col, col, col));
    }
    graphic_update();
    gui_waitEnter();
}