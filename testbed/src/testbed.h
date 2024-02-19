#pragma once

#include <fracture/core/defines.h>
#include <fracture.h>

typedef struct testbed_state {
    b8 is_running;
} testbed_state;

b8 testbed_initialize(application_handle* app_handle);

b8 testbed_shutdown(application_handle* app_handle);

b8 testbed_update(application_handle* app_handle, f64 delta_time);

b8 testbed_render(application_handle* app_handle, f64 delta_time);

b8 testbed_on_resize(application_handle* app_handle, u32 width, u32 height);