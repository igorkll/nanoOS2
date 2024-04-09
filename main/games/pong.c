#include "pong.h"
#include "../main.h"
#include "../graphic.h"
#include "../gui.h"
#include "../color.h"
#include "../control.h"
#include "../palette.h"

void pong_run() {
    tcolor bg = color_bmselect(palette_games_bg);
    tcolor fg = color_wmselect(palette_games_content);

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
        int iterations = 0;
        do {
            vBallX = (((float)esp_random()) / ((float)4294967295) - 0.5) * 2;
            iterations++;
        } while ((fabs(vBallX) < 0.1 || fabs(vBallY) > 0.8) && iterations < 100);
        iterations = 0;
        do {
            vBallY = (((float)esp_random()) / ((float)4294967295) - 0.5) * 2;
            iterations++;
        } while ((fabs(vBallY) < 0.1 || fabs(vBallY) > 0.3) && iterations < 100);

        float len = sqrt(pow(vBallX, 2) + pow(vBallY, 2));
        vBallX /= len;
        vBallY /= len;

        vBallX = -fabs(vBallX);
    }
    reVector();

    void gameover() {
        graphic_clear(bg);
        int y = gui_drawScoreBar(score);
        graphic_drawConterTextBox(0, y, graphic_x(), graphic_y() - y, "GAMEOVER", fg);
        graphic_update();
        control_waitExit();
    }

    bool isRacketTouch(int racketPos) {
        return ballY >= racketPos && ballY < racketPos + racketSizeY;
    }

    while (true) {
        // control
        if (control_needExit()) return;
        if (control_isMoveButton(CONTROL_UP)) selfPos -= selfSpeed;
        if (control_isMoveButton(CONTROL_DOWN)) selfPos += selfSpeed;
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

        ballSpeed += 0.00005;
        if (ballSpeed > 0.3) ballSpeed = 0.3;

        opponentSpeed += 0.00005;
        if (opponentSpeed > 0.05) opponentSpeed = 0.05;

        selfSpeed += 0.0001;
        if (selfSpeed > 0.6) selfSpeed = 0.6;

        // draw
        graphic_clear(bg);
        graphic_drawInteger(4, 1, score, fg);
        graphic_fillRect(nRound(ballX) - 1, nRound(ballY) - 1, 3, 3, fg);
        graphic_fillRect(0, nRound(selfPos), racketSizeX, racketSizeY, fg);
        graphic_fillRect(graphic_x() - racketSizeX, nRound(opponentPos), racketSizeX, racketSizeY, fg);
        graphic_update();
        wait(10);
    }
}