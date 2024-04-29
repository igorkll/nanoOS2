#define CONTROL_COUNT 6
typedef enum {
    CONTROL_UP,
    CONTROL_RIGHT,
    CONTROL_DOWN,
    CONTROL_LEFT,

    CONTROL_ENTER,
    CONTROL_ESC
} control_key;

int8_t control_get(control_key key);
bool control_isSupport(control_key key);
bool control_getState(control_key key);
bool control_isPressed(control_key key);
bool control_isPressedExtended(control_key key);
bool control_isReleased(control_key key);

bool control_isEnter();
bool control_isEnterPressed();
bool control_isMoveButton(control_key index);
bool control_isMoveButtonPressed(control_key index);

bool control_needExit();
bool control_needExitWithoutGui();
void control_waitExit();
void control_waitEnter();
void control_waitExitOrEnter();
bool control_needExitOrEnter();

bool control_pageUp();
bool control_pageDown();