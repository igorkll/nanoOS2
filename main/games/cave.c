#include "../all.h"

const uint8_t blocksize = 12;
const float playerSizeUp = 0.3;
const float playerSizeDown = 0.5;
const float playerSizeSide = 0.3;
const uint8_t fakeBorderSize = 4;

static struct Game {
    uint8_t currentLevel;
    uint8_t gameState;
    bool block;
    float hvec;
    char* level;
    bool playerXFlip;
    int levelLen;
    int levelSizeX;
    int levelSizeY;
    float playerPosX;
    float playerPosY;
    uint32_t* stone_img;
    uint32_t* end_img;
    uint32_t* player_img;
    uint32_t* lava_img;
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
    char path[] = "cave/levels/*";
    path[strlen(path) - 1] = level + '0';
    if (filesystem_exists(path)) {
        loadLevel(game, path);
        return true;
    }
    return false;
}

static void gameCrop() {
    graphic_setYCloserTo(30);
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
    return levelGet(game, x, y);
}

static char levelGetAdvCheck(struct Game* game, float x, float y, float dx, float dy) {
    x += dx;
    y += dy;
    if (x < 0 || y < 0 || x >= game->levelSizeX || y >= game->levelSizeY) return '*';
    char chr = ' ';
    if (dx < 0 && dy < 0 && (chr == ' ' || chr == '^')) chr = levelGet(game, nRound(x - playerSizeSide), nRound(y - playerSizeUp));
    if (dx > 0 && dy > 0 && (chr == ' ' || chr == '^')) chr = levelGet(game, nRound(x + playerSizeSide), nRound(y + playerSizeDown));
    if (dx < 0 && dy > 0 && (chr == ' ' || chr == '^')) chr = levelGet(game, nRound(x - playerSizeSide), nRound(y + playerSizeDown));
    if (dx > 0 && dy < 0 && (chr == ' ' || chr == '^')) chr = levelGet(game, nRound(x + playerSizeSide), nRound(y - playerSizeUp));
    if (dx < 0 && (chr == ' ' || chr == '^')) chr = levelGet(game, nRound(x - playerSizeSide), nRound(y));
    if (dx > 0 && (chr == ' ' || chr == '^')) chr = levelGet(game, nRound(x + playerSizeSide), nRound(y));
    if (dy > 0 && (chr == ' ' || chr == '^')) chr = levelGet(game, nRound(x), nRound(y + playerSizeDown));
    if (dy < 0 && (chr == ' ' || chr == '^')) chr = levelGet(game, nRound(x), nRound(y - playerSizeUp));
    return chr;
}

static void levelSet(struct Game* game, int x, int y, char val) {
    game->level[x + (y * (game->levelSizeX + 1))] = val;
}

static char move(struct Game* game, float x, float y) {
    char chr = levelGetAdvCheck(game, game->playerPosX, game->playerPosY, x, y);
    if (chr == ' ' || chr == '^') {
        game->playerPosX += x;
        game->playerPosY += y;
    }
    return chr;
}

static char checkBlock(struct Game* game, char chr) {
    switch (chr) {
        case '@':
            game->currentLevel++;
            if (loadLevelWithNumber(game, game->currentLevel)) {
                mathLevel(game);
            } else {
                game->gameState = 2;
            }
            break;
        case '~':
            game->gameState = 1;
            break;
    }
    return chr;
}

#define drawStop graphic_end(); return
static void drawCallback(int dt, float mul, void* param) {
    struct Game* game = (struct Game*)param;
    if (game->block) { drawStop; }

    int rx = graphic_x();
    int ry = graphic_y();
    graphic_clear(color_bmselect(0x0d0064));
    if (game->gameState == 1) {
        struct xstr str = xstr_new();
        xstr_minsize(&str, 32);
        xstr_fill(&str, "gameover\nlevel: %i", game->currentLevel + 1);
        graphic_fullscreenTextBox(str.ptr, color_red);
        xstr_del(&str);
    } else if (game->gameState == 2) {
        graphic_fullscreenTextBox("WIN WIN!", color_green);
    } else {
        graphic_advancedDraw(graphic_centerX(blocksize) - 1, graphic_centerY(blocksize), game->player_img, game->playerXFlip, false);
        uint8_t light_count = 0;
        int light_posX[16];
        int light_posY[16];
        for (int ix = -fakeBorderSize; ix < game->levelSizeX + fakeBorderSize; ix++) {
            if (game->block) { drawStop; }
            for (int iy = -fakeBorderSize; iy < game->levelSizeY + fakeBorderSize; iy++) {
                if (game->block) { drawStop; }
                int px = ((ix * blocksize) - (game->playerPosX * blocksize) - (blocksize / 2)) + (rx / 2);
                int py = ((iy * blocksize) - (game->playerPosY * blocksize) - (blocksize / 2)) + (ry / 2);
                if (px + blocksize > 0 && py + blocksize > 0 && px < rx + blocksize && py < ry + blocksize) {
                    char chr = levelGetCheck(game, ix, iy);
                    switch (chr) {
                        case '#':
                        case '*':
                            graphic_draw(px, py, game->stone_img);
                            break;
                        case '@':
                            graphic_draw(px, py, game->end_img);
                            break;
                        case '~':
                            graphic_draw(px, py, game->lava_img);
                            if (light_count < 16) {
                                light_posX[light_count] = px;
                                light_posY[light_count] = py;
                                light_count++;
                            }
                            break;
                    }
                }
            }
        }
        if (graphic_isColor()) {
            uint32_t* dump = graphic_fullscreenDump();
            if (dump != NULL) {
                graphic_clear(color_packAlpha(0, 0, 0, color_ftoa(0.1)));
                gfx_coneBack((rx / 2) + (game->playerXFlip ? -4 : 1), (ry / 2) - 3, game->playerXFlip ? -1 : 1, 0, rx * 0.55, 0, 0, 0, 1, dump);
                for (int i = 0; i < light_count; i++) {
                    gfx_fillBack(light_posX[i], light_posY[i], blocksize, blocksize, dump);
                    int lpx = (light_posX[i] + (blocksize / 2)) - 1;
                    int lpy = light_posY[i] + 3;
                    gfx_coneBackLight(lpx, lpy, 0, -1, 8, blocksize / 2, 0, 0.5, 0, dump, 0.6, 0.05, color_red);
                }
                free(dump);
            }
        }
    }

    if (game->block) { drawStop; }
    if (system_isDebug()) {
        graphic_resetCursor();
        graphic_setCrop(1);
        graphic_printf(color_white, "FPS: %i", xmath_fpsCount(dt));
        gameCrop();
    }
    graphic_update();
}

static bool tickCallback(int dt, float mul, void* param) {
    struct Game* game = (struct Game*)param;
    if (game->gameState == 0) {
        if (control_isMoveButton(CONTROL_LEFT)) {
            game->playerXFlip = true;
            checkBlock(game, move(game, -0.1 * mul, 0));
        }
        if (control_isMoveButton(CONTROL_RIGHT)) {
            game->playerXFlip = false;
            checkBlock(game, move(game, 0.1 * mul, 0));
        }
        
        bool vecUp = game->hvec < 0;
        bool vecDown = game->hvec > 0;
        char chr = checkBlock(game, move(game, 0, game->hvec * mul));
        bool col = chr != ' ' && chr != '^';
        if (col && control_isMoveButtonPressed(CONTROL_UP) && !vecUp) {
            game->hvec = -0.3;
        } else if (col && vecUp) {
            game->hvec = 0;
        } else {
            game->hvec += 0.02;
            if (game->hvec > 0.15) game->hvec = 0.15;
        }

        chr = levelGetAdvCheck(game, game->playerPosX, game->playerPosY, 0, 0.25);
        char chr2 = levelGetAdvCheck(game, game->playerPosX, game->playerPosY, 0, -0.1);
        if (chr != ' ' && chr != '^' && (chr2 == ' ' || chr2 == '^') && vecDown) {
            game->playerPosY = nRound(game->playerPosY);
        }
    }

    bool exit = false;
    if (control_needExitWithoutGui()) {
        game->block = true;
        if (gui_exitQuestion()) exit = true;
        game->block = false;
    }
    return exit;
}

void cave_run() {
    struct Game game;
    game.block = false;
    game.level = NULL;
    game.currentLevel = 0;
    game.gameState = 0;
    game.hvec = 0;
    game.playerXFlip = false;
    game.stone_img = graphic_loadImage("cave/stone.bmp");
    game.end_img = graphic_loadImage("cave/end.bmp");
    game.player_img = graphic_loadImage("cave/player.bmp");
    game.lava_img = graphic_loadImage("cave/lava.bmp");

    if (system_isDebug()) {
        game.currentLevel = gui_selectNumber("select level", 1, 3) - 1;
    }

    gameCrop();
    loadLevelWithNumber(&game, game.currentLevel);
    mathLevel(&game);
    system_xApp(16000, 25, 20, drawCallback, tickCallback, &game);

    free(game.stone_img);
    free(game.end_img);
    free(game.player_img);
    free(game.lava_img);
    free(game.level);
}