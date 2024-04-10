#include "../all.h"

const uint8_t blocksize = 8;

static struct Game {
    uint8_t currentLevel;
    uint8_t gameState;
    char* level;
    int levelLen;
    int levelSizeX;
    int levelSizeY;
    float playerPosX;
    float playerPosY;
    uint32_t* stone_img;
    uint32_t* end_img;
    uint32_t* player_img;
};

static void loadLevel(struct Game* game, const char* path) {
    if (game->level != NULL) free(game->level);
    game->level = NULL;
    int filesize = filesystem_size(path);
    if (filesize <= 0) return;
    game->level = malloc(filesize + 1);
    int readsize = filesystem_readFile(path, game->level, filesize);
    game->level[readsize] = '\0';
}

static bool loadLevelWithNumber(struct Game* game, uint8_t level) {
    char path[] = "/storage/cave/levels/*";
    path[strlen(path) - 1] = level + '0';
    if (!filesystem_exists(path)) {
        loadLevel(game, path);
        return true;
    }
    return false;
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

static char levelGetCheck(struct Game* game, int x, int y) {
    if (x < 0 || y < 0 || x >= game->levelSizeX || y >= game->levelSizeY) return '*';
    return game->level[x + (y * (game->levelSizeX + 1))];
}

static void levelSet(struct Game* game, int x, int y, char val) {
    game->level[x + (y * (game->levelSizeX + 1))] = val;
}

static void drawCallback(int dt, float mul, void* param) {
    struct Game* game = (struct Game*)param;
    int rx = graphic_x();
    int ry = graphic_y();

    graphic_clear(color_bmselect(0x0d0064));
    if (game->gameState == 1) {
        xstr str = xstr_build(32, "gameover\nlevel: %i", game->currentLevel + 1);
        graphic_fullscreenTextBox(str, color_red);
        free(str);
    } else if (game->gameState == 2) {
        graphic_fullscreenTextBox("WIN WIN!", color_green);
    } else {
        graphic_draw(graphic_centerX(blocksize) - 1, graphic_centerY(blocksize), game->player_img);
        for (int ix = 0; ix < game->levelSizeX; ix++) {
            for (int iy = 0; iy < game->levelSizeY; iy++) {
                int px = ((ix * blocksize) - (game->playerPosX * blocksize) - (blocksize / 2)) + (rx / 2);
                int py = ((iy * blocksize) - (game->playerPosY * blocksize) - (blocksize / 2)) + (ry / 2);
                if (px + blocksize > 0 && py + blocksize > 0 && px < rx + blocksize && py < ry + blocksize) {
                    char chr = levelGet(game, ix, iy);
                    switch (chr) {
                        case '#':
                            graphic_draw(px, py, game->stone_img);
                            break;
                        case '@':
                            graphic_draw(px, py, game->end_img);
                            break;
                    }
                }
            }
        }
    }
    graphic_update();
}

static char move(struct Game* game, float x, float y) {
    char chr = levelGetCheck(game, nRound(game->playerPosX + x), nRound(game->playerPosY + y));
    if (chr == ' ' || chr == '^') {
        game->playerPosX += x;
        game->playerPosY += y;
    }
    return chr;
}

static bool tickCallback(int dt, float mul, void* param) {
    struct Game* game = (struct Game*)param;
    if (game->gameState == 0) {
        char chr = ' ';
        if (control_isMoveButton(CONTROL_RIGHT)) chr = move(game, mul * 0.1, 0);
        if (control_isMoveButton(CONTROL_LEFT)) chr = move(game, mul * -0.1, 0);
        if (chr == '@') {
            if (loadLevelWithNumber(game, ++game->currentLevel)) {
                mathLevel(game);
            } else {
                game->gameState = 2;
            }
        }
    }
    return control_needExit();
}

void cave_run() {
    struct Game game;
    game.level = NULL;
    game.currentLevel = 0;
    game.gameState = 1;
    game.stone_img = graphic_loadImage("/storage/cave/stone.bmp");
    game.end_img = graphic_loadImage("/storage/cave/end.bmp");
    game.player_img = graphic_loadImage("/storage/cave/player.bmp");

    graphic_setYCloserTo(25);
    loadLevelWithNumber(&game, game.currentLevel);
    mathLevel(&game);
    system_xApp(16000, 25, 20, drawCallback, tickCallback, &game);

    free(game.stone_img);
    free(game.end_img);
    free(game.player_img);
    free(game.level);
}