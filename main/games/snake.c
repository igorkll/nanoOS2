#include "../main.h"
#include "snake.h"
#include "../graphic.h"
#include "../gui.h"
#include "../color.h"

void snake_run() {
    int score = 0;
    int skip = 0;
    uint16_t box[graphic_x()][gui_getStatusBarPosY()];

    while (true) {
        graphic_clear(color_black);
        int start = gui_drawScoreBar(score);
        graphic_drawPixel(2, start, color_white);
        graphic_update();

        if (gui_isEnter()) return;
        yield();
    }
}