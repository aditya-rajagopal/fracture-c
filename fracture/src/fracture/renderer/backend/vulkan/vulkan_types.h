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

#include <vulkan/vulkan.h>

#define VK_CHECK_RESULT(f)                                  \
    {                                                       \
        VkResult res = (f);                                 \
        if (res != VK_SUCCESS) {                            \
            FR_CORE_FATAL("Vulkan error %s : %d", #f, res); \
            return FALSE;                                   \
        }                                                   \
    }

typedef struct vulkan_image {
    /** @brief The handle for the image */
    VkImage image_handle;
    /** @brief The memory handle for the image */
    VkDeviceMemory memory_handle;
    /** @brief The image view handle */
    VkImageView image_view;
    /** @brief the width of the image */
    u32 width;
    /** @brief the height of the image */
    u32 height;
} vulkan_image;

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
    /** @brief The format of the depth buffer */
    VkFormat depth_format;

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

typedef struct vulkan_swapchain {
    /** @brief The image format for the swapchain */
    VkSurfaceFormatKHR format;
    /** @brief the maximum number of frames we can have going at once*/
    u32 max_frames_in_flight;
    /** @brief The handle for the swapchain */
    VkSwapchainKHR handle;
    /** @brief the number of images in the swapchain queue */
    u32 image_count;
    /** @brief the array of image handles */
    VkImage* images;
    /** @brief the array of image views */
    VkImageView* image_views;
    /** @brief the depth buffer image */
    vulkan_image depth_attachment;
} vulkan_swapchain;

typedef struct vulkan_context {
    /** @brief The handle for the vulkan instance */
    VkInstance instance;
    /** @brief callback for a custom allocator */
    VkAllocationCallbacks* allocator;
    /** @brief The handle for the window surface */
    VkSurfaceKHR surface;
    /** 
     * @brief The width of the framebuffer we are currently rendering to.
     * @details the height of the framebuffer we are currently rendering to. We
     * want to store this in the context as it might be useful in many rendering
     * tasks*/
    u32 framebuffer_width;
    /** @brief the width of the framebuffer we are currently rendering to.
     * @details We want to store this in the context as it might be useful in
     * many rendering tasks
     */
    u32 framebuffer_height;
#if defined(FR_DEBUG)
    /** @brief The debug messenger */
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
    /** @brief the vulkan device struct holding information we need about our device and its capabilities */
    vulkan_device device;
    /** @brief the vulkan swapchain struct holding information we need about our swapchain and its handle*/
    vulkan_swapchain swapchain;
    /** @brief the current image index we are rendering */
    u32 current_image_index;
    /** @brief  */
    u32 current_frame_index;
    /** @brief boolean to track if we are currently recreating the swapchain */
    b8 recreating_swapchain;


    i32 (*PFN_find_memory_type)(u32 type_filter, VkMemoryPropertyFlags properties);
} vulkan_context;
