#include "test_screen.h"
#include "../drivers/screen.h"
#include "../main.h"
#include "../gui.h"
#include "../graphic.h"
#include "../color.h"
#include "../functions.h"
#include "../control.h"

void viewColors(int count, uint32_t* colors) {
    graphic_clear(colors[count - 1]);
    int colorSize = nRound((graphic_x() - 1) / (float)count);
    for (int x = 0; x < graphic_x();x++) {
        for (int y = 0; y < graphic_y();y++) {
            uint8_t index = x / colorSize;
            if (index < count) {
                graphic_drawPixel(x, y, colors[index]);
            }
        }
    }
    graphic_update();
    control_waitEnter();
}

void screentest_run() {
    gui_splash("screen test");

    for (int y = 0; y < graphic_y();y++) {
        int col = map(y, 0, graphic_y() - 1, 0, 255);
        graphic_line(0, y, graphic_x(), y, color_pack(col, col, col));
    }
    graphic_update();
    control_waitEnter();

    for (int x = 0; x < graphic_x();x++) {
        int col = map(x, 0, graphic_x() - 1, 0, 255);
        graphic_line(x, 0, x, graphic_y(), color_pack(col, col, col));
    }
    graphic_update();
    control_waitEnter();

    graphic_clear(color_black);
    for (int x = 0; x < graphic_x();x++) {
        for (int y = 0; y < graphic_y();y++) {
            if ((x + y) % 2 == 0) {
                graphic_drawPixel(x, y, color_white);
            }
        }
    }
    graphic_update();
    control_waitEnter();

    uint32_t colors1[] = color_all_grays;
    viewColors(C_SIZE(colors1), colors1);

    uint32_t colors2[] = color_all_colors;
    viewColors(C_SIZE(colors2), colors2);

    graphic_clear(color_black);
    graphic_setCursor(0, 0);
    for (int i = 0; i < 5; i++) {
        bool breakFlag = false;
        for (char i = 32; i <= 126; i++) {
            char str[] = " ";
            str[0] = i;
            graphic_print(str, color_random());
            graphic_update();
            wait(10);
            if (control_isEnterPressed()) {
                breakFlag = true;
                break;
            }
        }
        graphic_println("", color_white);
        if (breakFlag) break;
    }
    for (int i = 0; i < 10; i++) {
        graphic_sprint("RGB TEXT TEST!", color_random());
        graphic_update();
        if (waitUntil(500, control_isEnterPressed)) {
            break;
        }
    }
    graphic_sprint("press enter to continue.", color_white);
    graphic_update();
    control_waitEnter();

    float startTime = uptime();
    int frames = 0;
    while (true) {
        graphic_clear(color_black);
        for (int x = 0; x < graphic_x();x++) {
            for (int y = 0; y < graphic_y();y++) {
                if ((x + y) % 2 == frames % 2) {
                    screen_colormode mode = screen_getColormode();
                    if (mode == screen_blackwhite) {
                        graphic_drawPixel(x, y, color_randomBlackwhite());
                    } else if (mode == screen_monochrome) {
                        graphic_drawPixel(x, y, color_randomGray());
                    } else {
                        graphic_drawPixel(x, y, color_random());
                    }
                }
            }
        }
        graphic_update();
        frames++;
        if (uptime() - startTime >= 5000) break;
        if (control_isEnterPressed()) {
            frames = -1;
            break;
        }
    }
    graphic_clear(color_black);
    graphic_drawText(1, 1, "FPS:", color_white);
    if (frames > 0) {
        graphic_drawInteger(1, 2 + graphic_getFontSizeY(), frames / 5, color_white);
    } else {
        graphic_drawText(1, 2 + graphic_getFontSizeY(), "test was skipped", color_white);
    }
    graphic_update();
    control_waitEnter();
}