#include "screentest.h"
#include "../main.h"
#include "../gui.h"
#include "../graphic.h"
#include "../color.h"
#include "../functions.h"
#include "../control.h"

void screentest_run() {
    gui_splash("screen test");

    for (int y = 0; y < graphic_y();y++) {
        int col = map(y, 0, graphic_y() - 1, 0, 256);
        graphic_line(0, y, graphic_x(), y, color_pack(col, col, col));
    }
    graphic_update();
    control_waitEnter();

    for (int x = 0; x < graphic_x();x++) {
        int col = map(x, 0, graphic_x() - 1, 0, 256);
        graphic_line(x, 0, x, graphic_y(), color_pack(col, col, col));
    }
    graphic_update();
    control_waitEnter();

    graphic_clear(color_black);
    for (int x = 0; x < graphic_x();x++) {
        for (int y = 0; y < graphic_y();y++) {
            if ((x + y) % 2 == 0) {
                graphic_drawPixel(x, y, color_white);
            }
        }
    }
    graphic_update();
    control_waitEnter();
}