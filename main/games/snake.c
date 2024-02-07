#include "../main.h"
#include "snake.h"
#include "../graphic.h"
#include "../gui.h"
#include "../color.h"

void snake_run() {
    int score = 0;
    int len = 5;
    int speed = 100;
    int tick = 0;

    int boxOffset = gui_getStatusBarPosY();
    int boxSizeX = graphic_x();
    int boxSizeY = graphic_y() - boxOffset;
    int boxSize = boxSizeX * boxSizeY;
    uint16_t* box = malloc(boxSize);

    void boxSet(int x, int y, uint16_t val) {
        box[x + (y * boxSizeX)] = val;
    }

    uint16_t boxGet(int x, int y) {
        return box[x + (y * boxSizeX)];
    }

    void gameOver() {
        graphic_clear(color_black);
        gui_drawScoreBar(score);
        graphic_drawTextBox(0, boxOffset, boxSizeX, boxSizeY, "GAMEOVER", color_white);
        graphic_update();
        while (!gui_isEnter()) yield();
    }

    for (int ix = 0; ix < boxSizeX; ix++) {
        for (int iy = 0; iy < boxSizeY; iy++) {
            boxSet(ix, iy, 0);
        }
    }


    int direction = 0;
    int snakePosX = boxSizeX / 2;
    int snakePosY = boxSizeY / 2;
    boxSet(snakePosX, snakePosY, 20);

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
            switch (direction) {
                case 0:
                    snakePosY--;
                    break;
                case 1:
                    snakePosX++;
                    break;
                case 2:
                    snakePosY++;
                    break;
                case 3:
                    snakePosX--;
                    break;
            }
            if (snakePosX < 0 || snakePosY < 0 || snakePosX >= boxSizeX || snakePosY >= boxSizeY) {
                gameOver();
                return;
            }
            boxSet(snakePosX, snakePosY, len);
            graphic_update();
        }

        if (gui_isEnter()) {
            free(box);
            return;
        }
        for (int i = 0; i < 4; i++) {
            if (gui_isMoveButton(i)) direction = i;
        }
        yield();
        tick = tick + 1;
    }
}