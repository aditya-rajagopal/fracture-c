#pragma once

#include <core/defines.h>
#include <fracture.h>

b8 testbed_initialize(application_handle* app_handle);

b8 testbed_shutdown(application_handle* app_handle);

b8 testbed_update(application_handle* app_handle, f32 delta_time);

b8 testbed_render(application_handle* app_handle, f32 delta_time);

b8 testbed_on_resize(application_handle* app_handle, u32 width, u32 height);