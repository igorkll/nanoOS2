#include "test_screen.h"
#include "../drivers/screen.h"
#include "../main.h"
#include "../gui.h"
#include "../graphic.h"
#include "../color.h"
#include "../functions.h"
#include "../control.h"
#include "../system.h"

static void viewColors(int count, tcolor* colors) {
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
}

static void viewCombine(tcolor color1, tcolor color2) {
    for (int x = 0; x < graphic_x(); x++) {
        tcolor color = color_combine(fmap(x, 0, graphic_x() - 1, 0, 1), color1, color2);
        for (int y = 0; y < graphic_y(); y++) {
            graphic_drawPixel(x, y, color);
        }
    }
    graphic_update();
}

static bool fpsTest(uint8_t testmode) {
    float startTime = system_uptime();
    int frames = 0;
    bool force = false;
    while (true) {
        switch (testmode) {
            case 0:
                for (int x = 0; x < graphic_x();x++) {
                    for (int y = 0; y < graphic_y();y++) {
                        if ((x + y) % 2 == frames % 2) {
                            screen_colormode mode = graphic_getColormode();
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
                graphic_drawText(1, 1, "FPS TESTING...", color_black);
                break;
            case 1:
                graphic_clear(color_black);
                for (uint16_t x = 0; x < graphic_x(); x++) {
                    uint8_t posbyte = (rmap(x, 0, graphic_x() - 1, 0, 255) + frames) % 256;
                    uint16_t y = (graphic_y() / 2) + (sin(fmap(posbyte, 0, 255, 0, M_PI * 2)) * graphic_y() * 0.4);
                    graphic_fillRect(x, y - 2, 1, 5, color_hsv(posbyte, 255, 255));
                }
                graphic_drawText(1, 1, "FPS TESTING...", color_white);
                break;
            case 2:
                graphic_clear(color_black);
                graphic_drawText(1, 1, "FPS TESTING...", color_white);
                break;
        }
        graphic_update();
        frames++;
        if (system_uptime() - startTime >= 5000) break;

        control_begin();
        if (control_needExitWithoutGui()) {
            frames = -1;
            force = true;
            break;
        } else if (control_isEnterPressed()) {
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
    return force;
}

#define WAIT if (control_waitExitOrEnter()) return

void screentest_run() {
    graphic_setCrop(1);
    gui_splash("screen test");

    for (int y = 0; y < graphic_y();y++) {
        int col = map(y, 0, graphic_y() - 1, 0, 255);
        graphic_line(0, y, graphic_x(), y, color_pack(col, col, col));
    }
    graphic_update();
    WAIT;

    for (int x = 0; x < graphic_x();x++) {
        int col = map(x, 0, graphic_x() - 1, 0, 255);
        graphic_line(x, 0, x, graphic_y(), color_pack(col, col, col));
    }
    graphic_update();
    WAIT;

    graphic_clear(color_black);
    for (int x = 0; x < graphic_x();x++) {
        for (int y = 0; y < graphic_y();y++) {
            if ((x + y) % 2 == 0) {
                graphic_drawPixel(x, y, color_white);
            }
        }
    }
    graphic_update();
    WAIT;

    tcolor colors1[] = color_all_grays;
    viewColors(C_SIZE(colors1), colors1);
    WAIT;

    tcolor colors2[] = color_all_colors;
    viewColors(C_SIZE(colors2), colors2);
    WAIT;

    tcolor colors3[] = {0xFFE45A, 0xD2D2D2, 0xD8B19E, 0xDD7D54, 0x484647, 0xFFDD2D, 0xC46D4D};
    viewColors(C_SIZE(colors3), colors3);
    WAIT;

    viewCombine(color_red, color_black); WAIT;
    viewCombine(color_black, color_green); WAIT;
    viewCombine(color_green, color_orange); WAIT;
    viewCombine(color_red, color_blue); WAIT;

    while (true) {
        bool exitFlag = false;
        int hsvCrop = 4;
        for (int hue = 0; hue < 256; hue++) {
            for (int ix = 0; ix < graphic_x(); ix += hsvCrop) {
                for (int iy = 0; iy < graphic_y(); iy += hsvCrop) {
                    graphic_fillRect(ix, iy, hsvCrop, hsvCrop, color_hsv(hue, map(ix, 0, graphic_x() - 1, 0, 255), map(iy, 0, graphic_y() - 1, 255, 0)));
                }
            }
            graphic_update();

            if (waitUntilWithControlBegin(10, control_isEnterPressed)) {
                exitFlag = true;
                break;
            }
        }
        if (exitFlag) break;
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

            control_begin();
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
        if (waitUntilWithControlBegin(500, control_isEnterPressed)) {
            break;
        }
    }
    graphic_sprint("press enter to continue.", color_white);
    graphic_update();
    WAIT;

    for (uint8_t i = 0; i <= 2; i++) {
        if(fpsTest(i)) return;
        WAIT;
    }
}