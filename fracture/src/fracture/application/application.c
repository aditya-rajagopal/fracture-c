#include "application.h"

#include "fracture/systems/logging.h"
#include "fracture/systems/fracture_memory.h"

#include <platform.h>

typedef struct application_state {
    application_handle* app_handle;
    b8 is_running;
    b8 is_minimized;
    platform_state plat_state;
    f64 last_frame_time;
    const char* name;
} application_state;

static b8 is_initialized = FALSE;
static application_state state;

b8 application_initialize(application_handle* app_handle) {
    if (is_initialized) {
      FR_CORE_FATAL("An application has already been intialized: %s but "
                    "initializing a new one: %s",
                    state.name, app_handle->app_config.name);
      return FALSE;
    }

    state.name = app_handle->app_config.name;
    state.is_running = TRUE;
    state.is_minimized = FALSE;
    state.last_frame_time = 0.0;
    state.app_handle = app_handle;

    if (!platform_startup(
            &state.plat_state, app_handle->app_config.name,
            app_handle->app_config.width, app_handle->app_config.height,
            app_handle->app_config.x_pos, app_handle->app_config.y_pos)) {
        FR_CORE_FATAL("Failed to initialize platform");
        return FALSE;
    }

    // Initialize the logger
    if (!initialize_logging(&app_handle->app_config.logging_config)) {
        FR_CORE_FATAL("Failed to initialize logging");
        return FALSE;
    }
    FR_CORE_INFO("Logging initialized: %s", app_handle->app_config.name);


    // Initialize the application
    if (!app_handle->initialize(app_handle)) {
        FR_CORE_FATAL("Failed to initialize client application");
        return FALSE;
    }
    FR_CORE_INFO("Client Application initialized: %s",
                 app_handle->app_config.name);

    is_initialized = TRUE;
    return TRUE;
}


b8 application_shutdown(application_handle* app_handle) {
    if (!is_initialized) {
        FR_CORE_FATAL("No application has been initialized");
        return FALSE;
    }

    app_handle->shutdown(app_handle);

    shutdown_logging();
    platform_shutdown(&state.plat_state);
    is_initialized = FALSE;
    return TRUE;
}

b8 application_run(application_handle* app_handle) {
    if (!is_initialized) {
        FR_CORE_FATAL("No application has been initialized");
        return FALSE;
    }
    FR_CORE_INFO("Running application: %s", app_handle->app_config.name);

    f32 local_pi = PI;

    FR_CORE_FATAL("This is a fatal message: %f", local_pi);
    FR_CORE_ERROR("This is an error message: %f", local_pi);
    FR_CORE_WARN("This is a warning message: %f", local_pi);
    FR_CORE_INFO("This is an info message: %f", local_pi);
    FR_CORE_TRACE("This is a trace message: %f", local_pi);
    FR_CORE_ASSERT(1 == 1);

    FR_CORE_INFO(fr_memory_get_stats());
    
    logging_level_set(LOG_LEVEL_INFO, FALSE);
    FR_CORE_INFO("This is an info message: %f", local_pi);

    while(state.is_running) {
        f64 current_time = platform_get_absolute_time();
        f32 delta_time = (f32)(current_time - state.last_frame_time);
        state.last_frame_time = current_time;

        if (!app_handle->update(app_handle, delta_time)) {
          FR_CORE_FATAL("Failed to update client application");
          return FALSE;
        }

        if (!app_handle->render(app_handle, delta_time)) {
          FR_CORE_FATAL("Failed to render client application");
          return FALSE;
        }

        if(!platform_pump_messages(&state.plat_state)) {
            state.is_running = FALSE;
        }
    }

    state.is_running = FALSE;
    return TRUE;
}