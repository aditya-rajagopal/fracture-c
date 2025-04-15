#include "testbed.h"

#include "fracture/core/containers/llist.h"
#include "fracture/core/library/random/fr_random.h"
#include "fracture/core/systems/clock.h"
#include "fracture/core/systems/event.h"
#include "fracture/core/systems/fracture_memory.h"
#include "fracture/core/systems/input.h"
#include "fracture/engine/application_types.h"
#include "fracture/renderer/renderer_types.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TEST_LEN 10000000

typedef struct test_node {
    struct llist_node* node;
    u32 data;
} test_node;

typedef struct testbed_internal_state {
    i32 test;
    struct llist_head* test_llist_head;
    struct llist_head* test_llist_head_2;
    fr_rng_config rng_state;
} testbed_internal_state;

static testbed_internal_state* state = NULL_PTR;
static u32* transform_array = NULL_PTR;

static inline b8 testbed_add_test_node(struct llist_head* head, u32 data);

b8 testbed_on_key_pressed(u16 event_code, void* sender, void* listener_instance, event_data data);

b8 testbed_on_key_B(u16 event_code, void* sender, void* listener_instance, event_data data);
b8 testbed_on_mouse_button1(u16 event_code, void* sender, void* listener_instance, event_data data);

b8 testbed_initialize(application_handle* app_handle) {
    testbed_state* app_state = (testbed_state*)app_handle->application_data;
    app_state->is_running = TRUE;
    state = fr_memory_allocate(sizeof(testbed_internal_state), MEMORY_TYPE_APPLICATION);

    // Event test
    fr_event_register_handler(EVENT_CODE_KEY_PRESS, app_handle, testbed_on_key_pressed);
    fr_event_register_handler(EVENT_CODE_KEY_RELEASE, app_handle, testbed_on_key_pressed);
    fr_event_register_handler(EVENT_CODE_KEY_B, NULL_PTR, testbed_on_key_B);
    fr_event_register_handler(EVENT_CODE_MOUSE_BUTTON_LEFT, NULL_PTR, testbed_on_mouse_button1);

    f64 current_time = fr_clock_get_absolute_time_s();
    u32 memory_requirement = 0;
    fr_random_xorwow_init(0, NULL_PTR, &memory_requirement);
    state->rng_state.state = fr_memory_allocate(memory_requirement, MEMORY_TYPE_APPLICATION);
    state->rng_state.type = FR_RANDOM_DEFAULT;
    fr_random_xorwow_init(current_time, &state->rng_state, NULL_PTR);

    // mat3 print test
    // llist unit test
    state->test_llist_head = fr_memory_allocate(sizeof(struct llist_head), MEMORY_TYPE_LLIST);
    state->test_llist_head_2 = fr_memory_allocate(sizeof(struct llist_head), MEMORY_TYPE_LLIST);

    testbed_add_test_node(state->test_llist_head, 5);
    testbed_add_test_node(state->test_llist_head, 10);

    for (u32 i = 0; i < 10; i++) {
        testbed_add_test_node(state->test_llist_head_2, i);
    }

    fr_llist_for_each(pos, state->test_llist_head->head) { FR_INFO("LLIST: %d", ((test_node*)pos)->data); }

    fr_llist_for_each(pos, state->test_llist_head_2->head) { FR_INFO("LLIST2: %d", ((test_node*)pos)->data); }

    fr_core_llist_push(state->test_llist_head_2->head, state->test_llist_head);

    fr_llist_for_each(pos, state->test_llist_head->head) { FR_INFO("LLIST AFTER MERGE: %d", ((test_node*)pos)->data); }

    fr_llist_for_each(pos, state->test_llist_head_2->head) {
        FR_INFO("LLIST 2 AFTER MERGE: %d", ((test_node*)pos)->data);
    }

    testbed_add_test_node(state->test_llist_head, 12);
    testbed_add_test_node(state->test_llist_head_2, 9);

    fr_llist_for_each(pos, state->test_llist_head->head) { FR_INFO("LLIST AFTER MERGE: %d", ((test_node*)pos)->data); }
    fr_llist_for_each(pos, state->test_llist_head_2->head) {
        FR_INFO("LLIST 2 AFTER MERGE: %d", ((test_node*)pos)->data);
    }

    clock clock;
    fr_clock_start(&clock);


    int width, height, channels;
    u8* output = stbi_load("test2.png", &width, &height, &channels, 4);
    fr_clock_update(&clock);
    const f64 time_seconds = fr_clock_get_elapsed_time_ms(&clock);
    if (output == NULL) {
        FR_FATAL("Failed to load image");
    }
    FR_INFO("Time to load image: %f", time_seconds);

    return TRUE;
}

b8 testbed_shutdown(application_handle* app_handle) {
    testbed_state* app_state = (testbed_state*)app_handle->application_data;
    app_state->is_running = FALSE;
    fr_memory_free(state, sizeof(testbed_internal_state), MEMORY_TYPE_APPLICATION);
    fr_event_deregister_handler(EVENT_CODE_KEY_PRESS, app_handle, testbed_on_key_pressed);
    fr_event_deregister_handler(EVENT_CODE_KEY_RELEASE, app_handle, testbed_on_key_pressed);
    fr_event_deregister_handler(EVENT_CODE_KEY_B, NULL_PTR, testbed_on_key_B);
    fr_event_deregister_handler(EVENT_CODE_MOUSE_BUTTON_LEFT, NULL_PTR, testbed_on_mouse_button1);

    FR_INFO("Client Application shutdown: %s", app_handle->app_config.name);
    return TRUE;
}

b8 testbed_update(application_handle* app_handle, f64 delta_time) {
    if (fr_input_is_key_pressed_this_frame(KEY_G)) {
        FR_INFO("G key was pressed this frame");
        FR_INFO("Current frame rate is %f", app_handle->current_frame_rate);
        FR_INFO("Current delta time is %f", delta_time);
    }

    if (fr_input_is_key_released_this_frame(KEY_G)) {
        FR_INFO("G key was released this frame");
    }

    return TRUE;
}

b8 testbed_render(application_handle* app_handle, f64 delta_time) { return TRUE; }

b8 testbed_on_resize(application_handle* app_handle, u32 width, u32 height) { return TRUE; }

b8 testbed_on_key_pressed(u16 event_code, void* sender, void* listener_instance, event_data data) {
    application_handle* handle = (application_handle*)listener_instance;
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
                handle->app_config.settings.swapchain_present_mode = RENDERER_BACKEND_PRESENT_MODE_MAILBOX;
                handle->renderer_settings_modified = TRUE;
                return FALSE;
            case KEY_R:
                FR_INFO("R key was pressed this frame %d", fr_input_is_key_pressed_this_frame(KEY_R));
                FR_INFO("R key pressed at position: (%d, %d)", mouse_x, mouse_y);
                handle->app_config.settings.swapchain_present_mode = RENDERER_BACKEND_PRESENT_MODE_FIFO;
                handle->renderer_settings_modified = TRUE;
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

static inline b8 testbed_add_test_node(struct llist_head* llist, u32 data) {
    struct test_node* node;
    node = fr_memory_allocate(sizeof(*node), MEMORY_TYPE_LLIST);
    node->data = data;
    fr_core_llist_push((struct llist_node*)node, llist);
    return TRUE;
}
