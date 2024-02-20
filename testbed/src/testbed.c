#include "testbed.h"

#include "fracture/core/containers/darrays.h"
#include "fracture/core/systems/event.h"
#include "fracture/core/systems/input.h"

typedef struct testbed_internal_state {
    i32 test;
    i32 postition[4];
    f32 rotation[4];
    f32 scale[4];
} testbed_internal_state;

static testbed_internal_state* state = NULL_PTR;
static u32* transform_array = NULL_PTR;

b8 testbed_on_key_pressed(u16 event_code, void* sender, void* listener_instance, event_data data);

b8 testbed_on_key_B(u16 event_code, void* sender, void* listener_instance, event_data data);
b8 testbed_on_mouse_button1(u16 event_code, void* sender, void* listener_instance, event_data data);

b8 testbed_initialize(application_handle *app_handle) {
    FR_INFO("Client Application initialized: %s", app_handle->app_config.name);
    state = fr_memory_allocate(sizeof(testbed_internal_state), MEMORY_TYPE_APPLICATION);
    transform_array = fr_memory_allocate(4 * 4 * sizeof(u32), MEMORY_TYPE_TRANSFORM);

    // Event test
    fr_event_register_handler(EVENT_CODE_KEY_PRESS, NULL_PTR, testbed_on_key_pressed);
    fr_event_register_handler(EVENT_CODE_KEY_RELEASE, NULL_PTR, testbed_on_key_pressed);
    fr_event_register_handler(EVENT_CODE_KEY_B, NULL_PTR, testbed_on_key_B);
    fr_event_register_handler(EVENT_CODE_MOUSE_BUTTON_LEFT, NULL_PTR, testbed_on_mouse_button1);

    return TRUE;
}

b8 testbed_shutdown(application_handle *app_handle) {
    FR_INFO("Client Application shutdown: %s", app_handle->app_config.name);
    fr_memory_free(state, sizeof(testbed_internal_state), MEMORY_TYPE_APPLICATION);
    fr_memory_free(transform_array, 4 * 4 * sizeof(u32), MEMORY_TYPE_TRANSFORM);
    fr_event_deregister_handler(EVENT_CODE_KEY_PRESS, NULL_PTR, testbed_on_key_pressed);
    fr_event_deregister_handler(EVENT_CODE_KEY_RELEASE, NULL_PTR, testbed_on_key_pressed);
    fr_event_deregister_handler(EVENT_CODE_KEY_B, NULL_PTR, testbed_on_key_B);
    fr_event_deregister_handler(EVENT_CODE_MOUSE_BUTTON_LEFT, NULL_PTR, testbed_on_mouse_button1);
    return TRUE;
}

b8 testbed_update(application_handle *app_handle, f64 delta_time) {

    if (fr_input_is_key_pressed_this_frame(KEY_G)) {
        FR_INFO("G key was pressed this frame");
    }

    if (fr_input_is_key_released_this_frame(KEY_G)) {
        FR_INFO("G key was released this frame");
    }

    return TRUE;
}

b8 testbed_render(application_handle *app_handle, f64 delta_time) {
    return TRUE;
}

b8 testbed_on_resize(application_handle *app_handle, u32 width, u32 height) {
    return TRUE;
}

b8 testbed_on_key_pressed(u16 event_code, void* sender, void* listener_instance, event_data data) {
    if (event_code == EVENT_CODE_KEY_PRESS) {
        keys key = (keys)data.data.du16[0];
        b8 is_repeated = (b8)data.data.du16[1];
        i16 mouse_x = (i16)data.data.du16[2];
        i16 mouse_y = (i16)data.data.du16[3];
        switch (key) {
            case KEY_T:
                FR_INFO(
                    "T key pressed at position: (%d, %d) and is %s repeated",
                    mouse_x, mouse_y, is_repeated ? "" : "not");
                return FALSE;
            case KEY_R:
                FR_INFO("R key was pressed this frame %d", fr_input_is_key_pressed_this_frame(KEY_R));
                FR_INFO("R key pressed at position: (%d, %d)", mouse_x, mouse_y);
                return FALSE;
        case KEY_LCONTROL:
        case KEY_RCONTROL:
            FR_INFO("Ctrl key pressed at position: (%d, %d)", mouse_x, mouse_y);
            default:
                break;
        }
    } else if (event_code == EVENT_CODE_KEY_RELEASE) {
        keys key = (keys)data.data.du16[0];
        i16 mouse_x = (i16)data.data.du16[1];
        i16 mouse_y = (i16)data.data.du16[2];
        switch (key) {
            case KEY_T:
                FR_INFO("T key released at position: (%d, %d)", mouse_x, mouse_y);
                return FALSE;
            default:
                break;
        }
    }
    return TRUE;
}

b8 testbed_on_key_B(u16 event_code, void* sender, void* listener_instance, event_data data) {
    b8 is_repeated = (b8)data.data.du16[1];
    i16 mouse_x = (i16)data.data.du16[2];
    i16 mouse_y = (i16)data.data.du16[3];
    b8 pressed = (b8)data.data.du16[4];
    if(pressed) {
        FR_INFO("B key pressed at position: (%d, %d) and is %s repeated", mouse_x, mouse_y, is_repeated ? "" : "not");
    } else {
        FR_INFO("B key released at position: (%d, %d)", mouse_x, mouse_y);
    }
    return TRUE;
}

b8 testbed_on_mouse_button1(u16 event_code, void* sender, void* listener_instance, event_data data) {
    i16 mouse_x = (i16)data.data.du16[2];
    i16 mouse_y = (i16)data.data.du16[3];
    b8 pressed = (b8)data.data.du16[4];
    if(pressed) {
        FR_INFO("Mouse button 1 pressed at position: (%d, %d)", mouse_x, mouse_y);
    } else {
        FR_INFO("Mouse button 1 released at position: (%d, %d)", mouse_x, mouse_y);
    }
    return TRUE;
}
