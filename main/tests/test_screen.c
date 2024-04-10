#include "test_screen.h"
#include "../drivers/screen.h"
#include "../main.h"
#include "../gui.h"
#include "../graphic.h"
#include "../color.h"
#include "../functions.h"
#include "../control.h"

void viewColors(int count, tcolor* colors) {
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
    graphic_setCrop(1);
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

    tcolor colors1[] = color_all_grays;
    viewColors(C_SIZE(colors1), colors1);

    tcolor colors2[] = color_all_colors;
    viewColors(C_SIZE(colors2), colors2);

    tcolor colors3[] = {0xFFE45A, 0xD2D2D2, 0xD8B19E, 0xDD7D54, 0x484647, 0xFFDD2D, 0xC46D4D};
    viewColors(C_SIZE(colors3), colors3);

    while (true) {
        for (int hue = 0; hue < 256; hue++) {
            for (int ix = 0; ix < graphic_x(); ix++) {
                for (int iy = 0; iy < graphic_y(); iy++) {
                    graphic_drawPixel(0, 0, color_hsv(hue, map(ix, 0, graphic_x() - 1, 0, 255), map(iy, 0, graphic_y() - 1, 0, 255)));
                }
            }

            if (control_isEnterPressed()) break;
            wait(100);
        }
    }

    graphic_clear(color_black);
    graphic_setCursor(0, 0);
    for (int i = 0; i < 5; i++) {
        if (i > 0) graphic_println("", color_white);
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
    graphic_setCursor(0, 0);
    if (frames > 0) {
        graphic_printf(color_white, "FPS: %i", frames / 5);
    } else {
        graphic_printf(color_white, "FPS: test skipped");
    }
    graphic_update();
    control_waitEnter();
}