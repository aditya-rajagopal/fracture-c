#include "vulkan_image.h"

#include "fracture/core/systems/logging.h"
#include "vulkan/vulkan_core.h"

b8 vulkan_image_create(vulkan_context* context, VkImageType image_type,
                         u32 width, u32 height, VkFormat format,
                         VkImageTiling tiling, VkImageUsageFlags usage,
                         VkMemoryPropertyFlags memory_flags, b32 create_view,
                         VkImageAspectFlags view_aspect_flags,
                         vulkan_image* out_image) {
    out_image->width = width;
    out_image->height = height;

    VkImageCreateInfo image_info = {0};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = image_type;
    image_info.extent.width = width;
    image_info.extent.height = height;
    image_info.extent.depth = 1; // TODO: Make this a parameter configurable
    image_info.mipLevels = 4;    // TODO: Make this a parameter configurable
    image_info.arrayLayers = 1;  // TODO: Make this a parameter configurable
    image_info.format = format;
    image_info.tiling = tiling;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = usage;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT; // TODO: Make this a parameter configurable
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // TODO: Make this a parameter configurable

    VK_CHECK_RESULT(vkCreateImage(context->device.logical_device, &image_info, NULL, &out_image->image_handle));

    // Once we create the image handle we need to allocate memory for it
    VkMemoryRequirements mem_reqs;
    vkGetImageMemoryRequirements(context->device.logical_device, out_image->image_handle, &mem_reqs);    

    i32 memory_type = context->PFN_find_memory_type(mem_reqs.memoryTypeBits, memory_flags);
    if (memory_type == -1) {
        FR_CORE_FATAL("Failed to find suitable memory type for image");
        return FALSE;
    }

    VkMemoryAllocateInfo alloc_info = {0};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_reqs.size;
    alloc_info.memoryTypeIndex = memory_type;

    VK_CHECK_RESULT(vkAllocateMemory(context->device.logical_device, &alloc_info, context->allocator, &out_image->memory_handle));

    // We need to bind the memory to the image handle so that we can use it in the shader programs
    // TODO: Make the offset a parameter for memory pools
    VK_CHECK_RESULT(vkBindImageMemory(context->device.logical_device, out_image->image_handle, out_image->memory_handle, 0));

    if (create_view) {
        out_image->image_view = 0;
        vulkan_image_create_view(context, out_image, format, view_aspect_flags);
    }
    return TRUE;
}

b8 vulkan_image_create_view(vulkan_context *context, vulkan_image *image, VkFormat format, VkImageAspectFlags aspect_flags) {
    return vulkan_image_create_view_from_image(context, image->image_handle, format, aspect_flags, &image->image_view);
}

b8 vulkan_image_create_view_from_image(
    vulkan_context* context,
    VkImage image,
    VkFormat format,
    VkImageAspectFlags aspect_flags,
    VkImageView* out_view) {
    VkImageViewCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    image_info.image = image;
    image_info.viewType = VK_IMAGE_VIEW_TYPE_2D; // TODO: Make this a parameter configurable
    image_info.format = format;
    image_info.subresourceRange.aspectMask = aspect_flags;

    image_info.subresourceRange.baseMipLevel = 0;
    image_info.subresourceRange.levelCount = 1;
    image_info.subresourceRange.baseArrayLayer = 0;
    image_info.subresourceRange.layerCount = 1;

    VK_CHECK_RESULT(vkCreateImageView(context->device.logical_device, &image_info, NULL, out_view));

    return TRUE;
}

void vulkan_image_destroy(vulkan_context* context, vulkan_image* image) {
    if (image->image_view != 0) {
        vkDestroyImageView(context->device.logical_device, image->image_view, context->allocator);
        image->image_view = 0;
    }
    if (image->image_handle != 0) {
        vkDestroyImage(context->device.logical_device, image->image_handle, context->allocator);
        image->image_handle = 0;
    }
    if (image->memory_handle != 0) {
        vkFreeMemory(context->device.logical_device, image->memory_handle, context->allocator);
        image->memory_handle = 0;
    }
}
