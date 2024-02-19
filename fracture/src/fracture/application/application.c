#include "application.h"

#include "fracture/core/systems/logging.h"
#include "fracture/core/systems/fracture_memory.h"
#include "fracture/core/systems/event.h"
#include "fracture/core/systems/input.h"
#include "fracture/application/application_events.h"

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

b8 application_on_event(u16 event_code, void* sendeer, void* listener_instance, event_data data);
b8 application_on_key_event(u16 event_code, void* sender, void* listener_instance, event_data data);

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

    // Initialize the platform
    state.plat_state.on_key_event = fr_input_process_keypress;
    state.plat_state.on_mouse_move = fr_input_process_mouse_move;
    state.plat_state.on_mouse_button_event = fr_input_process_mouse_button;
    state.plat_state.on_mouse_scroll = fr_input_process_mouse_wheel;
    state.plat_state.on_window_close = fr_application_process_window_close;

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

    // Initialize the event system
    if (!fr_event_initialize()) {
        FR_CORE_FATAL("Failed to initialize event system");
        return FALSE;
    }
    FR_CORE_INFO("Event system initialized: %s", app_handle->app_config.name);

    // Initialize the input system
    if (!fr_input_initialize()) {
        FR_CORE_FATAL("Failed to initialize input system");
        return FALSE;
    }

    // Register the application to listen for events
    fr_event_register_handler(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    fr_event_register_handler(EVENT_CODE_KEY_PRESS, 0, application_on_key_event);
    fr_event_register_handler(EVENT_CODE_KEY_RELEASE, 0, application_on_key_event);

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

    fr_event_deregister_handler(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    fr_event_deregister_handler(EVENT_CODE_KEY_PRESS, 0, application_on_key_event);
    fr_event_deregister_handler(EVENT_CODE_KEY_RELEASE, 0, application_on_key_event);

    fr_input_shutdown();
    fr_event_shutdown();
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

        fr_input_update(0.0);

        if(!platform_pump_messages(&state.plat_state)) {
            state.is_running = FALSE;
        }

    }

    state.is_running = FALSE;
    return TRUE;
}

b8 application_on_event(u16 event_code, void* sender, void* listener_instance, event_data data) {
    switch (event_code) {
        case EVENT_CODE_APPLICATION_QUIT:
            state.is_running = FALSE;
            return TRUE;
    }
    return FALSE;
}

b8 application_on_key_event(u16 event_code, void* sender, void* listener_instance, event_data data) {
    if (event_code == EVENT_CODE_KEY_PRESS) {
        keys key = (keys)data.data.du16[0];
        b8 is_repeated = (b8)data.data.du16[1];
        i16 mouse_x = (i16)data.data.du16[2];
        i16 mouse_y = (i16)data.data.du16[3];

        switch (key) {
            case KEY_ESCAPE:
                FR_CORE_INFO("Escape key pressed at position: (%d, %d)", mouse_x, mouse_y);
                event_data data = {0};
                fr_event_dispatch(EVENT_CODE_APPLICATION_QUIT, 0, data);
                return FALSE;
            case KEY_A:
                FR_CORE_INFO(
                    "A key pressed at position: (%d, %d) and is %s repeated",
                    mouse_x, mouse_y, is_repeated ? "" : "not");
                return FALSE;
            default:
                break;
        }
    } else if (event_code == EVENT_CODE_KEY_RELEASE) {
        keys key = (keys)data.data.du16[0];
        i16 mouse_x = (i16)data.data.du16[1];
        i16 mouse_y = (i16)data.data.du16[2];

        switch (key) {
            case KEY_A:
                FR_CORE_INFO("A key released at position: (%d, %d)", mouse_x, mouse_y);
                return FALSE;
            default:
                break;
        }
    }
    return FALSE;
}