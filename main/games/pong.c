#include "../main.h"
#include "pong.h"
#include "../graphic.h"
#include "../gui.h"
#include "../color.h"
#include "../drivers/keyboard.h"

void pong_run() {
    int racketSizeY = 10;
    int racketSizeX = 2;
    float selfPos = (graphic_y() / 2) - (racketSizeY / 2);
    float opponentPos = selfPos;
    float opponentSpeed = 0.01;
    float selfSpeed = 0.2;
    int score = 0;
    float ballSpeed = 0.1;

    float ballX = graphic_x() / 2;
    float ballY = graphic_y() / 2;
    float vBallX;
    float vBallY;

    void reVector() {
        do {
            vBallX = (((float)esp_random()) / ((float)4294967295) - 0.5) * 2;
        } while (fabs(vBallX) < 0.1);

        do {
            vBallY = (((float)esp_random()) / ((float)4294967295) - 0.5) * 2;
        } while (fabs(vBallY) < 0.1);

        float len = sqrt(pow(vBallX, 2) + pow(vBallY, 2));
        vBallX /= len;
        vBallY /= len;
    }
    reVector();

    void gameover() {
        graphic_clear(color_black);
        int y = gui_drawScoreBar(score);
        graphic_drawConterTextBox(0, y, graphic_x(), graphic_y() - y, "GAMEOVER", color_white);
        graphic_update();
        while (!gui_isEnter()) yield();
    }

    bool isRacketTouch(int racketPos) {
        return ballY >= racketPos && ballY < racketPos + racketSizeY;
    }

    while (true) {
        // control
        if (gui_isEnter()) return;
        if (keyboard_isMoveButton(0)) selfPos -= selfSpeed;
        if (keyboard_isMoveButton(2)) selfPos += selfSpeed;
        selfPos = clamp(selfPos, 0, graphic_y() - racketSizeY);

        // process
        ballX += vBallX * ballSpeed;
        ballY += vBallY * ballSpeed;
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
                ballX = racketSizeX + 1;
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
                reVector();
            }
        }
        opponentPos += (((float)ballY) - opponentPos) * opponentSpeed;

        ballSpeed += 0.0001;
        if (ballSpeed > 0.3) ballSpeed = 0.3;

        opponentSpeed += 0.0001;
        if (opponentSpeed > 0.05) opponentSpeed = 0.05;

        selfSpeed += 0.0001;
        if (selfSpeed > 0.6) selfSpeed = 0.6;

        // draw
        graphic_clear(color_black);
        graphic_drawInteger(4, 1, score, color_white);
        graphic_fillRect(nRound(ballX) - 1, nRound(ballY) - 1, 3, 3, color_white);
        graphic_fillRect(0, nRound(selfPos), racketSizeX, racketSizeY, color_white);
        graphic_fillRect(graphic_x() - racketSizeX, nRound(opponentPos), racketSizeX, racketSizeY, color_white);
        graphic_update();
        yield();
    }
}