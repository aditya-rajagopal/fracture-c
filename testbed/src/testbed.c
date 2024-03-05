#include "testbed.h"
#include <xmmintrin.h>

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
    // FR_INFO("Client Application initialized: %s", app_handle->app_config.name);
    // state = fr_memory_allocate(sizeof(testbed_internal_state), MEMORY_TYPE_APPLICATION);
    // transform_array = fr_memory_allocate(4 * 4 * sizeof(u32), MEMORY_TYPE_TRANSFORM);

    // // Event test
    // fr_event_register_handler(EVENT_CODE_KEY_PRESS, NULL_PTR, testbed_on_key_pressed);
    // fr_event_register_handler(EVENT_CODE_KEY_RELEASE, NULL_PTR, testbed_on_key_pressed);
    // fr_event_register_handler(EVENT_CODE_KEY_B, NULL_PTR, testbed_on_key_B);
    // fr_event_register_handler(EVENT_CODE_MOUSE_BUTTON_LEFT, NULL_PTR, testbed_on_mouse_button1);
    // Test execution time of vec4_veqv_simd and vec4_veqv
    vec4 v0, v1, v3, result;
    bvec4 bresult;
    f32 norm = 0.0f;
    f32 res = 0.0f;
    fr_vec4(fr_random(), fr_random(), fr_random(), fr_random(), &v0);
    FR_TRACE("v0 %f %f %f %f", v0.x, v0.y, v0.z, v0.w);
    fr_vec4(fr_random(), fr_random(), fr_random(), fr_random(), &v1);
    fr_vec4(1.0f, 2.0f, 4.0f, 4.0f, &v3);
    clock c;
    fr_clock_start(&c);
    for (u32 i = 0; i < 10000000; i++) {
        res = fr_vec4_dot(&v0, &v1);
        // norm = fr_vec4_norm_simd(&v0);
        fr_vec4_s(fr_random(), &v0);
    }
    fr_clock_update(&c);
    FR_INFO("v0 %f %f %f %f", v0.x, v0.y, v0.z, v0.w);
    FR_INFO("v0 norm: %f", res);
    FR_INFO("vec4_veqv_simd took: %f", c.elapsed_time * 1000.0f);
    fr_vec4(fr_random(), fr_random(), fr_random(), fr_random(), &v0);
    fr_vec4(fr_random(), fr_random(), fr_random(), fr_random(), &v1);
    fr_clock_start(&c);
    for (u32 i = 0; i < 10000000; i++) {
        res = fr_vec4_dot(&v0, &v1);
        // norm = fr_vec4_norm(&v0);
        fr_vec4_s(fr_random(), &v0);
    }
    fr_clock_update(&c);
    FR_INFO("vec4_veqv took: %f", c.elapsed_time * 1000.0f);
    FR_INFO("final v0 %f %f %f %f", v0.x, v0.y, v0.z, v0.w);
    FR_INFO("v0 norm: %f", res);

    // unit test for norm
    fr_vec4(1.0f, 2.0f, 3.0f, 4.0f, &v0);
    norm = fr_vec4_norm(&v0);
    FR_INFO("norm of v0: %f", norm);
    // random vec
    fr_vec4(fr_random(), fr_random(), fr_random(), fr_random(), &v0);
    norm = fr_vec4_norm(&v0);
    FR_INFO("v0 %f %f %f %f", v0.x, v0.y, v0.z, v0.w);
    FR_INFO("norm of v0: %f", norm + fr_random());
    
    return TRUE;
}

b8 testbed_shutdown(application_handle *app_handle) {
    FR_INFO("Client Application shutdown: %s", app_handle->app_config.name);
    // fr_memory_free(state, sizeof(testbed_internal_state), MEMORY_TYPE_APPLICATION);
    // fr_memory_free(transform_array, 4 * 4 * sizeof(u32), MEMORY_TYPE_TRANSFORM);
    // fr_event_deregister_handler(EVENT_CODE_KEY_PRESS, NULL_PTR, testbed_on_key_pressed);
    // fr_event_deregister_handler(EVENT_CODE_KEY_RELEASE, NULL_PTR, testbed_on_key_pressed);
    // fr_event_deregister_handler(EVENT_CODE_KEY_B, NULL_PTR, testbed_on_key_B);
    // fr_event_deregister_handler(EVENT_CODE_MOUSE_BUTTON_LEFT, NULL_PTR, testbed_on_mouse_button1);

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
