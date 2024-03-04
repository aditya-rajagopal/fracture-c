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

    // Event test
    // fr_event_register_handler(EVENT_CODE_KEY_PRESS, NULL_PTR, testbed_on_key_pressed);
    // fr_event_register_handler(EVENT_CODE_KEY_RELEASE, NULL_PTR, testbed_on_key_pressed);
    // fr_event_register_handler(EVENT_CODE_KEY_B, NULL_PTR, testbed_on_key_B);
    // fr_event_register_handler(EVENT_CODE_MOUSE_BUTTON_LEFT, NULL_PTR, testbed_on_mouse_button1);

    vec3 test, out;
    fr_vec3_new_to(fr_random(), 2.0f, 3.0f, &test);
    FR_CORE_TRACE("test: (%f, %f, %f)", test.x, test.y, test.z);
    fr_vec3_new_to(4.0f, 5.0f, fr_random(), &out);
    fr_vec3_make_sqrt(&test);
    fr_vec3_make_sqrt(&out);
    FR_CORE_TRACE("test: (%f, %f, %f)", test.x, test.y, test.z);
    // vec3 norm;
    // // fr_vec3_make_zero(&norm);
    // fr_vec3_cross(&test, &out, &norm);
    // FR_CORE_TRACE("Cross product of (%f, %f, %f) and (%f, %f, %f) is (%f, %f, %f)", test.x, test.y, test.z, out.x, out.y, out.z, norm.x, norm.y, norm.z);
    // Test the 2 variations of vec3_norm : fr_vec3_norm and fr_vec3_normp
    // clock timer;
    // vec3 test, out, norm;
    // fr_clock_start(&timer);
    // f32 random = fr_random();
    // // fr_vec3_new_to(-1.0f, 2.0f, 6.4f, &test);
    // fr_vec3_new_to(fr_random(), fr_random(), fr_random(), &out);
    // fr_vec3_new_to(fr_random(), fr_random(), fr_random(), &test);
    // // vec3 norm;
    // // f32 output = fr_vec3_norm(&test);
    // for (u32 i = 0; i < 10000000; i++) {
    //     fr_vec3_new_to(fr_random(), fr_random(), fr_random(), &out);
    //     fr_vec3_cross_simd(&out, &test, &norm);
    // }
    // fr_clock_update(&timer);
    // f32 time_per_call = timer.elapsed_time;
    // FR_CORE_TRACE("fr_vec3_norm took %f ms per call", time_per_call * 1000);
    // FR_CORE_TRACE("norm: (%f, %f, %f)", norm.x, norm.y, norm.z);

    // fr_clock_start(&timer);
    // // random = fr_random();
    // // fr_vec3_new_to(fr_random(), fr_random(), fr_random(), &out);
    // for (u32 i = 0; i < 10000000; i++) {
    //     // vec3 v1;
    //     // fr_vec3_new_to(fr_random(), fr_random(), fr_random(), &test);
    //     fr_vec3_new_to(fr_random(), fr_random(), fr_random(), &out);
    //     fr_vec3_cross(&out, &test, &norm);
    // }
    // fr_clock_update(&timer);
    // time_per_call = timer.elapsed_time;
    // FR_CORE_TRACE("fr_vec3_sign_glm took %f ms per call", time_per_call * 1000);
    // FR_CORE_TRACE("norm: (%f, %f, %f)", norm.x, norm.y, norm.z);
    // fr_vec3_new_to(1.0f, -2.0f, 3.0f, &test);
    // fr_vec3_new_to(4.0f, 0.0f, -6.0f, &out);
    // vec3 norm;
    // fr_vec3_cross(&test, &out, &norm);
    // FR_CORE_TRACE("Cross product of (%f, %f, %f) and (%f, %f, %f) is (%f, %f, %f)", test.x, test.y, test.z, out.x, out.y, out.z, norm.x, norm.y, norm.z);
    // norm = fr_vec3_get_cross(&test, &out);
    // FR_CORE_TRACE("Cross product of (%f, %f, %f) and (%f, %f, %f) is (%f, %f, %f)", test.x, test.y, test.z, out.x, out.y, out.z, norm.x, norm.y, norm.z);
    return TRUE;
}

b8 testbed_shutdown(application_handle *app_handle) {
    FR_INFO("Client Application shutdown: %s", app_handle->app_config.name);
    // fr_memory_free(state, sizeof(testbed_internal_state), MEMORY_TYPE_APPLICATION);
    // fr_memory_free(transform_array, 4 * 4 * sizeof(u32), MEMORY_TYPE_TRANSFORM);
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
