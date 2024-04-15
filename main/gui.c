#include "main.h"
#include "graphic.h"
#include "color.h"
#include "gui.h"
#include "control.h"
#include "palette.h"
#include "filesystem.h"

void gui_status(const char* text) {
    graphic_clear(color_bmselect(palette_splash_bg));
    graphic_drawRect(2, 2, graphic_x() - 4, graphic_y() - 4, color_wmselect(palette_splash_frame));
    graphic_drawTextBox(4, 4, graphic_x() - 8, graphic_y() - 8, text, color_wmselect(palette_splash_text));
    graphic_update();
}

void gui_splash(const char* text) {
    int offset = graphic_getFontSizeY() + 1;
    graphic_clear(color_bmselect(palette_splash_bg));
    graphic_drawRect(2, 2, graphic_x() - 4, graphic_y() - 4 - offset, color_wmselect(palette_splash_frame));
    graphic_drawTextBox(4, 4, graphic_x() - 8, graphic_y() - 8 - offset, text, color_wmselect(palette_splash_text));
    graphic_drawText(2, graphic_y() - (offset + 1), "Press Enter", color_wmselect(palette_splash_text));
    graphic_update();
    control_waitEnter();
}

int gui_menu(struct menuState* menu) {
    int fontY = graphic_getFontSizeY();
    int lineY = fontY + 2;
    bool firstSelected = false;
    bool lastSelected = false;

    menu->rightLeftState = 0;

    void draw() {
        graphic_clear(color_bmselect(palette_menu_bg));
        uint16_t textOffsetY = 0;
        for (int i = 0; i < menu->pointsCount; i++) {
            int pos = ((fontY + 2) * (i + menu->offset)) + lineY + 3 + textOffsetY;

            uint8_t textOffset = 0;
            if (menu->imgs && menu->imgs[i]) {
                tcolor* img = graphic_loadImage(menu->imgs[i]);
                graphic_draw(1, pos + 1, img);
                textOffset = img[0];
                uint8_t textYmax = graphic_getFontSizeY() - 2;
                if (img[1] > textYmax) textOffsetY += img[1] - textYmax;
                free(img);
            }
            graphic_drawText(1 + textOffset, pos + 1, menu->points[i], i == menu->current ? color_bmselect(palette_menu_text) : color_wmselect(palette_menu_text));
            
            if (i == menu->current) {
                graphic_fillRect(0, pos, graphic_x(), fontY + 2, color_wmselect(palette_menu_select));
                firstSelected = pos <= (lineY + fontY);
                lastSelected = pos + fontY + 2 >= (graphic_y() - fontY);
            }
        }
        gui_drawStatusBar(menu->title);
        graphic_update();
    }
    draw();

    while (true) {
        if (control_isEnterPressed()) return menu->current;
        if (control_isMoveButtonPressed(0)) {
            menu->current = menu->current - 1;
            if (menu->current < 0) {
                menu->current = 0;
            } else if (firstSelected) {
                menu->offset++;
            }
            draw();
        }
        if (control_isMoveButtonPressed(2)) {
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
    void* sCrop = graphic_saveCrop();
    graphic_setYCloserTo(24);
    
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
                graphic_restoreCrop(sCrop);
                return false;
            case 1:
                graphic_restoreCrop(sCrop);
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

char* gui_getFileImage(char* path) {
    if (filesystem_isDirectory(path)) {
        return "/storage/expimg/_dir.bmp";
    } else {
        return "/storage/expimg/_file.bmp";
    }
}