#include "../main.h"
#include "snake.h"
#include "../graphic.h"
#include "../gui.h"
#include "../color.h"

void snake_run() {
    int score = 0;
    while (true) {
        graphic_clear(color_black);

        char scoreStr[16];
        strcpy(scoreStr, "score: ");

        char numStr[16];
        itoa(score, numStr, C_SIZE(numStr));
        strcat(scoreStr, numStr);

        graphic_drawText(2, 2, scoreStr, color_white);
    }
}