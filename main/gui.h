struct menuState {
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
bool gui_yesno(const char* title);
int gui_selectNumber(const char* title, int from, int to);
bool gui_exitQuestion();
int gui_drawStatusBar(const char* text);
int gui_drawScoreBar(int score);
int gui_getStatusBarPosY();