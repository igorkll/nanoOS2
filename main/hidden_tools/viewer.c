#include "viewer.h"
#include "../all.h"

bool viewer_draw(const char* path) {
    uint32_t* img = graphic_loadImage(path);
    if (img == NULL) {
        return false;
    }

    uint8_t cropX = graphic_getCropX();
    uint8_t cropY = graphic_getCropY();
    //graphic_setXYCloserTo(img[0], img[1]);
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
    graphic_setCropXY(cropX, cropY);
    return true;
}

void viewer_open(const char* path) {
    gui_status("image loading...");
    graphic_update();
    if (!viewer_draw(path)) {
        gui_status("failed to load");
    }
    graphic_update();
    control_waitExitOrEnter();
}