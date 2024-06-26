#include <fracture/engine/entry.h>
#include <string.h>

#include "fracture/renderer/renderer_types.h"
#include "testbed.h"

b8 create_client_application(application_handle* app_handle) {
    app_handle->app_config.name = "Testbed";
    app_handle->app_config.start_width = 1280;
    app_handle->app_config.start_height = 720;
    app_handle->app_config.start_x_pos = 100;
    app_handle->app_config.start_y_pos = 100;
    app_handle->app_config.target_frame_rate = 60;
    app_handle->app_config.lock_frame_rate = FALSE;

    logging_config config = {0};
    config.enable_console = TRUE;
    config.enable_file = FALSE;
    config.logging_flags = FR_LOG_LEVEL_ALL;
    config.filename = NULL_PTR;
    app_handle->app_config.logging_config = config;

    app_handle->app_config.settings.backend_type = FR_RENDERER_BACKEND_VULKAN;
    app_handle->app_config.settings.require_graphics_queue = TRUE;
    app_handle->app_config.settings.require_transfer_queue = TRUE;
    app_handle->app_config.settings.require_present_queue = TRUE;
    app_handle->app_config.settings.require_compute_queue = TRUE;
    app_handle->app_config.settings.sample_anisotropy = TRUE;
    app_handle->app_config.settings.use_discrete_gpu = TRUE;
    app_handle->app_config.settings.max_frames_in_flight = 2;
    app_handle->app_config.settings.swapchain_present_mode = RENDERER_BACKEND_PRESENT_MODE_MAILBOX;

    app_handle->initialize = testbed_initialize;
    app_handle->shutdown = testbed_shutdown;
    app_handle->update = testbed_update;
    app_handle->render = testbed_render;
    app_handle->on_resize = testbed_on_resize;

    app_handle->application_data = fr_memory_allocate(sizeof(testbed_state), MEMORY_TYPE_APPLICATION);
    memset(app_handle->application_data, 0, sizeof(testbed_state));
    return TRUE;
}

b8 destroy_client_application(application_handle* app_handle) {
    fr_memory_free(app_handle->application_data, sizeof(testbed_state), MEMORY_TYPE_APPLICATION);
    return TRUE;
}
