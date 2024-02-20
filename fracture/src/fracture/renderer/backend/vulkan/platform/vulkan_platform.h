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

#include "fracture/fracture_core.h"

/**
 * @brief Gets the required instance extensions for the platform and appends it
 * to the darray of required extensions.
 *
 * @param required_extensions The required instance extensions
 */
void vulkan_platform_get_required_instance_extensions(const char*** required_extensions);
