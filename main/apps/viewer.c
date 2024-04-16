#include "../all.h"
#include "viewer.h"

void viewer_run() {

}

void viewer_open(const char* path) {
    graphic_setCrop(1);
    graphic_clear(color_black);
    uint32_t* img = graphic_loadImage(path);
    uint32_t* resizedImage = graphic_resize(img, graphic_x(), graphic_y());
    free(img);
    graphic_draw(graphic_centerX(resizedImage[0]), graphic_centerY(resizedImage[1]), resizedImage);
    free(resizedImage);
    graphic_update();
    control_waitEnter();
}