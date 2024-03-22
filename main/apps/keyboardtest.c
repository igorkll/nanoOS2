#include "keyboardtest.h"
#include "../main.h"
#include "../gui.h"
#include "../graphic.h"
#include "../functions.h"
#include "../control.h"
#include "../color.h"

void keyboardtest_run() {
    while (true) {
        graphic_clear(color_black);
        graphic_setCursor(0, 0);
        graphic_print("l1", color_white);
        graphic_update();

        wait(100);
    }
}