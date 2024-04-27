#include "renderer_backend.h"

#include "fracture/core/systems/logging.h"
#include "fracture/renderer/backend/vulkan/vulkan_backend.h"
#include "fracture/renderer/renderer_types.h"

b8 fr_renderer_backend_create(const renderer_settings* settings,
                              struct platform_state* plat_state,
                              renderer_backend* out_renderer_backend) {
    if (!plat_state) {
        FR_CORE_FATAL("Platform state is NULL");
        return FALSE;
    }
    out_renderer_backend->plat_state = plat_state;

    switch (settings->backend_type) {
        case FR_RENDERER_BACKEND_VULKAN: {
            out_renderer_backend->settings = *settings;
            out_renderer_backend->PFN_initialize = vulkan_backend_initialize;
            out_renderer_backend->PFN_shutdown = vulkan_backend_shutdown;
            out_renderer_backend->PFN_begin_frame = vulkan_backend_begin_frame;
            out_renderer_backend->PFN_end_frame = vulkan_backend_end_frame;
            out_renderer_backend->PFN_on_window_resize = vulkan_backend_on_window_resize;
            out_renderer_backend->PFN_renderer_settings_update_callback = vulkan_backend_settings_callback;
            return TRUE;
        } break;
        default: {
            FR_CORE_FATAL("Renderer backend type %d not supported", settings->backend_type);
            return FALSE;
        }
    }

    return FALSE;
}

void fr_renderer_backend_destroy(renderer_backend* renderer_backend) {
    if (!renderer_backend) {
        FR_CORE_FATAL("Renderer backend is NULL");
        return;
    }
    renderer_backend->plat_state = NULL_PTR;
    renderer_backend->is_initialized = FALSE;
    renderer_backend->PFN_initialize = NULL_PTR;
    renderer_backend->PFN_shutdown = NULL_PTR;
    renderer_backend->PFN_begin_frame = NULL_PTR;
    renderer_backend->PFN_end_frame = NULL_PTR;
    renderer_backend->PFN_on_window_resize = NULL_PTR;
}
