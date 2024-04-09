#include "../all.h"

static struct Game {
    char* level;
    int levelLen;
    int levelSizeX;
    int levelSizeY;
    float playerPosX;
    float playerPosY;
};

static void loadLevel(struct Game* game, const char* path) {
    game->level = NULL;
    int filesize = filesystem_size(path);
    if (filesize <= 0) return;
    game->level = malloc(filesize + 1);
    int readsize = filesystem_readFile(path, game->level, filesize);
    game->level[readsize] = '\0';
}

static void drawCallback(int dt, float mul, void* param) {
    struct Game* game = (struct Game*)param;

    graphic_clear(color_bmselect(0x0d0064));
    graphic_resetCursor();
    graphic_smartPrint(color_white, "FPS: %i", xmath_fpsCount(dt));
    graphic_smartPrint(color_white, "W: %i %i", game->levelSizeX, game->levelSizeY);
    graphic_smartPrint(color_white, "P: %f %f", game->playerPosX, game->playerPosY);
    graphic_update();
}

static bool tickCallback(int dt, float mul, void* param) {
    struct Game* game = (struct Game*)param;

    return control_needExit();
}

void cave_run() {
    struct Game game = {
        .level = NULL,
        .levelLen = 0,
        .levelSizeX = -1,
        .levelSizeY = 1,
        .playerPosX = 0,
        .playerPosY = 0
    };

    graphic_setYCloserTo(40);
    loadLevel(&game, "/storage/cave/0");
    game.levelLen = strlen(game.level);
    int xPos = 0;
    for (int i = 0; i < game.levelLen; i++) {
        char chr = game.level[i];
        switch (chr) {
            case '\n':
                xPos = -1;
                game.levelSizeY++;
                if (game.levelSizeX == -1) game.levelSizeX = i;
                break;
            case '^':
                game.playerPosX = xPos;
                game.playerPosY = game.levelSizeY - 1;
                break;
        }
        xPos++;
    }
    system_xApp(16000, 25, 20, drawCallback, tickCallback, &game);
    free(game.level);
}