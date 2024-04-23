/**
 * @file renderer_frontend.h
 * @author Aditya Rajagopal
 * @brief The renderer frontend is the interface that the game engine uses to
 * interact with the renderer backend.
 * @details The renderer frontend is the interface that the game engine uses to
 * interact with the renderer backend. It is a thin layer that abstracts the backend that can be
 * different renderers such as OpenGL, Vulkan, DirectX, or Metal.
 * @version 0.0.1
 * @date 2024-02-19
 *
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 *
 */
#pragma once

#include "fracture/renderer/renderer_types.h"

/**
 * @brief Initializes the renderer backend
 *
 * @param app_name The name of the application
 * @param plat_state The platform state
 * @return b8 TRUE if the renderer was initialized successfully
 */
b8 fr_renderer_initialize(const char* app_name, struct platform_state* plat_state);

/**
 * @brief Shuts down the renderer backend
 *
 */
void fr_renderer_shutdown();

/**
 * @brief Handles the drawing of the frame
 *
 * @return b8 TRUE if the frame was begun successfully
 */
b8 fr_renderer_draw_frame(renderer_packet* package);

/**
 * @brief Handles window resize events for the renderer
 *
 * @param width The new width of the window
 * @param height The new height of the window
 * @return b8 TRUE if the frame was ended successfully
 */
void fr_renderer_on_window_resize(u32 width, u32 height);
