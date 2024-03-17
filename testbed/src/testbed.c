#include "testbed.h"

#include "fracture/core/containers/darrays.h"
#include "fracture/core/systems/event.h"
#include "fracture/core/systems/input.h"

#define TEST_LEN 10000000

typedef struct testbed_internal_state {
    i32 test;
    i32 postition[4];
    f32 rotation[4];
    f32 scale[4];
    mat4 matrices[TEST_LEN];
    mat4 out_matrix[TEST_LEN];
    vec4 vectors[TEST_LEN];
    vec4 out_vector[TEST_LEN];
    fr_rng_config pcg_state;
    fr_rng_config xorwow_state;
} testbed_internal_state;

static testbed_internal_state* state = NULL_PTR;
static u32* transform_array = NULL_PTR;

b8 testbed_on_key_pressed(u16 event_code, void* sender, void* listener_instance, event_data data);

b8 testbed_on_key_B(u16 event_code, void* sender, void* listener_instance, event_data data);
b8 testbed_on_mouse_button1(u16 event_code, void* sender, void* listener_instance, event_data data);

b8 testbed_initialize(application_handle *app_handle) {
    clock c;
    fr_clock_start(&c);
    // FR_INFO("Client Application initialized: %s", app_handle->app_config.name);
    state = fr_memory_allocate(sizeof(testbed_internal_state), MEMORY_TYPE_APPLICATION);
    u32 memory_requirement = 0;

    fr_random_pcg32_init(0, NULL_PTR, &memory_requirement);
    state->pcg_state.state = fr_memory_allocate(memory_requirement, MEMORY_TYPE_APPLICATION);
    state->pcg_state.type = FR_RNG_PCG32;
    fr_random_pcg32_init(c.start_time, &state->pcg_state, NULL_PTR);

    fr_random_xorwow_init(0, NULL_PTR, &memory_requirement);
    state->xorwow_state.state = fr_memory_allocate(memory_requirement, MEMORY_TYPE_APPLICATION);
    state->xorwow_state.type = FR_RNG_XORWOW;
    fr_random_xorwow_init(c.start_time, &state->xorwow_state, NULL_PTR);

    // // Event test
    // fr_event_register_handler(EVENT_CODE_KEY_PRESS, NULL_PTR, testbed_on_key_pressed);
    // fr_event_register_handler(EVENT_CODE_KEY_RELEASE, NULL_PTR, testbed_on_key_pressed);
    // fr_event_register_handler(EVENT_CODE_KEY_B, NULL_PTR, testbed_on_key_B);
    // fr_event_register_handler(EVENT_CODE_MOUSE_BUTTON_LEFT, NULL_PTR, testbed_on_mouse_button1);
    // Test execution time of vec3_veqv_simd and vec4_veqv
    
    // mat3 print test
    fr_clock_start(&c);
    for (u32 i = 0; i < TEST_LEN; i++) {
        fr_mat4_random_uniform(&state->matrices[i], &state->xorwow_state);
    }
    fr_clock_update(&c);
    FR_TRACE("Time taken to generate TEST_LEN mat3s: %f", c.elapsed_time * 1000.0f);

    fr_clock_start(&c);
    for (u32 i = 0; i < TEST_LEN; i++) {
        fr_vec4_random_uniform(&state->vectors[i], &state->xorwow_state);
    }
    fr_clock_update(&c);
    FR_TRACE("Time taken to generate TEST_LEN vec3s: %f", c.elapsed_time * 1000.0f);

    fr_clock_start(&c);
    for (u32 i = 0; i < TEST_LEN; i++) {
        fr_vec4_random_uniform(&state->out_vector[i], &state->xorwow_state);
    }
    fr_clock_update(&c);
    FR_TRACE("Time taken to generate TEST_LEN vec3s using temp: %f", c.elapsed_time * 1000.0f);

    vec4 v1 = {1.0f, 2.0f, 3.0f, 4.0f};

    f32 values[16] = {1.0f, 1.0f, 2.0f,  -3.0f,  4.0f,  5.0f,  6.0f,  7.0f,
                      8.0f, -9.0f, 10.0f, -11.0f, 12.0f, 13.0f, 14.0f, 15.0f};
    char buffer[1024];
    i32 len = 1024;
    fr_mat4_print("rotation1", &len, &state->matrices[0], buffer);
    FR_TRACE("Rotation1: %s", buffer);

    fr_clock_start(&c);
    for (u32 i = 0; i < TEST_LEN; i++) {
        fr_mat4_inv(&state->matrices[i], &state->matrices[i]);
    }
    fr_clock_update(&c);
    FR_TRACE("Time taken to copy TEST_LEN mat3s: %f", c.elapsed_time * 1000.0f);

    fr_clock_start(&c);
    for (u32 i = 0; i < TEST_LEN; i++) {
        fr_mat4_random_uniform(&state->matrices[i], &state->xorwow_state);
        // fr_mat4_random_uniform_xorwow(&state->matrices[i], &state->xorwow_state, -1.0f, 1.0f);
    }
    fr_clock_update(&c);
    FR_TRACE("Time taken to create TEST_LEN mat3s: %f", c.elapsed_time * 1000.0f);

    mat4 identity;
    fr_clock_start(&c);
    for (u32 i = 0; i < TEST_LEN; i++) {
        fr_mat4_inv(&state->out_matrix[i], &state->out_matrix[i]);
    }
    fr_clock_update(&c);
    FR_TRACE("Time taken to copy TEST_LEN mat3s using temp: %f", c.elapsed_time * 1000.0f);

    fr_mat4_print("rotation2", &len, &state->matrices[0], buffer);
    FR_TRACE("Rotation2: %s", buffer);
    return TRUE;
}

b8 testbed_shutdown(application_handle *app_handle) {
    FR_INFO("Client Application shutdown: %s", app_handle->app_config.name);
    fr_memory_free(state, sizeof(testbed_internal_state), MEMORY_TYPE_APPLICATION);
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
