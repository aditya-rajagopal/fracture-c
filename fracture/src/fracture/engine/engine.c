#include "engine.h"

#include "fracture/core/systems/logging.h"
#include "fracture/core/systems/fracture_memory.h"
#include "fracture/core/systems/event.h"
#include "fracture/core/systems/input.h"
#include "fracture/core/systems/clock.h"
#include "fracture/core/library/fracture_string.h"

#include "fracture/engine/engine_events.h"
#include "fracture/core/types/system_event_codes.h"

#include "fracture/renderer/renderer_frontend.h"

#include <platform.h>

typedef struct engine_state {
    application_handle* app_handle;
    b8 is_running;
    b8 is_minimized;
    b8 is_supended;
    platform_state plat_state;
    clock app_clock;
    f64 last_frame_time;
    const char* name;
} engine_state;

static b8 is_initialized = FALSE;
static engine_state state;

b8 engine_on_event(u16 event_code, void* sendeer, void* listener_instance, event_data data);
b8 engine_on_key_event(u16 event_code, void* sender, void* listener_instance, event_data data);

b8 engine_initialize(application_handle* app_handle) {
    if (is_initialized) {
      FR_CORE_FATAL("An application has already been intialized: %s but "
                    "initializing a new one: %s",
                    state.name, app_handle->app_config.name);
      return FALSE;
    }

    state.name = app_handle->app_config.name;
    state.is_running = TRUE;
    state.is_minimized = FALSE;
    state.is_supended = FALSE;
    state.last_frame_time = 0.0;
    state.app_handle = app_handle;

    // Initialize the platform
    state.plat_state.on_key_event = fr_input_process_keypress;
    state.plat_state.on_mouse_move = fr_input_process_mouse_move;
    state.plat_state.on_mouse_button_event = fr_input_process_mouse_button;
    state.plat_state.on_mouse_scroll = fr_input_process_mouse_wheel;
    state.plat_state.on_window_close = fr_engine_process_window_close;

    if (!platform_startup(
            &state.plat_state, app_handle->app_config.name,
            app_handle->app_config.width, app_handle->app_config.height,
            app_handle->app_config.x_pos, app_handle->app_config.y_pos)) {
        FR_CORE_FATAL("Failed to initialize platform");
        return FALSE;
    }
    FR_CORE_INFO("Platform initialized: %s", app_handle->app_config.name);

    // Initialize the logger
    if (!fr_logging_initialize(&app_handle->app_config.logging_config)) {
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
    fr_event_register_handler(EVENT_CODE_APPLICATION_QUIT, 0, engine_on_event);
    fr_event_register_handler(EVENT_CODE_KEY_PRESS, 0, engine_on_key_event);
    fr_event_register_handler(EVENT_CODE_KEY_RELEASE, 0, engine_on_key_event);

    // Intialize the renderer
    if (!fr_renderer_initialize(app_handle->app_config.name, &state.plat_state)) {
        FR_CORE_FATAL("Failed to initialize renderer");
        return FALSE;
    }

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

b8 engine_shutdown(application_handle* app_handle) {
    if (!is_initialized) {
        FR_CORE_FATAL("No application has been initialized");
        return FALSE;
    }

    app_handle->shutdown(app_handle);
    FR_CORE_INFO("Shut down application: %s", app_handle->app_config.name);

    fr_event_deregister_handler(EVENT_CODE_APPLICATION_QUIT, 0, engine_on_event);
    fr_event_deregister_handler(EVENT_CODE_KEY_PRESS, 0, engine_on_key_event);
    fr_event_deregister_handler(EVENT_CODE_KEY_RELEASE, 0, engine_on_key_event);

    fr_renderer_shutdown();
    FR_CORE_INFO("Renderer shutdown: %s", app_handle->app_config.name);
    fr_input_shutdown();
    FR_CORE_INFO("Input system shutdown: %s", app_handle->app_config.name);
    fr_event_shutdown();
    FR_CORE_INFO("Event system shutdown: %s", app_handle->app_config.name);
    fr_logging_shutdown();
    FR_CORE_INFO("Logging shutdown: %s", app_handle->app_config.name);

    platform_shutdown(&state.plat_state);
    FR_CORE_INFO("Platform shutdown: %s", app_handle->app_config.name);
    is_initialized = FALSE;
    return TRUE;
}

b8 engine_run(application_handle* app_handle) {
    if (!is_initialized) {
        FR_CORE_FATAL("No application has been initialized");
        return FALSE;
    }
    fr_clock_start(&state.app_clock);
    fr_clock_update(&state.app_clock);
    state.last_frame_time = state.app_clock.elapsed_time;
    // f64 total_run_time = 0.0;
    // u64 frame_count = 0;
    f64 target_frame_seconds = 1.0F / app_handle->app_config.target_frame_rate;

    FR_CORE_INFO("Running application: %s", app_handle->app_config.name);
    
    fr_memory_print_stats();

    while(state.is_running) {
        if (!platform_pump_messages(&state.plat_state)) {
            state.is_running = FALSE;
        }

        if (!state.is_supended) {
            fr_clock_update(&state.app_clock);
            f64 delta_time = state.app_clock.elapsed_time - state.last_frame_time;
            f64 frame_start_time = platform_get_absolute_time();

            if (!app_handle->update(app_handle, delta_time)) {
                FR_CORE_FATAL("Failed to update client application");
                state.is_running = FALSE;
                return FALSE;
            }

            if (!app_handle->render(app_handle, delta_time)) {
                FR_CORE_FATAL("Failed to render client application");
                state.is_running = FALSE;
                return FALSE;
            }

            // Trigger the renderer to draw the frame
            {
                // TODO: We dont want to create the packet here every frame.
                renderer_packet packet = {0};
                packet.delta_time = delta_time;
                if(!fr_renderer_draw_frame(&packet)) {
                    FR_CORE_ERROR("Failed to draw frame");
                }
            }

            f64 frame_end_time = platform_get_absolute_time();
            f64 frame_time = frame_end_time - frame_start_time;
            // total_run_time += frame_time;
            f64 sleep_time = target_frame_seconds - frame_time;


            if (sleep_time > 0.0 && app_handle->app_config.lock_frame_rate) {
                u64 sleep_time_ms = (u64)(sleep_time * 1000);

                platform_sleep(sleep_time_ms - 1);
            }

            fr_input_update(delta_time);
            // frame_count++;
            state.last_frame_time = state.app_clock.elapsed_time;

        }

    }

    state.is_running = FALSE;
    return TRUE;
}

b8 engine_on_event(u16 event_code, void* sender, void* listener_instance, event_data data) {
    switch (event_code) {
        case EVENT_CODE_APPLICATION_QUIT:
            state.is_running = FALSE;
            return TRUE;
    }
    return FALSE;
}

b8 engine_on_key_event(u16 event_code, void* sender, void* listener_instance, event_data data) {
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