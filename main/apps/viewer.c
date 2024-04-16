#include "../all.h"
#include "viewer.h"

void viewer_run() {

}

void viewer_open(const char* path) {
    uint8_t cropX = graphic_getCropX();
    uint8_t cropY = graphic_getCropY();
    graphic_setCrop(1);
    uint32_t* img = graphic_loadImage(path);
    graphic_clear(color_getGray(graphic_dumpPrimaryColor(img)) < 8 ? color_white : color_black);

    float mul;
    uint16_t rx = graphic_x();
    uint16_t ry = graphic_y();
    if (ry > rx) {
        mul = rx / img[0];
    } else {
        mul = ry / img[1];
    }

    uint32_t* resizedImage = graphic_resize(img, img[0] * mul, img[1] * mul);
    free(img);
    graphic_draw(graphic_centerX(resizedImage[0]), graphic_centerY(resizedImage[1]), resizedImage);
    free(resizedImage);

    graphic_update();
    control_waitEnter();
    graphic_setCropXY(cropX, cropY);
}