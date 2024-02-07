#include "../main.h"
#include "pong.h"
#include "../graphic.h"
#include "../gui.h"
#include "../color.h"
#include "../drivers/keyboard.h"

void pong_run() {
    int racketSize = 8;
    int selfPos = (graphic_y() / 2) - (racketSize / 2);
    int opponentPos = selfPos;

    float ballX = graphic_x() / 2;
    float ballY = graphic_y() / 2;
    float vBallX = (((float)esp_random()) / ((float)4294967295) - 0.5) * 0.1;
    float vBallY = (((float)esp_random()) / ((float)4294967295) - 0.5) * 0.1;

    while (true) {
        graphic_clear(color_black);
        graphic_fillRect(nRound(ballX) - 1, nRound(ballY) - 1, 3, 3, color_white);
        graphic_fillRect(0, selfPos, 2, racketSize, color_white);
        graphic_fillRect(graphic_x() - 2, opponentPos, 2, racketSize, color_white);
        graphic_update();

        ballX += vBallX;
        ballY += vBallY;

        if (gui_isEnter()) return;
        yield();
    }
}