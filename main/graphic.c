#include "main.h"
#include "graphic.h"

#include "../libs/libbmp/libbmp.h"
#include "drivers/screen.h"
#include "color.h"

void graphic_image(int x, int y, const char* path) {
    bmp_img img;
    if (bmp_img_read(&img, path) != BMP_OK) {
        printf("failed to draw bmp: %s\n", path);
        return;
    }

    for (int cx = 0; cx < img.img_header.biWidth; cx++) {
        for (int cy = 0; cy < img.img_header.biHeight; cy++) {
            screen_set(cx, cy, color_pack_rgb888(img.img_pixels[cy][cx].red, img.img_pixels[cy][cx].green, img.img_pixels[cy][cx].blue));
        }
    }

    bmp_img_free(&img);
}