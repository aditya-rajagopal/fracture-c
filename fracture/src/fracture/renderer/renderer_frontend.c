#include "renderer_frontend.h"

#include "fracture/core/systems/fracture_memory.h"
#include "fracture/core/systems/logging.h"
#include "fracture/renderer/renderer_backend.h"

static renderer_backend *current_backend = NULL_PTR;

b8 _renderer_begin_frame(renderer_packet *package);
b8 _renderer_end_frame(renderer_packet *package);

b8 fr_renderer_initialize(const char *app_name, struct platform_state *plat_state, const renderer_settings *settings) {
    if (current_backend) {
        FR_CORE_WARN("Renderer already initialized");
        return TRUE;
    }

    if (!plat_state) {
        FR_CORE_FATAL("Platform state is NULL");
        return FALSE;
    }

    current_backend = (renderer_backend *)fr_memory_allocate(sizeof(renderer_backend), MEMORY_TYPE_RENDERER);

    // TODO: Add ability to pass in a configuration structure to the renderer backend
    if (!fr_renderer_backend_create(settings, plat_state, current_backend)) {
        FR_CORE_FATAL("Failed to create renderer backend");
        return FALSE;
    }

    if (!current_backend->PFN_initialize(current_backend, app_name, plat_state)) {
        FR_CORE_FATAL("Failed to initialize renderer backend");
        return FALSE;
    }

    return TRUE;
}

void fr_renderer_shutdown() {
    if (!current_backend) {
        FR_CORE_WARN("Renderer already shutdown or not initialized");
        return;
    }

    current_backend->PFN_shutdown(current_backend);
    fr_renderer_backend_destroy(current_backend);
    fr_memory_free(current_backend, sizeof(renderer_backend), MEMORY_TYPE_RENDERER);
    current_backend = NULL_PTR;
}

void fr_renderer_on_window_resize(u32 width, u32 height) {
    if (!current_backend) {
        FR_CORE_FATAL("Renderer not initialized");
        return;
    }

    current_backend->PFN_on_window_resize(current_backend, width, height);
}

b8 fr_renderer_draw_frame(renderer_packet *package) {
    if (!current_backend) {
        FR_CORE_FATAL("Renderer not initialized");
        return FALSE;
    }

    if (_renderer_begin_frame(package)) {
        // Only continue if the frame was successfully started and the backend is ready to render
        if (!_renderer_end_frame(package)) {
            FR_CORE_FATAL("Failed to end frame");
            return FALSE;
        }
    }
    return TRUE;
}

b8 fr_renderer_update_renderer_config(const renderer_settings *settings) {
    if (!current_backend) {
        FR_CORE_FATAL("Renderer has not been initialised");
        return FALSE;
    }
    if (settings->backend_type != current_backend->settings.backend_type) {
        FR_CORE_WARN("Attempting to change the renderer backend type");
        FR_CORE_FATAL("Changing renderer backend type is currently not supported");
        // TODO: If we implement multiple backends here is where we would hotswap renderers if the application requests
        // a new renderer backend
        return FALSE;
    }

    current_backend->settings = *settings;
    current_backend->PFN_renderer_settings_update_callback(current_backend);
    return TRUE;
}

// -----------------------------------------------------------------------
// PRIVATE FUNCTIONS
// -----------------------------------------------------------------------

b8 _renderer_begin_frame(renderer_packet *package) {
    return current_backend->PFN_begin_frame(current_backend, package->delta_time);
}

b8 _renderer_end_frame(renderer_packet *package) {
    b8 result = current_backend->PFN_end_frame(current_backend, package->delta_time);
    current_backend->frame_number++;
    return result;
}
