/**
 * @file vulkan_types.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-02-19
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */

#pragma once

#include "fracture/fracture_core.h"
#include "fracture/core/systems/logging.h"
#include "vulkan/vulkan_core.h"

#include <vulkan/vulkan.h>

#define VK_CHECK_RESULT(f)                                                     \
    {                                                                            \
        VkResult res = (f);                                                        \
        if (res != VK_SUCCESS) {                                                   \
            FR_CORE_FATAL("Vulkan error %s : %d", #f, res);                          \
            return FALSE;                                                            \
        }                                                                          \
    }

typedef struct vulkan_swapchain_support_info {
    /** @brief The capabilities of the swapchain available */
    VkSurfaceCapabilitiesKHR capabilities;
    /** @brief The number of image formats available */
    u32 format_count;
    /** @brief Arary of the image formats */
    VkSurfaceFormatKHR* formats;
    /** @brief The number of present modes available */
    u32 present_mode_count;
    /** @brief Array of the present modes */
    VkPresentModeKHR* present_modes;
} vulkan_swapchain_support_info;

typedef struct vulkan_device {
    /** @brief The handle for the physical device. It is mostly used for getting
     * some device properties */
    VkPhysicalDevice physical_device;

    /** @brief The handle for the logical device, which is used for creating
     * queues and command buffers. This is what we interact with most of the
     * time*/
    VkDevice logical_device;

    /** @brief Holds the swapchain infomration */
    vulkan_swapchain_support_info swapchain_support;

    /** @brief The queue family indices */
    i32 graphics_family_index;
    i32 present_family_index;
    i32 compute_family_index;
    i32 transfer_family_index;

    /** @brief The queue family handles */
    VkQueue graphics_queue_handle;
    VkQueue present_queue_handle;
    VkQueue compute_queue_handle;
    VkQueue transfer_queue_handle;

    /** @brief physical device properties */
    VkPhysicalDeviceProperties properties;
    /** @brief physical device features */
    VkPhysicalDeviceFeatures features;
    /** @brief physical device memory properties */
    VkPhysicalDeviceMemoryProperties memory_properties;
} vulkan_device;

typedef struct vulkan_context {
    VkInstance instance;
    VkAllocationCallbacks* allocator;
    VkSurfaceKHR surface;

#if defined(FR_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif

    vulkan_device device;
} vulkan_context;
