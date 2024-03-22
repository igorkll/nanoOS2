#include "screentest.h"
#include "/main.h"
#include "/gui.h"
#include "/graphic.h"
#include "/color.h"
#include "/functions.h"

void screentest_run() {
   gui_splash("screen test");
   for (int y = 0; y < graphic_y();y++) {
      int col = map(y, 0, graphic_y() - 1, 0, 255)
      graphic_line(0, y, graphic_x(), y, color_pack(col, col, col));
   }
   gui_waitEnter();
}