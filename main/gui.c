#include "main.h"
#include "graphic.h"
#include "color.h"
#include "gui.h"
#include "control.h"

void gui_status(const char* text) {
    graphic_clear(color_black);
    graphic_drawRect(2, 2, graphic_x() - 4, graphic_y() - 4, color_white);
    graphic_drawTextBox(4, 4, graphic_x() - 8, graphic_y() - 8, text, color_white);
    graphic_update();
}

void gui_splash(const char* text) {
    int offset = graphic_getFontSizeY() + 1;

    graphic_clear(color_black);
    graphic_drawRect(2, 2, graphic_x() - 4, graphic_y() - 4 - offset, color_white);
    graphic_drawTextBox(4, 4, graphic_x() - 8, graphic_y() - 8 - offset, text, color_white);
    graphic_drawText(2, graphic_y() - (offset + 1), "Press Enter", color_white);
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
        graphic_clear(color_black);
        for (int i = 0; i < (*menu).pointsCount; i++) {
            int pos = ((fontY + 2) * (i + (*menu).offset)) + lineY + 3;
            graphic_fillRect(0, pos, graphic_x(), fontY + 2, i == (*menu).current ? color_white : color_black);
            graphic_drawText(1, pos + 1, (*menu).points[i], i == (*menu).current ? color_black : color_white);
            if (i == (*menu).current) {
                firstSelected = pos <= (lineY + fontY);
                lastSelected = pos + fontY + 2 >= (graphic_y() - fontY);
            }
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

int gui_drawStatusBar(const char* text) {
    int fontY = graphic_getFontSizeY();
    int lineY = fontY + 2;
    graphic_fillRect(0, 0, graphic_x(), lineY, color_black);
    graphic_drawText(1, 1, text, color_white);
    graphic_line(0, lineY, graphic_x() - 1, lineY, color_white);
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