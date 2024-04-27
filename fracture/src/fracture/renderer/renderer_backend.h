/**
 * @file renderer_backend.h
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

struct platform_state;

/**
 * @brief Creates a renderer backend of the given type.
 *
 * @param type The type of the renderer backend to create.
 * @param plat_state The platform state to use for the renderer backend.
 * @param out_renderer_backend The renderer backend to create.
 * @return b8 True if the renderer backend was created successfully, false otherwise.
 */
b8 fr_renderer_backend_create(const renderer_settings* settings,
                              struct platform_state* plat_state,
                              renderer_backend* out_renderer_backend);

/**
 * @brief Destroys the given renderer backend.
 *
 * @param backend The renderer backend to destroy.
 */
void fr_renderer_backend_destroy(renderer_backend* backend);
