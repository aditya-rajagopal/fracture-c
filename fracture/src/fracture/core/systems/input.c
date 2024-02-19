#include "input.h"

#include "fracture/core/systems/logging.h"
#include "fracture/core/systems/event.h"
#include "fracture/core/systems/fracture_memory.h"

typedef struct keyboard_state {
    b8 keys[TOTAL_KEYS];    
} keyboard_state;

typedef struct mouse_state {
    i32 x;
    i32 y;
    b8 buttons[TOTAL_MOUSE_BUTTONS];
} mouse_state;

typedef struct input_state {
    keyboard_state keys_current_state;
    keyboard_state keys_previous_state;
    mouse_state button_current_state;
    mouse_state button_previous_state;

    b8 allow_repeats;
} input_state;

static input_state* state_ptr = NULL_PTR;

b8 fr_input_initialize() {
    if (state_ptr) {
        FR_CORE_ERROR("Input system has already been initialized");
        return FALSE;
    }
    state_ptr = NULL_PTR;

    state_ptr = (input_state*)fr_memory_allocate(sizeof(input_state), MEMORY_TYPE_SYSTEM);

    state_ptr->allow_repeats = FALSE;
    return TRUE;
}

b8 fr_input_shutdown() {
    if (!state_ptr) {
        FR_CORE_ERROR("Input system had not been initialized yet!");
        return FALSE;
    }
    fr_memory_free(state_ptr, sizeof(input_state), MEMORY_TYPE_SYSTEM);
    state_ptr = NULL_PTR;
    return TRUE;
}

void fr_input_update(f64 delta_time) {
    if (!state_ptr) {
        return;
    }

    fr_memory_copy(&state_ptr->keys_previous_state, &state_ptr->keys_current_state, sizeof(keyboard_state));
    fr_memory_copy(&state_ptr->button_previous_state, &state_ptr->button_current_state, sizeof(mouse_state));
}

void fr_input_process_keypress(keys key, b8 pressed) {
    if (!state_ptr) {
        return;
    }

    b8 is_repeated = pressed & state_ptr->keys_current_state.keys[key];
    b8 key_state_change = state_ptr->keys_current_state.keys[key] != pressed;
    if (state_ptr->allow_repeats || key_state_change) {
        state_ptr->keys_current_state.keys[key] = pressed;
        // TODO: Add information about modifiers
        event_data e;
        e.data.du16[0] = key;
        e.data.du16[1] = is_repeated ? 1 : 0;
        e.data.du16[2] = state_ptr->button_current_state.x;
        e.data.du16[3] = state_ptr->button_current_state.y;
        fr_event_dispatch(pressed ? EVENT_CODE_KEY_PRESS : EVENT_CODE_KEY_RELEASE, 0, e);
    }
}

FR_API void fr_input_set_repeats(b8 enable) {
    if (state_ptr) {
        state_ptr->allow_repeats = enable;
    }
}

FR_API b8 fr_input_is_key_down(keys key) {
    if (!state_ptr) {
        return FALSE;
    }
    return state_ptr->keys_current_state.keys[key];
}

FR_API b8 fr_input_was_key_down(keys key) {
    if (!state_ptr) {
        return FALSE;
    }
    return state_ptr->keys_previous_state.keys[key];
}

FR_API b8 fr_input_is_key_pressed_this_frame(keys key) {
    if (!state_ptr) {
        return FALSE;
    }
    return state_ptr->keys_current_state.keys[key] && !state_ptr->keys_previous_state.keys[key];
}

FR_API b8 fr_input_is_key_up(keys key) {
    if (!state_ptr) {
        return TRUE;
    }
    return !state_ptr->keys_current_state.keys[key];
}

FR_API b8 fr_input_was_key_up(keys key) {
    if (!state_ptr) {
        return TRUE;
    }
    return !state_ptr->keys_previous_state.keys[key];
}

FR_API b8 fr_input_is_key_released_this_frame(keys key) {
    if (!state_ptr) {
        return TRUE;
    }
    return !state_ptr->keys_current_state.keys[key] && state_ptr->keys_previous_state.keys[key];
}

// Mouse events

void fr_input_process_mouse_button(mouse_button button, b8 pressed) {
    if (!state_ptr) {
        return;
    }

    b8 button_state_change = state_ptr->button_current_state.buttons[button] != pressed;
    b8 is_repeated = pressed & state_ptr->button_current_state.buttons[button];
    if (button_state_change) {
        state_ptr->button_current_state.buttons[button] = pressed;

        // TODO: Add information about modifiers
        event_data e;
        e.data.du16[0] = button;
        e.data.du16[1] = is_repeated ? 1 : 0;
        e.data.du16[2] = state_ptr->button_current_state.x;
        e.data.du16[3] = state_ptr->button_current_state.y;
        fr_event_dispatch(pressed ? EVENT_CODE_MOUSE_BUTTON_PRESS : EVENT_CODE_MOUSE_BUTTON_RELEASE, 0, e);
    }

}

void fr_input_process_mouse_move(i16 x, i16 y) {
    if (!state_ptr) {
        return;
    }

    state_ptr->button_current_state.x = x;
    state_ptr->button_current_state.y = y;

    event_data e;
    e.data.du16[0] = x;
    e.data.du16[1] = y;
    fr_event_dispatch(EVENT_CODE_MOUSE_MOVE, 0, e);
}

void fr_input_process_mouse_wheel(i8 z_delta) {
    event_data e;
    e.data.du16[0] = z_delta;
    e.data.du16[1] = state_ptr->button_current_state.x;
    e.data.du16[2] = state_ptr->button_current_state.y;
    fr_event_dispatch(EVENT_CODE_MOUSE_SCROLL, 0, e);
}

FR_API b8 fr_input_is_button_down(mouse_button button) {
    if (!state_ptr) {
        return FALSE;
    }
    return state_ptr->button_current_state.buttons[button];
}

FR_API b8 fr_input_was_button_down(mouse_button button) {
    if (!state_ptr) {
        return FALSE;
    }
    return state_ptr->button_previous_state.buttons[button];
}

FR_API b8 fr_input_is_button_pressed_this_frame(mouse_button button) {
    if (!state_ptr) {
        return FALSE;
    }
    return state_ptr->button_current_state.buttons[button] && !state_ptr->button_previous_state.buttons[button];
}

FR_API b8 fr_input_is_button_up(mouse_button button) {
    if (!state_ptr) {
        return TRUE;
    }
    return !state_ptr->button_current_state.buttons[button];
}

FR_API b8 fr_input_was_button_up(mouse_button button) {
    if (!state_ptr) {
        return TRUE;
    }
    return !state_ptr->button_previous_state.buttons[button];
}

FR_API b8 fr_input_is_button_released_this_frame(mouse_button button) {
    if (!state_ptr) {
        return TRUE;
    }
    return !state_ptr->button_current_state.buttons[button] && state_ptr->button_previous_state.buttons[button];
}

FR_API void fr_input_get_current_mouse_position(i32* x, i32* y) {
    if (!state_ptr) {
        *x = 0;
        *y = 0;
        return;
    }
    *x = state_ptr->button_current_state.x;
    *y = state_ptr->button_current_state.y;
}

FR_API void fr_input_get_last_mouse_position(i32* x, i32* y) {
    if (!state_ptr) {
        *x = 0;
        *y = 0;
        return;
    }
    *x = state_ptr->button_previous_state.x;
    *y = state_ptr->button_previous_state.y;
}

FR_API void fr_input_get_delta_mouse_position(i32* delta_x, i32* delta_y) {
    if (!state_ptr) {
        *delta_x = 0;
        *delta_y = 0;
        return;
    }
    *delta_x = state_ptr->button_current_state.x - state_ptr->button_previous_state.x;
    *delta_y = state_ptr->button_current_state.y - state_ptr->button_previous_state.y;
}

const char* fr_input_key_to_string(keys key) {
    switch (key) {
        case KEY_BACKSPACE:
            return "backspace";
        case KEY_ENTER:
            return "enter";
        case KEY_TAB:
            return "tab";
        case KEY_SHIFT:
            return "shift";
        case KEY_CONTROL:
            return "ctrl";
        case KEY_PAUSE:
            return "pause";
        case KEY_CAPITAL:
            return "capslock";
        case KEY_ESCAPE:
            return "esc";

        case KEY_CONVERT:
            return "ime_convert";
        case KEY_NONCONVERT:
            return "ime_noconvert";
        case KEY_ACCEPT:
            return "ime_accept";
        case KEY_MODECHANGE:
            return "ime_modechange";

        case KEY_SPACE:
            return "space";
        case KEY_PAGEUP:
            return "pageup";
        case KEY_PAGEDOWN:
            return "pagedown";
        case KEY_END:
            return "end";
        case KEY_HOME:
            return "home";
        case KEY_LEFT:
            return "left";
        case KEY_UP:
            return "up";
        case KEY_RIGHT:
            return "right";
        case KEY_DOWN:
            return "down";
        case KEY_SELECT:
            return "select";
        case KEY_PRINT:
            return "print";
        case KEY_EXECUTE:
            return "execute";
        case KEY_PRINTSCREEN:
            return "printscreen";
        case KEY_INSERT:
            return "insert";
        case KEY_DELETE:
            return "delete";
        case KEY_HELP:
            return "help";

        case KEY_0:
            return "0";
        case KEY_1:
            return "1";
        case KEY_2:
            return "2";
        case KEY_3:
            return "3";
        case KEY_4:
            return "4";
        case KEY_5:
            return "5";
        case KEY_6:
            return "6";
        case KEY_7:
            return "7";
        case KEY_8:
            return "8";
        case KEY_9:
            return "9";

        case KEY_A:
            return "a";
        case KEY_B:
            return "b";
        case KEY_C:
            return "c";
        case KEY_D:
            return "d";
        case KEY_E:
            return "e";
        case KEY_F:
            return "f";
        case KEY_G:
            return "g";
        case KEY_H:
            return "h";
        case KEY_I:
            return "i";
        case KEY_J:
            return "j";
        case KEY_K:
            return "k";
        case KEY_L:
            return "l";
        case KEY_M:
            return "m";
        case KEY_N:
            return "n";
        case KEY_O:
            return "o";
        case KEY_P:
            return "p";
        case KEY_Q:
            return "q";
        case KEY_R:
            return "r";
        case KEY_S:
            return "s";
        case KEY_T:
            return "t";
        case KEY_U:
            return "u";
        case KEY_V:
            return "v";
        case KEY_W:
            return "w";
        case KEY_X:
            return "x";
        case KEY_Y:
            return "y";
        case KEY_Z:
            return "z";

        case KEY_LSUPER:
            return "l_super";
        case KEY_RSUPER:
            return "r_super";
        case KEY_APPS:
            return "apps";

        case KEY_SLEEP:
            return "sleep";

            // Numberpad keys
        case KEY_NUMPAD0:
            return "numpad_0";
        case KEY_NUMPAD1:
            return "numpad_1";
        case KEY_NUMPAD2:
            return "numpad_2";
        case KEY_NUMPAD3:
            return "numpad_3";
        case KEY_NUMPAD4:
            return "numpad_4";
        case KEY_NUMPAD5:
            return "numpad_5";
        case KEY_NUMPAD6:
            return "numpad_6";
        case KEY_NUMPAD7:
            return "numpad_7";
        case KEY_NUMPAD8:
            return "numpad_8";
        case KEY_NUMPAD9:
            return "numpad_9";
        case KEY_MULTIPLY:
            return "numpad_mult";
        case KEY_ADD:
            return "numpad_add";
        case KEY_SEPARATOR:
            return "numpad_sep";
        case KEY_SUBTRACT:
            return "numpad_sub";
        case KEY_DECIMAL:
            return "numpad_decimal";
        case KEY_DIVIDE:
            return "numpad_div";

        case KEY_F1:
            return "f1";
        case KEY_F2:
            return "f2";
        case KEY_F3:
            return "f3";
        case KEY_F4:
            return "f4";
        case KEY_F5:
            return "f5";
        case KEY_F6:
            return "f6";
        case KEY_F7:
            return "f7";
        case KEY_F8:
            return "f8";
        case KEY_F9:
            return "f9";
        case KEY_F10:
            return "f10";
        case KEY_F11:
            return "f11";
        case KEY_F12:
            return "f12";
        case KEY_F13:
            return "f13";
        case KEY_F14:
            return "f14";
        case KEY_F15:
            return "f15";
        case KEY_F16:
            return "f16";
        case KEY_F17:
            return "f17";
        case KEY_F18:
            return "f18";
        case KEY_F19:
            return "f19";
        case KEY_F20:
            return "f20";
        case KEY_F21:
            return "f21";
        case KEY_F22:
            return "f22";
        case KEY_F23:
            return "f23";
        case KEY_F24:
            return "f24";

        case KEY_NUMLOCK:
            return "num_lock";
        case KEY_SCROLL:
            return "scroll_lock";
        case KEY_NUMPAD_EQUAL:
            return "numpad_equal";

        case KEY_LSHIFT:
            return "l_shift";
        case KEY_RSHIFT:
            return "r_shift";
        case KEY_LCONTROL:
            return "l_ctrl";
        case KEY_RCONTROL:
            return "r_ctrl";
        case KEY_LALT:
            return "l_alt";
        case KEY_RALT:
            return "r_alt";

        case KEY_SEMICOLON:
            return ";";

        case KEY_APOSTROPHE:
            return "'";
        case KEY_EQUAL:
            return "=";
        case KEY_COMMA:
            return ",";
        case KEY_MINUS:
            return "-";
        case KEY_PERIOD:
            return ".";
        case KEY_SLASH:
            return "/";

        case KEY_GRAVE:
            return "`";

        case KEY_LBRACKET:
            return "[";
        case KEY_PIPE:
            return "\\";
        case KEY_RBRACKET:
            return "]";

        default:
            return "undefined";
    }
}