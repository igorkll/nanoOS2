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

    int boxSizeX = graphic_x();
    int boxSizeY = gui_getStatusBarPosY();
    uint16_t box[boxSizeX][boxSizeY];
    box[3][3] = 20;

    while (true) {
        if (tick % speed == 0) {
            graphic_clear(color_black);
            gui_drawScoreBar(score);
            for (int ix = 0; ix < boxSizeX; ix++) {
                for (int iy = 0; iy < boxSizeY; iy++) {
                    if (box[ix][iy] > 0) {
                        graphic_drawPixel(ix, iy, color_white);
                        box[ix][iy]--; 
                    }
                }
            }
            graphic_update();
        }

        if (gui_isEnter()) return;
        yield();
        tick = tick + 1
    }
}