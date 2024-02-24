/**
 * @file vulkan_image.h
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
 * @brief Create a new vulkan image.
 * 
 * @param context The vulkan context.
 * @param image_type The type of the image.
 * @param width The width of the image.
 * @param height The height of the image.
 * @param format The format of the image.
 * @param tiling The tiling of the image.
 * @param usage The usage of the image.
 * @param memory_flags The memory flags of the image.
 * @param create_view Whether to create a view for the image.
 * @param aspect_flags The aspect flags of the image.
 * @param out_image The vulkan image to be created.
 */
b8 vulkan_image_create(
    vulkan_context* context,
    VkImageType image_type,
    u32 width,
    u32 height,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags memory_flags,
    b32 create_view,
    VkImageAspectFlags view_aspect_flags,
    vulkan_image* out_image);

/**
 * @brief Creates a new image view for the given image.
 * 
 * @param context The vulkan context.
 * @param image The vulkan image to create the view for.
 * @param format The format of the image.
 * @param aspect_flags The aspect flags of the image.
 */
b8 vulkan_image_create_view(
    vulkan_context* context,
    vulkan_image* image,
    VkFormat format,
    VkImageAspectFlags aspect_flags);

/**
 * @brief Creates a new image view for the given image.
 * 
 * @param context The vulkan context.
 * @param image The vulkan image to create the view for.
 * @param format The format of the image.
 * @param aspect_flags The aspect flags of the image.
 * @param out_view The vulkan image view to be created.
 */
b8 vulkan_image_create_view_from_image(
    vulkan_context* context,
    VkImage image,
    VkFormat format,
    VkImageAspectFlags aspect_flags,
    VkImageView* out_view);

/**
 * @brief Destroy the given vulkan image.
 * 
 * @param context The vulkan context.
 * @param image The vulkan image to be destroyed.
 */
void vulkan_image_destroy(vulkan_context* context, vulkan_image* image);