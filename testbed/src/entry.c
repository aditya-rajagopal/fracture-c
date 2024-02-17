#include "testbed.h"

#include <fracture/application/entry.h>

#include <stdlib.h>
#include <string.h>

b8 create_client_application(application_handle* app_handle) {

    app_handle->app_config.name = "Testbed";
    app_handle->app_config.width = 1280;
    app_handle->app_config.height = 720;
    app_handle->app_config.x_pos = 100;
    app_handle->app_config.y_pos = 100;

    logging_config config = {0};
    config.enable_console = TRUE;
    config.enable_file = FALSE;
    config.logging_flags = FR_LOG_LEVEL_ALL;
    config.filename = NULL_PTR;
    app_handle->app_config.logging_config = config;

    app_handle->initialize = testbed_initialize;
    app_handle->shutdown = testbed_shutdown;
    app_handle->update = testbed_update;
    app_handle->render = testbed_render;
    app_handle->on_resize = testbed_on_resize;

    app_handle->application_data = fr_memory_allocate(sizeof(testbed_state), MEMORY_TYPE_APPLICATION);
    memset(app_handle->application_data, 1, sizeof(testbed_state));
    return TRUE;
}

b8 destroy_client_application(application_handle *app_handle) {
    fr_memory_free(app_handle->application_data, sizeof(testbed_state), MEMORY_TYPE_APPLICATION);
    return TRUE;
}
