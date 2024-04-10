#include "main.h"
#include "graphic.h"
#include "color.h"
#include "gui.h"
#include "control.h"
#include "palette.h"

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

    (*menu).rightLeftState = 0;

    void draw() {
        graphic_clear(color_bmselect(palette_menu_bg));
        for (int i = 0; i < (*menu).pointsCount; i++) {
            int pos = ((fontY + 2) * (i + (*menu).offset)) + lineY + 3;
            if (i == (*menu).current) {
                graphic_fillRect(0, pos, graphic_x(), fontY + 2, color_wmselect(palette_menu_select));
                firstSelected = pos <= (lineY + fontY);
                lastSelected = pos + fontY + 2 >= (graphic_y() - fontY);
            }
            graphic_drawText(1, pos + 1, (*menu).points[i], i == (*menu).current ? color_bmselect(palette_menu_text) : color_wmselect(palette_menu_text));
        }
        gui_drawStatusBar((*menu).title);
        graphic_update();
    }
    draw();

    while (true) {
        if (control_isEnterPressed()) return (*menu).current;
        if (control_isMoveButtonPressed(0)) {
            (*menu).current = (*menu).current - 1;
            if ((*menu).current < 0) {
                (*menu).current = 0;
            } else if (firstSelected) {
                (*menu).offset++;
            }
            draw();
        }
        if (control_isMoveButtonPressed(2)) {
            (*menu).current = (*menu).current + 1;
            if ((*menu).current >= (*menu).pointsCount) {
                (*menu).current = (*menu).pointsCount - 1;
            } else if (lastSelected) {
                (*menu).offset--;
            }
            draw();
        }
        if ((*menu).rightLeftControl) {
            if (control_isMoveButtonPressed(1)) {
                (*menu).rightLeftState = 1;
                return (*menu).current;
            }
            if (control_isMoveButtonPressed(3)) {
                (*menu).rightLeftState = 2;
                return (*menu).current;
            }
        }

        yield();
    }
}

bool gui_yesno(const char* title) {
    void* sCrop = graphic_saveCrop();
    graphic_setYCloserTo(24);
    
    char* strs[] = {"no", "yes"};
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