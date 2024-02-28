#include "vulkan_framebuffer.h"

#include "fracture/core/systems/logging.h"
#include "fracture/core/systems/fracture_memory.h"
#include "vulkan/vulkan_core.h"

b8 vulkan_framebuffer_create(vulkan_context* context,
                             vulkan_renderpass* renderpass,
                             u32 width, u32 height,
                             u32 attachment_count,
                             VkImageView* pAttachments,
                             vulkan_frame_buffer* out_framebuffer) {
    // Copy the attachments to the framebuffer
    out_framebuffer->attachment_count = attachment_count;
    out_framebuffer->pAttachments = fr_memory_allocate(
        sizeof(VkImageView) * attachment_count, MEMORY_TYPE_RENDERER);
    fr_memory_copy(out_framebuffer->pAttachments, pAttachments,
                   sizeof(VkImageView) * attachment_count);
    out_framebuffer->renderpass = renderpass;
    out_framebuffer->width = width;
    out_framebuffer->height = height;

    // Create the framebuffer
    VkFramebufferCreateInfo framebuffer_create_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
    // The renderpass to attach the framebuffer to
    framebuffer_create_info.renderPass = renderpass->handle;
    // The number of attachments
    framebuffer_create_info.attachmentCount = attachment_count;
    // The attachments
    framebuffer_create_info.pAttachments = out_framebuffer->pAttachments;
    // The width of the framebuffer
    framebuffer_create_info.width = width;
    // The height of the framebuffer
    framebuffer_create_info.height = height;
    // The number of layers in the framebuffer
    framebuffer_create_info.layers = 1;

    VK_CHECK_RESULT(vkCreateFramebuffer(
        context->device.logical_device, &framebuffer_create_info,
        context->allocator, &out_framebuffer->handle));
    return TRUE;
}

void vulkan_framer_buffer_destroy(vulkan_context* context,
                                  vulkan_frame_buffer* framebuffer) {
    if (framebuffer && framebuffer->handle != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(context->device.logical_device, framebuffer->handle, context->allocator);
        if (framebuffer->pAttachments) {
            fr_memory_free(framebuffer->pAttachments,
                           sizeof(VkImageView) * framebuffer->attachment_count,
                           MEMORY_TYPE_RENDERER);
            framebuffer->pAttachments = NULL;
        }
        framebuffer->handle = VK_NULL_HANDLE;
        framebuffer->attachment_count = 0;
        framebuffer->renderpass = NULL;
        return;
    }
    FR_CORE_WARN("Attempting to destroy a framebuffer handle that is NULL or not allocated");
}

