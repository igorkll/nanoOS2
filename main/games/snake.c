#include "../main.h"
#include "snake.h"
#include "../graphic.h"
#include "../gui.h"
#include "../color.h"

void snake_run() {
    int score = 0;
    int len = 5;
    int speed = 50;
    int tick = 0;
    int crop = 4;

    int boxOffset = gui_getStatusBarPosY();
    int boxSizeX = graphic_x() / crop;
    int boxSizeY = (graphic_y() - boxOffset) / crop;
    int boxSize = boxSizeX * boxSizeY;
    int16_t* box = malloc(boxSize * sizeof(int16_t));
    if (box == NULL) {
        printf("box alloc error");
        return;
    }

    void boxSet(int x, int y, int16_t val) {
        box[x + (y * boxSizeX)] = val;
    }

    int16_t boxGet(int x, int y) {
        return box[x + (y * boxSizeX)];
    }

    void gameOver() {
        free(box);
        graphic_clear(color_black);
        gui_drawScoreBar(score);
        graphic_drawConterTextBox(0, boxOffset, boxSizeX * crop, boxSizeY * crop, "GAMEOVER", color_white);
        graphic_update();
        while (!gui_isEnter()) yield();
    }

    int direction = 0;
    int snakePosX = boxSizeX / 2;
    int snakePosY = boxSizeY / 2;

    for (int ix = 0; ix < boxSizeX; ix++) {
        for (int iy = 0; iy < boxSizeY; iy++) {
            boxSet(ix, iy, 0);
        }
    }

    while (true) {
        if (tick % speed == 0) {
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


            graphic_clear(color_black);
            gui_drawScoreBar(score);
            graphic_drawPixel(3, boxOffset, color_white);
            for (int ix = 0; ix < boxSizeX; ix++) {
                for (int iy = 0; iy < boxSizeY; iy++) {
                    int value = boxGet(ix, iy);
                    if (value > 0) {
                        boxSet(ix, iy, value - 1);
                        graphic_fillRect(ix * crop, (iy * crop) + boxOffset, crop, crop, color_white);
                    }
                }
            }
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