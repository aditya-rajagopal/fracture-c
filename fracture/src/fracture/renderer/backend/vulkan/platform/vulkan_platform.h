/**
 * @file vulkan_platform.h
 * @author Aditya Rajagopal
 * @brief
 * @version 0.0.1
 * @date 2024-02-20
 *
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 *
 */
#pragma once

#include "fracture/renderer/backend/vulkan/vulkan_types.h"

struct platform_state;
struct vulkan_context;

/**
 * @brief Gets the required instance extensions for the platform and appends it
 * to the darray of required extensions.
 *
 * @param required_extensions The required instance extensions
 */
void vulkan_platform_get_required_instance_extensions(const char*** required_extensions);

/**
 * @brief Creates a surface for the platform
 *
 * @param plat_state The platform state
 * @param context The vulkan context
 * @return b8 Whether the surface was created successfully
 */
b8 vulkan_platform_create_surface(struct vulkan_context* context);
