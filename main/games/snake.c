#include "snake.h"
#include "../main.h"
#include "../graphic.h"
#include "../gui.h"
#include "../color.h"
#include "../control.h"
#include "../palette.h"

void snake_run() {
    tcolor bg = color_bmselect(palette_games_bg);
    tcolor fg = color_wmselect(palette_games_content);
    tcolor eat = color_wmselect(color_red);
    
    int score = 0;
    int len = 2;
    int speed = 50;
    int tick = -1;
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
        graphic_clear(bg);
        gui_drawScoreBar(score);
        graphic_drawCenterTextBox(0, boxOffset, boxSizeX * crop, boxSizeY * crop, "GAMEOVER", fg);
        graphic_update();
        control_waitExitOrEnter();
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

    int snakeDir = 1;
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
        control_begin();
        
        uint32_t startTime = uptime();

        for (int i = 0; i < 4; i++) {
            if (control_isMoveButtonPressed(i)) {
                if (abs(snakeDir - i) != 2) {
                    tick = -1;
                    snakeDir = i;
                }
            }
        }

        tick = tick + 1;
        if (tick % speed == 0) {
            switch (snakeDir) {
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

            graphic_clear(bg);
            gui_drawScoreBar(score);
            for (int ix = 0; ix < boxSizeX; ix++) {
                for (int iy = 0; iy < boxSizeY; iy++) {
                    int value = boxGet(ix, iy);
                    if (value > 0) {
                        graphic_fillRect(ix * crop, (iy * crop) + boxOffset, crop, crop, fg);
                    } else if (value < 0) {
                        graphic_drawRect(ix * crop, (iy * crop) + boxOffset, crop, crop, eat);
                    }
                }
            }
            graphic_update();
        }
        
        uint32_t checkTime = uptime();
        if (control_needExit()) {
            free(box);
            return;
        }

        uint16_t execTime = uptime() - startTime;
        int16_t waitTime = 10 - execTime;
        if (waitTime > 0) wait(waitTime);
        if (uptime() - checkTime > 10) tick = -1;
    }
}