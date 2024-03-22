#include "snake.h"
#include "../main.h"
#include "../graphic.h"
#include "../gui.h"
#include "../color.h"

void snake_run() {
    int score = 0;
    int len = 2;
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

    void randomizeDot() {
        while (true) {
            int x = esp_random() % boxSizeX;
            int y = esp_random() % boxSizeY;
            if (boxGet(x, y) == 0) {
                boxSet(x, y, -1);
                break;
            }
        }
    }

    int direction = 1;
    int snakePosX = boxSizeX / 2;
    int snakePosY = boxSizeY / 2;

    for (int ix = 0; ix < boxSizeX; ix++) {
        for (int iy = 0; iy < boxSizeY; iy++) {
            boxSet(ix, iy, 0);
        }
    }
    boxSet(snakePosX, snakePosY, len - 1);
    randomizeDot();

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
            int value = boxGet(snakePosX, snakePosY);
            if (value > 0 || snakePosX < 0 || snakePosY < 0 || snakePosX >= boxSizeX || snakePosY >= boxSizeY) {
                gameOver();
                return;
            } else if (value == -1) {
                randomizeDot();
                score++;
                len++;

                if (score % 10 == 0) {
                    randomizeDot();

                    tick = 1;
                    speed -= 5;
                    if (speed < 25) speed = 25;
                }
            } else {
                for (int ix = 0; ix < boxSizeX; ix++) {
                    for (int iy = 0; iy < boxSizeY; iy++) {
                        int value = boxGet(ix, iy);
                        if (value > 0) {
                            boxSet(ix, iy, value - 1);
                        }
                    }
                }
            }
            boxSet(snakePosX, snakePosY, len);

            graphic_clear(color_black);
            gui_drawScoreBar(score);
            for (int ix = 0; ix < boxSizeX; ix++) {
                for (int iy = 0; iy < boxSizeY; iy++) {
                    int value = boxGet(ix, iy);
                    if (value > 0) {
                        graphic_fillRect(ix * crop, (iy * crop) + boxOffset, crop, crop, color_white);
                    } else if (value < 0) {
                        graphic_drawRect(ix * crop, (iy * crop) + boxOffset, crop, crop, color_white);
                    }
                }
            }
            graphic_update();
        }

        tick = tick + 1;
        if (gui_isEnter()) {
            free(box);
            return;
        }
        for (int i = 0; i < 4; i++) {
            if (gui_isMoveButton(i)) {
                if (abs(direction - i) != 2) {
                    tick = 0;
                    direction = i;
                }
            }
        }
        wait(10);
    }
}