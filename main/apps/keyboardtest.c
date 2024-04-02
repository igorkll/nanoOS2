#include "keyboardtest.h"
#include "../main.h"
#include "../gui.h"
#include "../graphic.h"
#include "../functions.h"
#include "../control.h"
#include "../color.h"

void keyboardtest_run() {
    graphic_clear(color_black);
    graphic_setCursor(0, 0);
    while (true) {
        for (int i = 32; i <= 126; i++) {
            char str[] = " ";
            str[0] = i;
            graphic_print(str, color_white);
            graphic_update();
            wait(100);
        }
    }
}