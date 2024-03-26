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
        graphic_print("QW", color_white);
        graphic_update();
        wait(100);
    }
}