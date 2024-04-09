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

static void mathLevel(struct Game* game) {
    game->levelLen = strlen(game->level);
    game->levelSizeX = -1;
    game->levelSizeY = 1;
    game->playerPosX = 0;
    game->playerPosY = 0;
    int xPos = 0;
    for (int i = 0; i < game->levelLen; i++) {
        char chr = game->level[i];
        switch (chr) {
            case '\n':
                xPos = -1;
                game->levelSizeY++;
                if (game->levelSizeX == -1) game->levelSizeX = i;
                break;
            case '^':
                game->playerPosX = xPos;
                game->playerPosY = game->levelSizeY - 1;
                break;
        }
        xPos++; 
    }
}

static char levelGet(struct Game* game, int x, int y) {
    return game->level[x + (y * (game->levelSizeX + 1))];
}

static void levelSet(struct Game* game, int x, int y, int val) {
    game->level[x + (y * (game->levelSizeX + 1))] = val;
}

static void drawCallback(int dt, float mul, void* param) {
    struct Game* game = (struct Game*)param;

    graphic_clear(color_bmselect(0x0d0064));    
    for (int ix = 0; ix < game->levelSizeX; ix++) {
        for (int iy = 0; iy < game->levelSizeY; iy++) {
            if (levelGet(&game, ix, iy) != ' ') {
                graphic_drawPixel(ix, iy, color_red);
            }
        }
    }
    graphic_resetCursor();
    graphic_printf(color_white, "FPS: %i", xmath_fpsCount(dt));
    graphic_printf(color_white, "W: %i %i", game->levelSizeX, game->levelSizeY);
    graphic_printf(color_white, "P: %f %f", game->playerPosX, game->playerPosY);
    graphic_update();
}

static bool tickCallback(int dt, float mul, void* param) {
    struct Game* game = (struct Game*)param;

    return control_needExit();
}

void cave_run() {
    struct Game game;
    graphic_setYCloserTo(40);
    loadLevel(&game, "/storage/cave/0");
    mathLevel(&game);
    system_xApp(16000, 25, 20, drawCallback, tickCallback, &game);
    free(game.level);
}