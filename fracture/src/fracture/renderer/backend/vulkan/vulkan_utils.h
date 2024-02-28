/**
 * @file vulkan_utils.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-02-27
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include "vulkan_types.h"

/**
 * @brief Get the string representation of the given vulkan result.
 * 
 * @param result The vulkan result to get the string representation of.
 * @param get_extended Whether to get the extended string representation.
 * @return const char* The string representation of the given vulkan result.
 */
const char* vulkan_result_string(VkResult result, b8 get_extended);

/**
 * @brief Check if the given vulkan result is a success or an error as per the vulkan spec.
 * 
 * @param result The vulkan result to check.
 * @return b8 Whether the given vulkan result is a success. Defaults to TRUE if the result type is unknown
 */
b8 vulkan_result_is_success(VkResult result);