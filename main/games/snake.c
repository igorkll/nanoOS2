#include "../main.h"
#include "snake.h"
#include "../graphic.h"
#include "../gui.h"
#include "../color.h"

void snake_run() {
    int score = 0;
    int len = 1;
    int speed = 100;
    int tick = 0;

    int boxOffset = gui_getStatusBarPosY();
    int boxSizeX = graphic_x();
    int boxSizeY = graphic_y() - boxOffset;
    int boxSize = boxSizeX * boxSizeY;
    uint16_t* box = malloc(boxSize);

    void boxSet(int x, int y, uint16_t val) {
        box[x + (y + boxSizeY)] = val;
    }

    uint16_t boxGet(int x, int y) {
        return box[x + (y + boxSizeY)];
    }

    for (int ix = 0; ix < boxSizeX; ix++) {
        for (int iy = 0; iy < boxSizeY; iy++) {
            boxSet(ix, iy, 0);
        }
    }
    boxSet(3, 3, 20);

    while (true) {
        if (tick % speed == 0) {
            graphic_clear(color_black);
            gui_drawScoreBar(score);
            for (int ix = 0; ix < boxSizeX; ix++) {
                for (int iy = 0; iy < boxSizeY; iy++) {
                    if (boxGet(ix, iy) > 0) {
                        graphic_drawPixel(ix, iy + boxOffset, color_white);
                        boxSet(ix, iy, boxGet(ix, iy) - 1);
                    }
                }
            }
            graphic_update();
        }

        if (gui_isEnter()) {
            free(box);
            return;
        }
        yield();
        tick = tick + 1;
    }
}