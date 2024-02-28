/**
 * @file vulkan_renderpass.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-02-26
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include "vulkan_types.h"

/**
 * @brief Create a vulkan renderpass.
 * @details This function creates a vulkan renderpass with the given render area,
 * clear colour, clear depth, and clear stencil.
 *
 * @param context The vulkan context.
 * @param out_renderpass The vulkan renderpass struct to be created.
 * @param render_area The area of the renderpass in pixels.
 * @param clear_colour The colour to clear the renderpass with.
 * @param clear_depth The depth to clear the renderpass with.
 * @param clear_stencil The stencil to clear the renderpass with.
 */
b8 vulkan_renderpass_create(vulkan_context* context,
                            vulkan_renderpass* out_renderpass,
                            rect_2d render_area, colour clear_colour,
                            f32 clear_depth, u32 clear_stencil);

/**
 * @brief Destroy the given vulkan renderpass.
 * 
 * @param context The vulkan context.
 * @param renderpass The vulkan renderpass to be destroyed.
 */
void vulkan_renderpass_destroy(vulkan_context* context,
                               vulkan_renderpass* renderpass);

/**
 * @brief Begin the given vulkan renderpass.
 * 
 * @param renderpass The vulkan renderpass.
 * @param framebuffer Framebuffer to render to.
 * @param command_buffer The command buffer that will record commands for the renderpass.
 */
void vulkan_renderpass_begin(vulkan_renderpass* renderpass,
                           vulkan_frame_buffer* framebuffer,
                           vulkan_command_buffer* command_buffer);

/**
 * @brief End the given vulkan renderpass.
 * 
 * @param renderpass The vulkan renderpass.
 * @param command_buffer The command buffer that will record commands for the renderpass.
 */
void vulkan_renderpass_end(vulkan_renderpass* renderpass,
                           vulkan_command_buffer* command_buffer);