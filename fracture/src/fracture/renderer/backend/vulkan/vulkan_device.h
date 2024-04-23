/**
 * @file vulkan_device.h
 * @author Aditya Rajagopal
 * @brief
 * @version 0.0.1
 * @date 2024-02-20
 *
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 *
 */
#pragma once

#include "vulkan_types.h"

/**
 * @brief Creates a physical and logical device for the Vulkan context
 *
 * @param context Reference to the Vulkan context
 * @return b8 TRUE if the device was created successfully, FALSE otherwise
 */
b8 vulkan_create_device(vulkan_context* context);

/**
 * @brief Destroys the Vulkan device
 *
 * @param context Reference to the Vulkan context
 */
void vulkan_destroy_device(vulkan_context* context);

/**
 * @brief Queries the swapchain support for the device
 *
 * @param device The physical device
 * @param surface The surface
 * @param support The swapchain support info
 */
b8 vulkan_device_query_swapchain_support(VkPhysicalDevice device,
                                         VkSurfaceKHR surface,
                                         vulkan_swapchain_support_info* out_support_info);

/**
 * @brief Selects the format for the depth buffer
 * @details The depth buffer format is selected based on the available formats on the device and the preferred formats.
 *
 * @param device The Vulkan device
 * @return b8 TRUE if the depth format was detected successfully, FALSE otherwise
 */
b8 vulkan_device_detect_depth_format(vulkan_device* device);
