#include "../all.h"

void cave_run() {
    char* level = NULL;

    void loadLevel(const char* path) {
        level = NULL;
        int filesize = filesystem_size(path);
        if (filesize <= 0) return;
        level = malloc(filesize + 1);
        int readsize = filesystem_readFile(path, level, filesize);
        level[readsize] = '\0';
    }

    // main
    graphic_setYCloserTo(40);
    loadLevel("/storage/cave/0");

    void drawCallback(int dt, float mul) {
        graphic_clear(color_bmselect(0x0d0064));
        graphic_resetCursor();
        graphic_sprint(level, color_white);
        graphic_update();
    }

    bool tickCallback(int dt, float mul) {
        return control_needExit();
    }
    
    system_xApp(16000, 5, 20, drawCallback, tickCallback);
}