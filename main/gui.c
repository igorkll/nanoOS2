#include "main.h"
#include "graphic.h"
#include "color.h"
#include "gui.h"
#include "control.h"
#include "palette.h"
#include "filesystem.h"
#include "xstr.h"
#include "system.h"

void gui_status(const char* text) {
    graphic_clear(color_bmselect(palette_splash_bg));
    graphic_drawRect(2, 2, graphic_x() - 4, graphic_y() - 4, color_wmselect(palette_splash_frame));
    graphic_drawTextBox(4, 4, graphic_x() - 8, graphic_y() - 8, text, color_wmselect(palette_splash_text));
    graphic_update();
}

bool gui_splash(const char* text) {
    int offset = graphic_getFontSizeY() + 1;
    graphic_clear(color_bmselect(palette_splash_bg));
    graphic_drawRect(2, 2, graphic_x() - 4, graphic_y() - 4 - offset, color_wmselect(palette_splash_frame));
    graphic_drawTextBox(4, 4, graphic_x() - 8, graphic_y() - 8 - offset, text, color_wmselect(palette_splash_text));
    graphic_drawText(2, graphic_y() - (offset + 1), "Press Enter", color_wmselect(palette_splash_text));
    graphic_update();
    return control_waitExitOrEnter();
}

int gui_menu(struct menuState* menu) {
    uint16_t fontY = graphic_getFontSizeY();
    uint16_t fontOffset = 0;
    uint16_t lineY = fontY + 2;
    bool firstSelected = false;
    bool lastSelected = false;
    menu->rightLeftState = -1;

    if (menu->imgs && menu->imgs[0]) {
        uint16_t height = graphic_getImageHeight(menu->imgs[0]);
        fontY = height;
        fontOffset = height / 4;
    }

    void draw() {
        graphic_clear(color_bmselect(palette_menu_bg));
        for (int i = 0; i < menu->pointsCount; i++) {
            int pos = ((fontY + 2) * (i + menu->offset)) + lineY + 3;
            if (i == menu->current) {
                graphic_fillRect(0, pos, graphic_x(), fontY + 2, color_wmselect(palette_menu_select));
                firstSelected = pos <= (lineY + fontY);
                lastSelected = pos + fontY + 2 >= (graphic_y() - fontY);
            }

            uint8_t textOffset = 0;
            if (menu->imgs && menu->imgs[i]) {
                tcolor* img = graphic_loadImage(menu->imgs[i]);
                graphic_draw(1, pos + 1, img);
                textOffset = img[0] + 2;
                free(img);
            }
            graphic_drawText(1 + textOffset, pos + 1 + fontOffset, menu->points[i], i == menu->current ? color_bmselect(palette_menu_text) : color_wmselect(palette_menu_text));
        }
        gui_drawStatusBar(menu->title);
        graphic_update();
    }
    draw();

    while (true) {
        control_begin();
        if (control_isEnterPressed()) return menu->current;
        if (control_pageUp()) {
            menu->current = menu->current - 1;
            if (menu->current < 0) {
                menu->current = 0;
            } else if (firstSelected) {
                menu->offset++;
            }
            draw();
        }
        if (control_pageDown()) {
            menu->current = menu->current + 1;
            if (menu->current >= menu->pointsCount) {
                menu->current = menu->pointsCount - 1;
            } else if (lastSelected) {
                menu->offset--;
            }
            draw();
        }
        if (menu->rightLeftControl) {
            if (control_isMoveButtonPressed(CONTROL_LEFT)) {
                menu->rightLeftState = CONTROL_LEFT;
                return menu->current;
            }
            if (control_isMoveButtonPressed(CONTROL_RIGHT)) {
                menu->rightLeftState = CONTROL_RIGHT;
                return menu->current;
            }
        }

        yield();
    }
}

bool gui_yesno(const char* title) {
    const char* strs[] = {"no", "yes"};
    struct menuState menu = {
        .title = title,
        .pointsCount = C_SIZE(strs),
        .points = strs
    };

    while (true) {
        gui_menu(&menu);
        switch (menu.current) {
            case 0:
                return false;
            case 1:
                return true;
        }
    }
}

int gui_selectNumber(const char* title, int from, int to) {
    int count = (to - from) + 1;
    char* array[count];
    int index = 0;
    for (int i = from; i <= to; i++) {
        char* str = malloc(8);
        str[7] = '\n';
        itoa(i, str, 10);
        array[index++] = str;
    }

    struct menuState menu = {
        .title = title,
        .pointsCount = count,
        .points = array
    };

    int selected = gui_menu(&menu);
    for (int i = 0; i < count; i++) {
        free(array[i]);
    }
    return selected + from;
}

bool gui_exitQuestion() {
    return gui_yesno("exit?");
}

int gui_drawStatusBar(const char* text) {
    int fontY = graphic_getFontSizeY();
    int lineY = fontY + 2;
    graphic_fillRect(0, 0, graphic_x(), lineY, color_bmselect(palette_title_bg));
    graphic_drawText(1, 1, text, color_wmselect(palette_title_text));
    graphic_line(0, lineY, graphic_x() - 1, lineY, color_wmselect(palette_title_line));
    return lineY + 1;
}

int gui_drawScoreBar(int score) {
    char scoreStr[16];
    strcpy(scoreStr, "score:");
    char numStr[16];
    itoa(score, numStr, 10);
    strcat(scoreStr, numStr);
    return gui_drawStatusBar(scoreStr);
}

int gui_getStatusBarPosY() {
    return graphic_getFontSizeY() + 3;
}

void gui_getFileImage(char* dst, const char* path) {
    if (filesystem_isDirectory(path)) {
        strcpy(dst, "/storage/expimg/_dir.bmp");
    } else {
        char exp[FILESYSTEM_EXP_LEN] = {0};
        int8_t expLen = filesystem_expansion(exp, path);

        xstr str = xstr_new();
        xstr_minsize(&str, FILESYSTEM_PATH_LEN);
        xstr_fill(&str, "/storage/expimg/%s.bmp", exp);
        strcpy(dst, str.ptr);
        xstr_del(&str);

        if (!filesystem_exists(dst)) {
            strcpy(dst, "/storage/expimg/_file.bmp");
        }
    }
}

int16_t gui_sliderWithCallback(const char* title, uint8_t defaultVal, void(*callback)(int16_t)) {
    uint8_t margin = graphic_x() / 16;
    uint16_t sx = graphic_x() - (margin * 2);
    uint16_t sy = nRound(graphic_y() / 1.5);
    uint16_t x = margin;
    uint16_t y = nRound((graphic_y() / 2.0) - (sy / 2.0));
    if (title != NULL) {
        uint16_t newY = graphic_y() - sy - margin - 1;
        if (newY > y) y = newY;
    }

    graphic_clear(color_bmselect(palette_slider_bg));
    graphic_drawCenterTextLine(0, 1, graphic_x(), title, color_wmselect(palette_slider_text));

    int16_t val = defaultVal;

    void redraw() {
        graphic_fillRect(x, y, sx, sy, color_bmselect(palette_slider_base));
        graphic_fillRect(x, y, nRound(sx * (val / 255.0)), sy, color_wmselect(palette_slider_fill));
        graphic_drawRect(x, y, sx, sy, color_wmselect(palette_slider_frame));
        graphic_update();
    }
    redraw();

    callback(defaultVal);
    while (true) {
        control_begin();
        if (control_needExitWithoutGui()) break;
        if (control_isEnterPressed()) {
            callback(-2);
            return val;
        }
        if (control_pageLeftState() > 0 && val > 0) {
            val -= 1;
            if (val < 0) val = 0;
            callback(val);
            redraw();
        }
        if (control_pageRightState() > 0 && val < 255) {
            val += 1;
            if (val > 255) val = 255;
            callback(val);
            redraw();
        }
        
        yield();
    }
    callback(-1);
    return -1;
}

void nullCallback(int16_t var) {}
int16_t gui_slider(const char* title, uint8_t defaultVal) {
    return gui_sliderWithCallback(title, defaultVal, nullCallback);
}

// ---- advanced menu

static void _menu_addCallback(struct tabMenuState* menu, const char* title, const char* img, void(*callback)(), uint8_t info, void* data) {
    if (menu->pointsCount > 0) {
        uint8_t count = menu->pointsCount + 1;
        menu->points = realloc(menu->points, count * sizeof(*menu->points));
        menu->imgs = realloc(menu->imgs, count * sizeof(*menu->imgs));
        menu->callbacks = realloc(menu->callbacks, count * sizeof(*menu->callbacks));
        menu->callbacksInfo = realloc(menu->callbacksInfo, count * sizeof(*menu->callbacksInfo));
        menu->callbacksData = realloc(menu->callbacksData, count * sizeof(*menu->callbacksData));
    }
    menu->points[menu->pointsCount] = title;
    menu->imgs[menu->pointsCount] = img;
    menu->callbacks[menu->pointsCount] = callback;
    menu->callbacksInfo[menu->pointsCount] = info;
    menu->callbacksData[menu->pointsCount] = data;
    menu->pointsCount++;
}





void gui_menu_init(struct tabMenuState* menu, const char* title) {
    menu->points = malloc(1 * sizeof(*menu->points));
    menu->imgs = malloc(1 * sizeof(*menu->imgs));
    menu->callbacks = malloc(1 * sizeof(*menu->callbacks));
    menu->callbacksInfo = malloc(1 * sizeof(*menu->callbacksInfo));
    menu->callbacksData = malloc(1 * sizeof(*menu->callbacksData));
    menu->pointsCount = 0;
    menu->title = title;
}

struct tabMenuState gui_menu_new(const char* title) {
    struct tabMenuState menu;
    gui_menu_init(&menu, title);
    return menu;
}

void gui_menu_addCallback(struct tabMenuState* menu, const char* title, const char* img, void(*callback)()) {
    _menu_addCallback(menu, title, img, callback, 0, NULL);
}

void gui_menu_addApp(struct tabMenuState* menu, const char* title, const char* img, void(*callback)()) {
    _menu_addCallback(menu, title, img, callback, 1, NULL);
}

void gui_menu_addSlider(struct tabMenuState* menu, const char* title, const char* img, void(*callback)(int16_t), uint8_t* defaultVal) {
    _menu_addCallback(menu, title, img, callback, 2, defaultVal);
}

struct tabMenuState* gui_menu_addTab(struct tabMenuState* menu, const char* title, const char* img) {
    struct tabMenuState* tab = malloc(sizeof(struct tabMenuState));
    gui_menu_init(tab, title);
    _menu_addCallback(menu, title, img, NULL, 3, tab);
    return tab;
}

void gui_menu_addExit(struct tabMenuState* menu, const char* title, const char* img) {
    if (title == NULL) title = "< back";
    _menu_addCallback(menu, title, img, NULL, 4, NULL);
}

void gui_menu_run(struct tabMenuState* menu) {
    struct menuState localMenu = {
        .title = menu->title,
        .pointsCount = menu->pointsCount,
        .points = menu->points,
        .imgs = menu->imgs
    };

    while (true) {
        gui_menu(&localMenu);
        uint8_t pos = localMenu.current;
        void* callback = menu->callbacks[pos];
        switch (menu->callbacksInfo[pos]) {
            case 0:
                ((void(*)())callback)();
                break;
            case 1:
                system_runApp((void(*)())callback);
                break;
            case 2:
                gui_sliderWithCallback(menu->points[pos], *((uint8_t*)menu->callbacksData[pos]), (void(*)(int16_t))callback);
                break;
            case 3:
                gui_menu_run((struct tabMenuState*)menu->callbacksData[pos]);
                break;
            case 4:
                return;
        }
        
    }
}

void gui_menu_runOnce(struct tabMenuState* menu) {
    gui_menu_run(menu);
    gui_menu_free(menu);
}

void gui_menu_free(struct tabMenuState* menu) {
    free(menu->points);
    free(menu->imgs);
    free(menu->callbacks);
    free(menu->callbacksInfo);
    free(menu->callbacksData);
    for (uint8_t i = 0; i < menu->pointsCount; i++) {
        if (menu->callbacksInfo[i] == 3) {
            gui_menu_free((struct tabMenuState*)menu->callbacksData[i]);
        }
    }
}