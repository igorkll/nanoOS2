#include "../all.h"

void cave_run() {
    graphic_setYCloserTo(40);

    char* level;

    void loadLevel(const char* path) {
        int filesize = filesystem_size(path);
        if (filesize <= 0) return;
        level = malloc(filesize + 1);
        filesystem_readFile(path, level, filesize);
        level[filesize] = '\0';
    }

    void start() {
        loadLevel("/cave/0");
    }

    void drawCallback(int dt, float mul) {
        graphic_clear(color_bmselect(0x0d0064));
        graphic_println("@", color_white);
        graphic_update();
    }

    bool tickCallback(int dt, float mul) {
        return control_isEnterPressed();
    }

    start();
    system_xApp(16000, 25, 20, drawCallback, tickCallback);
}