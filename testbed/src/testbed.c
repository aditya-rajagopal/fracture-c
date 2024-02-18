#include "testbed.h"

#include "fracture/core/containers/darrays.h"

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

    // darray test
    u32* darray = darray_reserve(20, u32);
    for (u32 i = 0; i < 20; i++) {
        darray_push(darray, i);
    }
    FR_INFO("Darray length: %d", darray_length(darray));
    FR_INFO("Darray capacity: %d", darray_capacity(darray));
    FR_INFO("Darray element size: %d", darray_element_size(darray));
    u32 length = darray_length(darray);
    for (u32 i = 0; i < length; i++) {
        u32 value = 0;
        darray_pop(darray, &value);
        FR_INFO("Darray[%d]: %d", i, value);
    }
    darray_destroy(darray);

    u32* darray2 = darray_reserve(1, u32);
    for (u32 i = 0; i < 20; i++) {
        darray_push(darray2, i);
    }
    u32 value = 0;
    FR_INFO("Darray2 length: %d", darray_length(darray2));
    FR_INFO("Darray2 capacity: %d", darray_capacity(darray2));
    FR_INFO("Darray2 element size: %d", darray_element_size(darray2));
    darray_pop_at(darray2, &value, 5);
    FR_INFO("Darray2 pop at 5: %d", value);
    darray_pop(darray2, &value);
    FR_INFO("Darray2 pop: %d", value);
    darray_destroy(darray2);

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