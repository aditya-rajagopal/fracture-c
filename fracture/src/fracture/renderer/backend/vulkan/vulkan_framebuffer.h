/**
 * @file vulkan_framebuffer.h
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
 * @brief Create a vulkan framebuffer.
 * @details This function creates a vulkan framebuffer for the given renderpass,
 * width, height, and attachments.
 *
 * @param context The vulkan context.
 * @param renderpass The renderpass to create the framebuffer for.
 * @param width The width of the framebuffer in pixels.
 * @param height The height of the framebuffer in pixels.
 * @param attachment_count The number of attachments in the framebuffer.
 * @param pAttachments The attachments to the framebuffer.
 * @param out_framebuffer The vulkan framebuffer struct to be created.
 * @return b8 Whether the framebuffer was created successfully.
 */
b8 vulkan_framebuffer_create(vulkan_context* context,
                             vulkan_renderpass* renderpass,
                             u32 width,
                             u32 height,
                             u32 attachment_count,
                             VkImageView* pAttachments,
                             vulkan_frame_buffer* out_framebuffer);

/**
 * @brief Destroy the given vulkan framebuffer.
 *
 * @param context The vulkan context.
 * @param framebuffer The vulkan framebuffer to be destroyed.
 */
void vulkan_framer_buffer_destroy(vulkan_context* context, vulkan_frame_buffer* framebuffer);
