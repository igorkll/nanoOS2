struct menuState
{
    const char* title;
    int pointsCount;
    char** points;
    int current;
    int offset;
    int rightLeftState;
    bool rightLeftControl;
};

void gui_status(const char* text);
void gui_splash(const char* text);
int gui_menu(struct menuState* menu);
int gui_drawStatusBar(const char* text);
int gui_drawScoreBar(int score);
int gui_getStatusBarPosY();