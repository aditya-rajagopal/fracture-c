#include "testbed.h"

#include "fracture/core/library/math/vec3.h"
#include "fracture/core/library/random/fr_random.h"
#include "fracture/core/systems/clock.h"
#include "fracture/core/systems/event.h"
#include "fracture/core/systems/input.h"

#define TEST_LEN 10000000

typedef struct testbed_internal_state {
    i32 test;
    vec3 vec3s[TEST_LEN];
    vec3 dots[TEST_LEN];
    vec3 dots2[TEST_LEN];
    quat quaternions[TEST_LEN];
    fr_rng_config rng_state;
} testbed_internal_state;

static testbed_internal_state* state = NULL_PTR;
static u32* transform_array = NULL_PTR;

b8 testbed_on_key_pressed(u16 event_code, void* sender, void* listener_instance, event_data data);

b8 testbed_on_key_B(u16 event_code, void* sender, void* listener_instance, event_data data);
b8 testbed_on_mouse_button1(u16 event_code, void* sender, void* listener_instance, event_data data);

b8 testbed_initialize(application_handle* app_handle) {
    testbed_state* app_state = (testbed_state*)app_handle->application_data;
    app_state->is_running = TRUE;
    state = fr_memory_allocate(sizeof(testbed_internal_state), MEMORY_TYPE_APPLICATION);

    // Event test
    fr_event_register_handler(EVENT_CODE_KEY_PRESS, NULL_PTR, testbed_on_key_pressed);
    fr_event_register_handler(EVENT_CODE_KEY_RELEASE, NULL_PTR, testbed_on_key_pressed);
    fr_event_register_handler(EVENT_CODE_KEY_B, NULL_PTR, testbed_on_key_B);
    fr_event_register_handler(EVENT_CODE_MOUSE_BUTTON_LEFT, NULL_PTR, testbed_on_mouse_button1);

    f64 current_time = fr_clock_get_absolute_time_s();
    u32 memory_requirement = 0;
    fr_random_xorwow_init(0, NULL_PTR, &memory_requirement);
    state->rng_state.state = fr_memory_allocate(memory_requirement, MEMORY_TYPE_APPLICATION);
    state->rng_state.type = FR_RANDOM_DEFAULT;
    fr_random_xorwow_init(current_time, &state->rng_state, NULL_PTR);

    // mat3 print test
    clock c;
    for (u32 i = 0; i < TEST_LEN; i++) {
        fr_vec3(fr_random_uniform(&state->rng_state),
                fr_random_uniform(&state->rng_state),
                fr_random_uniform(&state->rng_state),
                &state->vec3s[i]);
        // fr_quat(fr_random_uniform(&state->rng_state),
        //         fr_random_uniform(&state->rng_state),
        //         fr_random_uniform(&state->rng_state),
        //         fr_random_uniform(&state->rng_state),
        //         &state->quaternions[i]);
    }

    fr_clock_start(&c);
    for (u32 i = 0; i < TEST_LEN; i++) {
        // fr_quat_vec3_rot(&state->vec3s[i], &state->quaternions[i], &state->vec3s[i]);
        fr_vec3_cross(&state->vec3s[i], &state->vec3s[i], &state->dots[i]);
    }
    fr_clock_update(&c);
    FR_INFO("Time to rotate %d vec3s with quaternions: %f", TEST_LEN, fr_clock_get_elapsed_time_s(&c));

    for (u32 i = 0; i < TEST_LEN; i++) {
        fr_vec3(fr_random_uniform(&state->rng_state),
                fr_random_uniform(&state->rng_state),
                fr_random_uniform(&state->rng_state),
                &state->vec3s[i]);
    }

    fr_clock_start(&c);
    for (u32 i = 0; i < TEST_LEN; i++) {
        // fr_quat_vec3_rot(&state->vec3s[i], &state->quaternions[i], &state->vec3s[i]);
        fr_vec3_cross(&state->vec3s[i], &state->vec3s[i], &state->dots2[i]);
    }
    fr_clock_update(&c);
    FR_INFO("Time to rotate %d vec3s SIMD with quaternions: %f", TEST_LEN, fr_clock_get_elapsed_time_s(&c));

    // quaternion unit test
    vec3 point = {1.0f, 2.0f, 3.0f};
    quat q;
    fr_quat_from_eulers321(PI_2, 0.0f, 0.0f, &q);
    fr_quat_vec3_rot(&point, &q, &point);
    FR_INFO("Rotated point: (%f, %f, %f)", point.x, point.y, point.z);
    FR_INFO("Client Application initialized: %s", app_handle->app_config.name);
    return TRUE;
}

b8 testbed_shutdown(application_handle* app_handle) {
    testbed_state* app_state = (testbed_state*)app_handle->application_data;
    app_state->is_running = FALSE;
    fr_memory_free(state, sizeof(testbed_internal_state), MEMORY_TYPE_APPLICATION);
    fr_event_deregister_handler(EVENT_CODE_KEY_PRESS, NULL_PTR, testbed_on_key_pressed);
    fr_event_deregister_handler(EVENT_CODE_KEY_RELEASE, NULL_PTR, testbed_on_key_pressed);
    fr_event_deregister_handler(EVENT_CODE_KEY_B, NULL_PTR, testbed_on_key_B);
    fr_event_deregister_handler(EVENT_CODE_MOUSE_BUTTON_LEFT, NULL_PTR, testbed_on_mouse_button1);

    FR_INFO("Client Application shutdown: %s", app_handle->app_config.name);
    return TRUE;
}

b8 testbed_update(application_handle* app_handle, f64 delta_time) {
    if (fr_input_is_key_pressed_this_frame(KEY_G)) {
        FR_INFO("G key was pressed this frame");
    }

    if (fr_input_is_key_released_this_frame(KEY_G)) {
        FR_INFO("G key was released this frame");
    }

    return TRUE;
}

b8 testbed_render(application_handle* app_handle, f64 delta_time) { return TRUE; }

b8 testbed_on_resize(application_handle* app_handle, u32 width, u32 height) { return TRUE; }

b8 testbed_on_key_pressed(u16 event_code, void* sender, void* listener_instance, event_data data) {
    if (event_code == EVENT_CODE_KEY_PRESS) {
        keys key = (keys)data.data.du16[0];
        b8 is_repeated = (b8)data.data.du16[1];
        i16 mouse_x = (i16)data.data.du16[2];
        i16 mouse_y = (i16)data.data.du16[3];
        switch (key) {
            case KEY_T:
                FR_INFO("T key pressed at position: (%d, %d) and is %s repeated",
                        mouse_x,
                        mouse_y,
                        is_repeated ? "" : "not");
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
    if (pressed) {
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
    if (pressed) {
        FR_INFO("Mouse button 1 pressed at position: (%d, %d)", mouse_x, mouse_y);
    } else {
        FR_INFO("Mouse button 1 released at position: (%d, %d)", mouse_x, mouse_y);
    }
    return TRUE;
}
