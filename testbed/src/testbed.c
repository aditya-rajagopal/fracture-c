#include "testbed.h"

b8 testbed_initialize(application_handle *app_handle) {
    FR_CORE_INFO("Client Application initialized: %s", app_handle->app_config.name);
    return TRUE;
}

b8 testbed_shutdown(application_handle *app_handle) {
    FR_CORE_INFO("Client Application shutdown: %s", app_handle->app_config.name);
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