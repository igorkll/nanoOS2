#define CONTROL_COUNT 6
typedef enum {
    CONTROL_UP,
    CONTROL_RIGHT,
    CONTROL_DOWN,
    CONTROL_LEFT,

    CONTROL_ENTER,
    CONTROL_ESC
} control_key;

void control_begin();
bool control_rawGet(control_key key);
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
bool control_waitExitOrEnter();
bool control_needExitOrEnter();

int8_t control_pageUpState();
int8_t control_pageDownState();
int8_t control_pageLeftState();
int8_t control_pageRightState();
bool control_pageUp();
bool control_pageDown();
bool control_pageLeft();
bool control_pageRight();