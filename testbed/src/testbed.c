#include "testbed.h"

typedef struct testbed_internal_state {
    i32 test;
    i32 postition[4];
    f32 rotation[4];
    f32 scale[4];
} testbed_internal_state;

static testbed_internal_state* state = NULL_PTR;
static u32* transform_array = NULL_PTR;

b8 testbed_initialize(application_handle *app_handle) {
    FR_INFO("Client Application initialized: %s", app_handle->app_config.name);
    state = fr_memory_allocate(sizeof(testbed_internal_state), MEMORY_TYPE_APPLICATION);
    transform_array = fr_memory_allocate(4 * 4 * sizeof(u32), MEMORY_TYPE_TRANSFORM);
    return TRUE;
}

b8 testbed_shutdown(application_handle *app_handle) {
    FR_INFO("Client Application shutdown: %s", app_handle->app_config.name);
    fr_memory_free(state, sizeof(testbed_internal_state), MEMORY_TYPE_APPLICATION);
    fr_memory_free(transform_array, 4 * 4 * sizeof(u32), MEMORY_TYPE_TRANSFORM);
    return TRUE;
}

b8 testbed_update(application_handle *app_handle, f32 delta_time) {
    return TRUE;
}

b8 testbed_render(application_handle *app_handle, f32 delta_time) {
    return TRUE;
}

b8 testbed_on_resize(application_handle *app_handle, u32 width, u32 height) {
    return TRUE;
}