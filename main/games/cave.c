#include "../all.h"

void cave_run() {
    struct Game {
        char* level;
    };
    struct Game game = {
        .level = NULL
    };

    void loadLevel(const char* path) {
        game.level = NULL;
        int filesize = filesystem_size(path);
        if (filesize <= 0) return;
        game.level = malloc(filesize + 1);
        int readsize = filesystem_readFile(path, game.level, filesize);
        game.level[readsize] = '\0';
    }

    // main
    graphic_setYCloserTo(40);
    loadLevel("/storage/cave/0");

    void drawCallback(int dt, float mul, void* param) {
        struct Game* game = (struct Game*)param;

        graphic_clear(color_bmselect(0x0d0064));
        graphic_resetCursor();
        graphic_smartPrint(color_white, "FPS: %i", math_fpsCount(dt));
        graphic_update();
    }

    bool tickCallback(int dt, float mul, void* param) {
        struct Game* game = (struct Game*)param;

        return control_needExit();
    }
    
    system_xApp(16000, 25, 20, drawCallback, tickCallback, &game);
}