#include "../main.h"
#include "snake.h"
#include "../graphic.h"
#include "../gui.h"
#include "../color.h"

void snake_run() {
    int score = 0;
    while (true) {
        graphic_clear(color_black);

        String scoreStr = "score: ";
        scoreStr += score;
        graphic_drawText(2, 2, scoreStr);
    }
}