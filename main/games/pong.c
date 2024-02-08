#include "../main.h"
#include "pong.h"
#include "../graphic.h"
#include "../gui.h"
#include "../color.h"
#include "../drivers/keyboard.h"

void pong_run() {
    int racketSizeY = 8;
    int racketSizeX = 2;
    int selfPos = (graphic_y() / 2) - (racketSizeY / 2);
    int opponentPos = selfPos;
    int opponentSpeed = 0.1;
    int score = 0;

    float ballX = graphic_x() / 2;
    float ballY = graphic_y() / 2;
    float vBallX = (((float)esp_random()) / ((float)4294967295) - 0.5) * 0.1;
    float vBallY = (((float)esp_random()) / ((float)4294967295) - 0.5) * 0.1;

    void gameover() {
        graphic_clear(color_black);
        int y = gui_drawScoreBar(score);
        graphic_drawConterTextBox(0, y, graphic_x(), graphic_y() - y, "GAMEOVER", color_white);
        graphic_update();
        while (!gui_isEnter()) yield();
    }

    bool isRacketTouch(int racketPos) {
        return ballY >= racketPos || ballY < racketPos + racketSizeY;
    }

    while (true) {
        ballX += vBallX;
        ballY += vBallY;
        if (ballY >= graphic_y()) {
            vBallY = -vBallY;
            ballY = graphic_y() - 1;
        } else if (ballY < 0) {
            vBallY = -vBallY;
            ballY = 0;
        }
        if (isRacketTouch(selfPos)) {
            if (ballX <= racketSizeX) {
                score++;
                ballX = racketSizeX - 1;
                vBallX = -vBallX;
            }
        } else {
            if (ballX <= 0) {
                gameover();
                return;
            }
        }
        if (isRacketTouch(opponentPos)) {
            if (ballX >= graphic_x() - 1 - racketSizeX) {
                ballX = graphic_x() - 2 - racketSizeX;
                vBallX = -vBallX;
            }
        } else {
            if (ballX >= graphic_x() - 1) {
                score += 10;
                ballX = graphic_x() / 2;
                ballY = graphic_y() / 2;
                vBallX = -vBallX;
            }
        }
        opponentPos += (ballY - opponentPos) * opponentSpeed;

        graphic_clear(color_black);
        graphic_drawInteger(1, 1, score, color_white);
        graphic_fillRect(nRound(ballX) - 1, nRound(ballY) - 1, 3, 3, color_white);
        graphic_fillRect(0, selfPos, racketSizeX, racketSizeY, color_white);
        graphic_fillRect(graphic_x() - racketSizeX, opponentPos, racketSizeX, racketSizeY, color_white);
        graphic_update();

        if (gui_isEnter()) return;
        yield();
    }
}