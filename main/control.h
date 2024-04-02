typedef enum {
    CONTROL_UP,
    CONTROL_RIGHT,
    CONTROL_DOWN,
    CONTROL_LEFT
} control_arrow;


bool control_isEnter();
bool control_isEnterPressed();
bool control_isMoveButton(int index);
bool control_isMoveButtonPressed(int index);
void control_waitEnter();