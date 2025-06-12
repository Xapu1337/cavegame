#ifndef OOGABOOGA_INPUT_H
#define OOGABOOGA_INPUT_H

#include <stdint.h>
#include <stdbool.h>
#include "linmath.h"

typedef enum Input_Event_Kind {
    INPUT_EVENT_KEY,
    INPUT_EVENT_SCROLL,
    INPUT_EVENT_TEXT,
    INPUT_EVENT_GAMEPAD_AXIS,
} Input_Event_Kind;

typedef enum Input_Key_Code {
    KEY_UNKNOWN = 0,
    KEY_BACKSPACE = 8,
    KEY_TAB = 9,
    KEY_ENTER = 13,
    KEY_ESCAPE = 27,
    KEY_SPACEBAR = 32,
    KEY_DELETE = 127,
    KEY_ARROW_UP = 128,
    KEY_ARROW_DOWN = 129,
    KEY_ARROW_LEFT = 130,
    KEY_ARROW_RIGHT = 131,
    KEY_PAGE_UP = 132,
    KEY_PAGE_DOWN = 133,
    KEY_HOME = 134,
    KEY_END = 135,
    KEY_INSERT = 136,
    KEY_PAUSE = 137,
    KEY_SCROLL_LOCK = 138,
    KEY_ALT,
    KEY_CTRL,
    KEY_SHIFT,
    KEY_CMD,
    KEY_META = KEY_CMD,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_PRINT_SCREEN,
    GAMEPAD_DPAD_UP,
    GAMEPAD_DPAD_RIGHT,
    GAMEPAD_DPAD_DOWN,
    GAMEPAD_DPAD_LEFT,
    GAMEPAD_A,
    GAMEPAD_X,
    GAMEPAD_Y,
    GAMEPAD_B,
    GAMEPAD_START,
    GAMEPAD_BACK,
    GAMEPAD_LEFT_STICK,
    GAMEPAD_RIGHT_STICK,
    GAMEPAD_LEFT_BUMPER,
    GAMEPAD_RIGHT_BUMPER,
    GAMEPAD_LEFT_TRIGGER,
    GAMEPAD_RIGHT_TRIGGER,
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_MIDDLE,
    MOUSE_BUTTON_RIGHT,
    GAMEPAD_FIRST = GAMEPAD_START,
    GAMEPAD_LAST = GAMEPAD_RIGHT_TRIGGER,
    MOUSE_FIRST = MOUSE_BUTTON_LEFT,
    MOUSE_LAST  = MOUSE_BUTTON_RIGHT,
    INPUT_KEY_CODE_COUNT
} Input_Key_Code;

typedef enum Input_State_Flags {
    INPUT_STATE_DOWN          = 1<<0,
    INPUT_STATE_JUST_PRESSED  = 1<<1,
    INPUT_STATE_JUST_RELEASED = 1<<2,
    INPUT_STATE_REPEAT        = 1<<3,
} Input_State_Flags;

typedef struct Input_Event {
    Input_Event_Kind kind;
    Input_Key_Code key_code;
    Input_State_Flags key_state;
    int64_t gamepad_index;
    double xscroll;
    double yscroll;
    union { uint32_t utf32; char ascii; };
} Input_Event;

typedef struct Input_Frame {
    Input_Event events[10000];
    uint64_t number_of_events;
    float mouse_x;
    float mouse_y;
    Vector2 left_stick;
    Vector2 right_stick;
    float left_trigger;
    float right_trigger;
    Input_State_Flags key_states[INPUT_KEY_CODE_COUNT];
} Input_Frame;

extern Input_Frame inputFrame;

bool IsKeyDown(Input_Key_Code code);
bool is_key_up(Input_Key_Code code);
bool IsKeyJustPressed(Input_Key_Code code);
bool is_key_just_released(Input_Key_Code code);
bool consume_key_down(Input_Key_Code code);
bool consume_key_just_pressed(Input_Key_Code code);
bool consume_key_just_released(Input_Key_Code code);

#endif
