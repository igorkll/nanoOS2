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
    uint32_t* key_img;
    uint32_t* dump;
    uint8_t foundKeys;
    uint8_t needKeys;
};

tcolor colorChange_end(uint16_t x, uint16_t y, tcolor color) {
    return color_getAlpha(color) == 0 ? color_white : color_black;
}

tcolor colorChange_lava(uint16_t x, uint16_t y, tcolor color) {
    if (color_getAlpha(color) > 0) return color_alpha;
    return ((color_getGreen(color) == 0) ^ (x % 4 == 0)) ? color_white : color_black;
}

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
    graphic_setYCloserTo(40);
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

static char levelGetAdvCheck(struct Game* game, float x, float y, float dx, float dy, int16_t* bx, int16_t* by) {
    x += dx;
    y += dy;
    if (x < 0 || y < 0 || x >= game->levelSizeX || y >= game->levelSizeY) return '*';
    char chr = ' ';
    *bx = -1;
    *by = -1;
    if (dx < 0 && dy < 0 && (chr == ' ' || chr == '^')) {
        *bx = nRound(x - playerSizeSide);
        *by = nRound(y - playerSizeUp);
        chr = levelGet(game, *bx, *by);
    }
    if (dx > 0 && dy > 0 && (chr == ' ' || chr == '^')) {
        *bx = nRound(x + playerSizeSide);
        *by = nRound(y + playerSizeDown);
        chr = levelGet(game, *bx, *by);
    }
    if (dx < 0 && dy > 0 && (chr == ' ' || chr == '^')) {
        *bx = nRound(x - playerSizeSide);
        *by = nRound(y + playerSizeDown);
        chr = levelGet(game, *bx, *by);
    }
    if (dx > 0 && dy < 0 && (chr == ' ' || chr == '^')) {
        *bx = nRound(x + playerSizeSide);
        *by = nRound(y - playerSizeUp);
        chr = levelGet(game, *bx, *by);
    }
    if (dx < 0 && (chr == ' ' || chr == '^')) {
        *bx = nRound(x - playerSizeSide);
        *by = nRound(y);
        chr = levelGet(game, *bx, *by);
    }
    if (dx > 0 && (chr == ' ' || chr == '^')) {
        *bx = nRound(x + playerSizeSide);
        *by = nRound(y);
        chr = levelGet(game, *bx, *by);
    }
    if (dy > 0 && (chr == ' ' || chr == '^')) {
        *bx = nRound(x);
        *by = nRound(y + playerSizeDown);
        chr = levelGet(game, *bx, *by);
    }
    if (dy < 0 && (chr == ' ' || chr == '^')) {
        *bx = nRound(x);
        *by = nRound(y - playerSizeUp);
        chr = levelGet(game, *bx, *by);
    }
    return chr;
}

static void levelSet(struct Game* game, int x, int y, char val) {
    game->level[x + (y * (game->levelSizeX + 1))] = val;
}

static char move(struct Game* game, float x, float y, int16_t* bx, int16_t* by) {
    char chr = levelGetAdvCheck(game, game->playerPosX, game->playerPosY, x, y, bx, by);
    if (chr == ' ' || chr == '^') {
        game->playerPosX += x;
        game->playerPosY += y;
    }
    return chr;
}

static char checkBlock(struct Game* game, char chr, int16_t x, int16_t y) {
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
        case '!':
            game->foundKeys++;
            levelSet(game, x, y, ' ');
            break;
    }
    return chr;
}

#define drawStop graphic_end(); return

static void rawDraw(struct Game* game, bool drawStopAllow) {
    int rx = graphic_x();
    int ry = graphic_y();
    graphic_clear(color_bmselect(0x0d0064));
    if (game->gameState == 1) {
        xstr str = xstr_new();
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
            if (game->block && drawStopAllow) { drawStop; }
            for (int iy = -fakeBorderSize; iy < game->levelSizeY + fakeBorderSize; iy++) {
                if (game->block && drawStopAllow) { drawStop; }
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
                        case '!':
                            graphic_draw(px, py, game->key_img);
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
            if (game->dump == NULL) {
                game->dump = graphic_fullscreenDump();
            } else {
                graphic_fullscreenDumpTo(game->dump);
            }

            if (game->dump != NULL) {
                graphic_clear(color_packAlpha(0, 0, 0, color_ftoa(0.1)));
                gfx_coneBack((rx / 2) + (game->playerXFlip ? -4 : 1), (ry / 2) - 3, game->playerXFlip ? -1 : 1, 0, rx * 0.55, 0, 0, 0, 1, game->dump);
                for (int i = 0; i < light_count; i++) {
                    gfx_fillBack(light_posX[i], light_posY[i], blocksize, blocksize, game->dump);
                    int lpx = (light_posX[i] + (blocksize / 2)) - 1;
                    int lpy = light_posY[i] + 3;
                    gfx_coneBackLight(lpx, lpy, 0, -1, 8, blocksize / 2, 0, 0.5, 0, game->dump, 0.6, 0.05, color_red);
                }
            }
        }
    }
}

static void drawCallback(int dt, float mul, void* param) {
    struct Game* game = (struct Game*)param;
    if (game->block) { drawStop; }
    rawDraw(game, true);
    if (game->block) { drawStop; }
    if (system_debugMode > 0) {
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
        int16_t bx;
        int16_t by;
        if (control_isMoveButton(CONTROL_LEFT)) {
            game->playerXFlip = true;
            char chr = move(game, -0.1 * mul, 0, &bx, &by);
            checkBlock(game, chr, bx, by);
        }
        if (control_isMoveButton(CONTROL_RIGHT)) {
            game->playerXFlip = false;
            char chr = move(game, 0.1 * mul, 0, &bx, &by);
            checkBlock(game, chr, bx, by);
        }
        
        bool vecUp = game->hvec < 0;
        bool vecDown = game->hvec > 0;
        char lchr = move(game, 0, game->hvec * mul, &bx, &by);
        char chr = checkBlock(game, lchr, bx, by);
        bool col = chr != ' ' && chr != '^';
        if (col && control_isMoveButtonPressed(CONTROL_UP) && !vecUp) {
            game->hvec = -0.3;
        } else if (col && vecUp) {
            game->hvec = 0;
        } else {
            game->hvec += 0.02;
            if (game->hvec > 0.15) game->hvec = 0.15;
        }

        int16_t bx;
        int16_t by;
        chr = levelGetAdvCheck(game, game->playerPosX, game->playerPosY, 0, 0.25, &bx, &by);
        char chr2 = levelGetAdvCheck(game, game->playerPosX, game->playerPosY, 0, -0.1, &bx, &by);
        if (chr != ' ' && chr != '^' && (chr2 == ' ' || chr2 == '^') && vecDown) {
            game->playerPosY = nRound(game->playerPosY);
        }

        bool exit = false;
        if (control_needExitWithoutGui()) {
            game->block = true;
            rawDraw(game, false);
            if (gui_exitQuestion()) exit = true;
            game->block = false;
        }
        return exit;
    } else {
        return control_needExitOrEnter();
    }
}

void cave_run() {
    struct Game game = {};
    if (system_debugMode > 0) {
        int num = gui_selectNumber("select level", true, 1, filesystem_fileCount("cave/levels"), 1, 1);
        if (num < 0) return;
        game.currentLevel = num - 1;
    }
    game.stone_img = graphic_loadImage("cave/stone.bmp");
    game.end_img = graphic_loadImage("cave/end.bmp");
    game.player_img = graphic_loadImage("cave/player.bmp");
    game.lava_img = graphic_loadImage("cave/lava.bmp");
    game.key_img = graphic_loadImage("cave/key.bmp");

    if (!graphic_isColor()) {
        graphic_colorChange(game.end_img, colorChange_end);
        graphic_colorChange(game.lava_img, colorChange_lava);
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
    if (game.dump != NULL) free(game.dump);
}