#include "vulkan_swapchain.h"
#include "vulkan/vulkan_core.h"
#include "vulkan_device.h"
#include "vulkan_image.h"

#include "fracture/core/systems/logging.h"
#include "fracture/core/systems/fracture_memory.h"

b8 _swapchain_create(vulkan_context* context, u32 width, u32 height, vulkan_swapchain* out_swapchain);
b8 _swapchain_destroy(vulkan_context* context, vulkan_swapchain* swapchain);

b8 vulkan_swapchain_create(vulkan_context* context, u32 width, u32 height, vulkan_swapchain* out_swapchain) {
    return _swapchain_create(context, width, height, out_swapchain);
}

b8 vulkan_swapchain_destroy(vulkan_context* context, vulkan_swapchain* swapchain) {
    return _swapchain_destroy(context, swapchain);
} 

b8 vulkan_swapchain_recreate(vulkan_context *context, u32 width, u32 height, vulkan_swapchain *swapchain) {
    if (!_swapchain_destroy(context, swapchain)) {
        FR_CORE_FATAL("Failed to destroy swapchain");
        return FALSE;
    }
    if (!_swapchain_create(context, width, height, swapchain)) {
        FR_CORE_FATAL("Failed to recreate swapchain");
        return FALSE;
    }
    FR_CORE_INFO("Swapchain recreated with new dimensions: %d x %d", width, height);
    return TRUE;
}

b8 vulkan_swapchain_get_next_image_index(vulkan_context* context,
                                         vulkan_swapchain* swapchain,
                                         u64 timeout_ns,
                                         VkSemaphore image_available_semaphore,
                                         VkFence in_flight_fence,
                                         u32* out_image_index) {
    /* There are 4 results that are technically not errors
     * VK_NOT_READY: The requested operation has not completed yet
     * VK_TIMEOUT: The requested operation has not completed in the specified time
     * VK_SUBOPTIMAL_KHR: The swapchain no longer matches the surface properties exactly, but can still be used
     * VK_SUCCESS: The requested operation completed successfully
     * any other result is an error.
     */
    VkResult result = vkAcquireNextImageKHR(context->device.logical_device, swapchain->handle,
                          timeout_ns, image_available_semaphore,
                          in_flight_fence, out_image_index);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        // The swapchain is out of date, we need to recreate it
        vulkan_swapchain_recreate(context, context->framebuffer_width, context->framebuffer_height, swapchain);
        return FALSE;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        // Something went wrong and we can't continue rendering
        FR_CORE_FATAL("Failed to acquire next image Error Code: %d", result);
        return FALSE;
    }
    return TRUE;
}

void vulkan_swapchain_present(vulkan_context* context,
                              vulkan_swapchain* swapchain,
                              VkQueue graphics_queue,
                              VkQueue present_queue,
                              u32 present_image_index,
                              VkSemaphore render_finished_semaphore) {
    // First we need to return the image to the swapchain to be presented
    VkPresentInfoKHR present_info = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
    present_info.waitSemaphoreCount = 1; // Number of semaphores to wait on
    // Presenting is a queued operation, so we need to wait for the rendering to finish
    present_info.pWaitSemaphores = &render_finished_semaphore;
    present_info.swapchainCount = 1;  // Number of swapchains to present
    present_info.pSwapchains = &swapchain->handle;
    present_info.pImageIndices = &present_image_index; // The index of the image to present
    present_info.pResults = NULL;                      // Optional

    VkResult result = vkQueuePresentKHR(present_queue, &present_info);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        // The swapchain is out of date, we need to recreate it
        vulkan_swapchain_recreate(context, context->framebuffer_width, context->framebuffer_height, swapchain);
    } else if (result != VK_SUCCESS) {
        // Something went wrong and we can't continue rendering
        FR_CORE_FATAL("Failed to present swapchain image Error Code: %d", result);
    }
}

// ---------------------------------------------------------------------------------------------------------------
// ---------------------------------------- PRIVATE FUNCTIONS ----------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------

b8 _swapchain_create(vulkan_context* context, u32 width, u32 height, vulkan_swapchain* out_swapchain) {
    VkExtent2D extent = {width, height};
    out_swapchain->max_frames_in_flight = 2; // TODO: Make this configurable

    // Choose a swap surface format. We want to use the sRGB color space and 8bit per channel format and RGBA.
    // Most modern graphics cards should support this format.
    b8 found_format = FALSE;
    for (u32 i = 0; i < context->device.swapchain_support.format_count; ++i) {
        VkSurfaceFormatKHR format = context->device.swapchain_support.formats[i];
        if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            out_swapchain->format = format;
            found_format = TRUE;
            break;
        }
    }

    if (!found_format) {
        // If we did not find the formats we wanted, warn the user and use the
        // first format available and hope for the best
        FR_CORE_WARN(
            "Failed to find suitable swap surface format. Using the default "
            "format %d and hoping for the best.",
            context->device.swapchain_support.formats[0].format);
        out_swapchain->format = context->device.swapchain_support.formats[0];
    }

    // Choose the presentation mode. We want to use the mailbox mode if it is available, as it is the lowest latency
    // mode available. If it is not available, we will use the FIFO mode, which is guaranteed to be available.
    // We could also use the immediate mode, but it is not guaranteed to be available and it has no vsync.
    // TODO: Make the presentation mode configurable
    VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
    for (u32 i = 0; i < context->device.swapchain_support.present_mode_count; ++i) {
        if (context->device.swapchain_support.present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        }
    }

    // We will requery the swapchain capabilities just in case there was a
    // change in the surface properties like a new window size or monitor change
    // etc. since the last time we queried the capabilities.
    vulkan_device_query_swapchain_support(context->device.physical_device,
                                          context->surface,
                                          &context->device.swapchain_support);

    // We are putting a safeguard here to make sure that the swapchain extent is
    // not something that we cannot handle.
    if (context->device.swapchain_support.capabilities.currentExtent.width != UINT32_MAX) {
        extent = context->device.swapchain_support.capabilities.currentExtent;
    }

    // We also want to clamp the width and height to the min and max extent that the GPU allows
    VkExtent2D min = context->device.swapchain_support.capabilities.minImageExtent;
    VkExtent2D max = context->device.swapchain_support.capabilities.maxImageExtent;
    extent.width = CLAMP(extent.width, min.width, max.width);
    extent.height = CLAMP(extent.height, min.height, max.height);

    // We are going to query the capabilities of the swapchain to see how many images we can have in the swapchain.
    // This is usually going to be 2. 
    u32 image_count = context->device.swapchain_support.capabilities.minImageCount + 1;
    // But just in case the maxImageCount is not 0, we will clamp the image count to the maxImageCount.
    if (context->device.swapchain_support.capabilities.maxImageCount > 0 &&
        image_count > context->device.swapchain_support.capabilities.maxImageCount) {
        image_count = context->device.swapchain_support.capabilities.maxImageCount;
    }

    // Now we are going to create the swapchain create info
    VkSwapchainCreateInfoKHR create_info = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    create_info.surface = context->surface; // The surface to create the swapchain on
    create_info.minImageCount = image_count; // The minimum number of images in the swapchain
    create_info.imageFormat = out_swapchain->format.format; // The format of the images in the swapchain
    create_info.imageColorSpace = out_swapchain->format.colorSpace; // The color space of the images in the swapchain
    create_info.imageExtent = extent; // The extent of the images in the swapchain that we checked 10 times to make sure it is valid
    create_info.imageArrayLayers = 1; // The number of layers in the images in the swapchain
    // The usage of the images in the swapchain (we are going to use it as a
    // color attachment which is the colour buffer)
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // We need to setup the queue families that are going to be used for the swapchain
    if (context->device.graphics_family_index != context->device.present_family_index) {
        u32 queue_family_indices[] = {
            (u32)context->device.graphics_family_index,
            (u32)context->device.present_family_index,
        };
        // If the graphics and present queues are different, we are going to use
        // concurrent mode for the swapchain where we can use the images in the
        // swapchain in multiple queues at the same time. We can work on one
        // image(with graphics commands) of the swapchain while presenting
        // another.
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0; // Optional
        create_info.pQueueFamilyIndices = NULL; // Optional
    }

    // The transform to be applied to the images in the swapchain
    create_info.preTransform = context->device.swapchain_support.capabilities.currentTransform;
    // The composite alpha mode to be used with the images in the swapchain. We
    // will use the opaque mode as we are not going to blend the images with the
    // operating system.
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode; // The presentation mode to be used with the swapchain
    create_info.clipped = VK_TRUE; // We are going to clip the pixels that are obscured by other windows
    create_info.oldSwapchain = VK_NULL_HANDLE; // We are not going to use an old swapchain especially when we are recreating the swapchain

    // Now finally we are going to create the swapchain
    VK_CHECK_RESULT(vkCreateSwapchainKHR(context->device.logical_device, &create_info, context->allocator, &out_swapchain->handle));
    FR_CORE_TRACE("Swapchain created");
    // Now we need to create the images and image views for the swapchain
    // In vulkan an image is typically explicitly created and then an image view
    // But for the swapchain, the images are created implicitly when the swapchain is created and it is also the one that destroys the images.
    // So we are going to query the images from the swapchain and then create the image views for them.

    // Start current frame index at 0
    context->current_frame_index = 0;

    // Query the images from the swapchain
    out_swapchain->image_count = 0;
    VK_CHECK_RESULT(vkGetSwapchainImagesKHR(context->device.logical_device, out_swapchain->handle, &out_swapchain->image_count, NULL));
    if (!out_swapchain->images) {
        // Only if we have not already allocated space to store the pointers to the images
        out_swapchain->images = (VkImage*)fr_memory_allocate(sizeof(VkImage) * out_swapchain->image_count, MEMORY_TYPE_RENDERER);
    }
    if (!out_swapchain->image_views) {
        // Only if we have not already allocated space to store the pointers to the image views
        out_swapchain->image_views = (VkImageView*)fr_memory_allocate(sizeof(VkImageView) * out_swapchain->image_count, MEMORY_TYPE_RENDERER);
    }

    VK_CHECK_RESULT(vkGetSwapchainImagesKHR(context->device.logical_device, out_swapchain->handle, &out_swapchain->image_count, out_swapchain->images));
    
    // Though the swapchain creates the images we have to manually create the image views for the images
    for (u32 i = 0; i < out_swapchain->image_count; ++i) {
        if(!vulkan_image_create_view_from_image(
                context,
                out_swapchain->images[i],
                out_swapchain->format.format,
                VK_IMAGE_ASPECT_COLOR_BIT,
                &out_swapchain->image_views[i])) {
            FR_CORE_FATAL("Failed to create image view for swapchain image %d", i);
            return FALSE;
        }
    }
    FR_CORE_TRACE("Created %d image views for the swapchain images", out_swapchain->image_count);

    // We are also going to create a resource for the depth buffer that we are going to use for the swapchain
    if(!vulkan_device_detect_depth_format(&context->device)) {
        // If we cannot detect a depth format that is supported, we are going to
        // return false as this is not an error that we can recover from
        context->device.depth_format = VK_FORMAT_UNDEFINED;
        FR_CORE_FATAL("Failed to detect depth format that is supported");
        return FALSE;
    }

    // Create a depth image and image view for the swapchain
    vulkan_image_create(
        context,
        VK_IMAGE_TYPE_2D,
        extent.width,
        extent.height,
        context->device.depth_format,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        TRUE,
        VK_IMAGE_ASPECT_DEPTH_BIT,
        &out_swapchain->depth_attachment);
    
    FR_CORE_TRACE("Created depth attachment for swapchain");

    FR_CORE_INFO("Swapchain created with %d images", out_swapchain->image_count);
    return TRUE;
}

b8 _swapchain_destroy(vulkan_context* context, vulkan_swapchain* swapchain) {
    vulkan_image_destroy(context, &swapchain->depth_attachment);
    FR_CORE_TRACE("Destroyed depth attachment for swapchain");
    // Destroy the image views we dont need to destroy the images as they are implicitly destroyed when the swapchain is destroyed
    for (u32 i = 0; i < swapchain->image_count; ++i) {
        vkDestroyImageView(context->device.logical_device, swapchain->image_views[i], context->allocator);
    }
    FR_CORE_TRACE("Destroyed %d image views", swapchain->image_count);
    // Destroy the swapchain
    vkDestroySwapchainKHR(context->device.logical_device, swapchain->handle, context->allocator);
    FR_CORE_INFO("Destroyed swapchain with %d images", swapchain->image_count);
    return TRUE;
}