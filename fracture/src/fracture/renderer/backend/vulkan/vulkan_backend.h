/**
 * @file vulkan_backend.h
 * @author Aditya Rajagopal
 * @brief
 * @version 0.0.1
 * @date 2024-02-19
 *
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 *
 */
#pragma once

#include "fracture/renderer/renderer_types.h"

/**
 * @brief Initializes the Vulkan renderer backend
 *
 * @param backend The renderer backend to initialize
 * @param app_name The name of the application
 * @param plat_state The platform state
 * @return b8
 */
b8 vulkan_backend_initialize(renderer_backend* backend, const char* app_name, struct platform_state* plat_state);

/**
 * @brief Shuts down the Vulkan renderer backend
 *
 * @param backend The renderer backend to shut down
 */
void vulkan_backend_shutdown(renderer_backend* backend);

/**
 * @brief Begins a frame in the Vulkan renderer backend
 *
 * @param backend The renderer backend to begin the frame in
 * @param delta_time The time since the last frame
 * @return b8
 */
b8 vulkan_backend_begin_frame(renderer_backend* backend, f64 delta_time);

/**
 * @brief Ends a frame in the Vulkan renderer backend
 *
 * @param backend The renderer backend to end the frame in
 * @param delta_time The time since the last frame
 * @return b8
 */
b8 vulkan_backend_end_frame(renderer_backend* backend, f64 delta_time);

/**
 * @brief Called when the window is resized
 *
 * @param backend The renderer backend
 * @param width The new width of the window
 * @param height The new height of the window
 */
void vulkan_backend_on_window_resize(renderer_backend* backend, u32 width, u32 height);

/**
 * @brief Callback for providing the renderer_settings to the backend
 *
 * @param settings a pointer to the struct containing the settings for the renderer
 * @return b8 TRUE if the settings were applied FALSE if there is an issue with the settings
 */
b8 vulkan_backend_settings_callback(renderer_backend* backend);
