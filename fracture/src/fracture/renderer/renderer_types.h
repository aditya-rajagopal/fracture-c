#pragma once

#include "fracture/core/defines.h"

typedef enum renderer_backend_type {
    FR_RENDERER_BACKEND_OPENGL,
    FR_RENDERER_BACKEND_VULKAN,
    FR_RENDERER_BACKEND_DIRECTX,
    FR_RENDERER_BACKEND_METAL
} renderer_backend_type;

typedef enum renderer_backend_return_codes {
    RENDERER_BACKEND_FAILURE = 0,
    RENDERER_BACKEND_SUCCESS = 1,
    RENDERER_BACKEND_NOT_READY = 2,
    RENDERER_BACKEND_FAILED_INITIALIZATION = 3,
} renderer_backend_return_codes_t;

typedef enum rendere_backend_present_modes {
    RENDERER_BACKEND_PRESENT_MODE_IMMEDIATE,
    RENDERER_BACKEND_PRESENT_MODE_MAILBOX,
    RENDERER_BACKEND_PRESENT_MODE_FIFO
} renderer_backend_present_modes;

typedef struct renderer_settings {
    // Some settings need an application restart and some can be changed live
    // Restartless parameters
    renderer_backend_type backend_type;
    u32 max_frames_in_flight;
    renderer_backend_present_modes swapchain_present_mode;

    // Requires restart
    b8 require_graphics_queue;
    b8 require_present_queue;
    b8 require_transfer_queue;
    b8 require_compute_queue;
    b8 sample_anisotropy;
    b8 use_discrete_gpu;
} renderer_settings;

/**
 * @brief The renderer backend is the interface that the game engine uses to
 */
typedef struct renderer_backend {
    // Forward declare platform_state
    struct platform_state* plat_state;
    renderer_settings settings;
    b8 is_initialized;
    u64 frame_number;

    // Renderer interface
    b8 (*PFN_initialize)(struct renderer_backend* backend, const char* app_name, struct platform_state* plat_state);
    void (*PFN_shutdown)(struct renderer_backend* backend);

    b8 (*PFN_begin_frame)(struct renderer_backend* backend, f64 delta_time);
    b8 (*PFN_end_frame)(struct renderer_backend* backend, f64 delta_time);

    void (*PFN_on_window_resize)(struct renderer_backend* backend, u32 width, u32 height);
    b8 (*PFN_renderer_settings_update_callback)(struct renderer_backend* backend);
} renderer_backend;

/**
 * @brief Holds data that is passed to the renderer with all the information it needs to render a frame.
 *
 */
typedef struct renderer_packet {
    f64 delta_time;
} renderer_packet;
