/**
 * @file vulkan_swapchain.h
 * @author Aditya Rajagopal
 * @brief
 * @version 0.0.1
 * @date 2024-02-23
 *
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 *
 */
#pragma once

#include "vulkan/vulkan_core.h"
#include "vulkan_types.h"

/**
 * @brief Create the vulkan swapchain.
 * @details This function accepts a pointer to the vulkan swapchain struct and
 * creates the swapchain with the given width and height.
 *
 * @param context The vulkan context.
 * @param width The width of the swapchain image in pixels.
 * @param height The height of the swapchain image in pixels.
 * @param out_swapchain The vulkan swapchain struct to be created.
 */
b8 vulkan_swapchain_create(vulkan_context* context, u32 width, u32 height, vulkan_swapchain* out_swapchain);

/**
 * @brief Destroy the given vulkan swapchain and recreate it with the given
 * width and height.
 * @details Once a swapchain is created it is immutable. If the window is
 * resized, the swapchain must be destroyed and recreated.
 *
 * @param context The vulkan context.
 * @param width The new width of the swapchain image in pixels.
 * @param height The new height of the swapchain image in pixels.
 * @param swapchain The vulkan swapchain to be recreated.
 */
b8 vulkan_swapchain_recreate(vulkan_context* context, u32 width, u32 height, vulkan_swapchain* swapchain);

/**
 * @brief Destroy the given vulkan swapchain.
 *
 * @param context The vulkan context.
 * @param swapchain The vulkan swapchain to be destroyed.
 */
b8 vulkan_swapchain_destroy(vulkan_context* context, vulkan_swapchain* swapchain);

/**
 * @brief Get the index of the next image in the swapchain.
 * @details This function gets the index of the next image in the swapchain.
 * This index is used to get the image we want to render to. Once we are done
 * rendering to the image, we present it to the screen.
 *
 * @param context The vulkan context.
 * @param swapchain The vulkan swapchain.
 * @param timeout_ns The timeout in nanoseconds to wait for the next image.
 * @param image_available_semaphore The semaphore to signal when the image is available.
 * @param in_flight_fence The fence to signal when the image is available.
 * @param out_image_index The index of the next image in the swapchain.
 * @return b8 True if the image index was successfully retrieved, false otherwise.
 */
b8 vulkan_swapchain_get_next_image_index(vulkan_context* context,
                                         vulkan_swapchain* swapchain,
                                         u64 timeout_ns,
                                         VkSemaphore image_available_semaphore,
                                         VkFence in_flight_fence,
                                         u32* out_image_index);

/**
 * @brief Presents a given image index's image to the screen.
 * @details Once we are done rendering to an image, we present it to the screen.
 * This will use the present queue to present the image to the screen. The
 * graphics queue is used to submit the command buffer that renders to the
 * image.
 *
 * @param context The vulkan context.
 * @param swapchain The vulkan swapchain.
 * @param graphics_queue The graphics queue to submit the command buffer to.
 * @param present_queue The present queue to present the image to the screen.
 * @param present_image_index The index of the image to present.
 * @param render_finished_semaphore The semaphore to signal when rendering is finished that present will wait on.
 */
void vulkan_swapchain_present(vulkan_context* context,
                              vulkan_swapchain* swapchain,
                              VkQueue graphics_queue,
                              VkQueue present_queue,
                              u32 present_image_index,
                              VkSemaphore render_finished_semaphore);
