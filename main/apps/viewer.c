#include "../all.h"
#include "viewer.h"

void viewer_run() {

}

void viewer_draw(const char* path) {
    uint32_t* img = graphic_loadImage(path);
    graphic_clear(color_getGray(graphic_dumpPrimaryColor(img)) < 8 ? color_white : color_black);

    float mul;
    float rx = graphic_x();
    float ry = graphic_y();
    if (ry > rx) {
        mul = rx / img[0];
    } else {
        mul = ry / img[1];
    }

    uint32_t* resizedImage = graphic_resize(img, img[0] * mul, img[1] * mul);
    free(img);
    graphic_draw(graphic_centerX(resizedImage[0]), graphic_centerY(resizedImage[1]), resizedImage);
    free(resizedImage);
}

void viewer_open(const char* path) {
    uint8_t cropX = graphic_getCropX();
    uint8_t cropY = graphic_getCropY();
    gui_status("image loading...");
    graphic_update();
    graphic_setCrop(1);
    viewer_draw(path);
    graphic_update();
    control_waitEnter();
    graphic_setCropXY(cropX, cropY);
}