#include "../all.h"

const uint8_t blocksize = 8;

static struct Game {
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

static void levelSet(struct Game* game, int x, int y, char val) {
    game->level[x + (y * (game->levelSizeX + 1))] = val;
}

static void drawCallback(int dt, float mul, void* param) {
    struct Game* game = (struct Game*)param;
    int rx = graphic_x();
    int ry = graphic_y();

    graphic_clear(color_bmselect(0x0d0064));
    for (int ix = 0; ix < game->levelSizeX; ix++) {
        for (int iy = 0; iy < game->levelSizeY; iy++) {
            graphic_draw(graphic_centerX(blocksize), graphic_centerY(blocksize), game->player_img);
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
    graphic_update();
}

static char move(struct Game* game, float x, float y) {
    char chr = levelGet(game, nRound(game->playerPosX + x), nRound(game->playerPosY + y));
    if (chr != ' ') {
        game->playerPosX += x;
        game->playerPosY += y;
    }
    return chr;
}

static bool tickCallback(int dt, float mul, void* param) {
    struct Game* game = (struct Game*)param;
    if (control_isMoveButton(CONTROL_RIGHT)) move(game, mul * 0.1, 0);
    if (control_isMoveButton(CONTROL_LEFT)) move(game, mul * -0.1, 0);

    return control_needExit();
}

void cave_run() {
    struct Game game;
    game.level = NULL;
    game.stone_img = graphic_loadImage("/storage/cave/stone.bmp");
    game.end_img = graphic_loadImage("/storage/cave/end.bmp");
    game.player_img = graphic_loadImage("/storage/cave/player.bmp");

    graphic_setYCloserTo(25);
    loadLevel(&game, "/storage/cave/levels/0");
    mathLevel(&game);
    system_xApp(16000, 25, 20, drawCallback, tickCallback, &game);

    free(game.stone_img);
    free(game.end_img);
    free(game.player_img);
    free(game.level);
}