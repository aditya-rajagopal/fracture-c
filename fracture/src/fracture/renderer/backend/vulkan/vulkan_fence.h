/**
 * @file vulkan_fence.h
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
 * @brief Create a vulkan fence.
 * @details This function creates a vulkan fence with the given flags.
 *
 * @param context The vulkan context.
 * @param out_fence The vulkan fence struct to be created.
 * @param create_singnaled Whether the fence should be created in the signaled state.
 * @return b8 Whether the fence was created successfully.
 */
b8 vulkan_fence_create(vulkan_context* context, vulkan_fence* out_fence, b8 create_singnaled);

/**
 * @brief Destroy the given vulkan fence.
 *
 * @param context The vulkan context.
 * @param fence The vulkan fence to be destroyed.
 */
void vulkan_fence_destroy(vulkan_context* context, vulkan_fence* fence);

/**
 * @brief Wait for the given vulkan fence to be signaled.
 *
 * @param context The vulkan context.
 * @param fence The vulkan fence to wait for.
 * @param timeout_ns maximum timeout in nanoseconds to wait for the fence to be signaled.
 * @return b8 Whether the fence was signaled within the timeout and FALSE if there was an error
 */
b8 vulkan_fence_wait(vulkan_context* context, vulkan_fence* fence, u64 timeout_ns);

/**
 * @brief Reset the given vulkan fence's signl state to false.
 *
 * @param context The vulkan context.
 * @param fence The vulkan fence to be reset.
 */
b8 vulkan_fence_reset(vulkan_context* context, vulkan_fence* fence);
