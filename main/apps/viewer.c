#include "../all.h"
#include "viewer.h"

void viewer_run() {

}

void viewer_open(const char* path) {
    graphic_clear(color_black);
    uint32_t* img = graphic_loadImage(path);
    graphic_draw(graphic_centerX(img[0]), graphic_centerY(img[1]), img);
    graphic_update();
    control_waitEnter();
}