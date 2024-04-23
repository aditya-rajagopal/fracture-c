#include "vulkan_fence.h"

#include "fracture/core/systems/logging.h"
#include "vulkan/vulkan_core.h"

b8 vulkan_fence_create(vulkan_context* context, vulkan_fence* out_fence, b8 create_singnaled) {
    out_fence->is_signaled = create_singnaled;
    VkFenceCreateInfo fence_create_info = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    fence_create_info.flags = create_singnaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

    VK_CHECK_RESULT(
        vkCreateFence(context->device.logical_device, &fence_create_info, context->allocator, &out_fence->handle));
    return TRUE;
}

void vulkan_fence_destroy(vulkan_context* context, vulkan_fence* fence) {
    if (fence && fence->handle != VK_NULL_HANDLE) {
        vkDestroyFence(context->device.logical_device, fence->handle, context->allocator);
        fence->handle = VK_NULL_HANDLE;
        fence->is_signaled = FALSE;
        return;
    }
    FR_CORE_WARN("Attempting to destyroy a Fence handle that is NULL or not allocated");
}

b8 vulkan_fence_wait(vulkan_context* context, vulkan_fence* fence, u64 timeout_ns) {
    if (fence->is_signaled) {
        return TRUE;
    }
    VkResult result = vkWaitForFences(context->device.logical_device, 1, &fence->handle, VK_TRUE, timeout_ns);
    switch (result) {
        case VK_SUCCESS:
            fence->is_signaled = TRUE;
            return TRUE;
        case VK_TIMEOUT:
            FR_CORE_WARN("Fence wait timed out");
            break;
        case VK_ERROR_DEVICE_LOST:
            FR_CORE_ERROR("Device lost while waiting for fence");
            break;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            FR_CORE_ERROR("Out of host memory while waiting for fence");
            break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            FR_CORE_ERROR("Out of device memory while waiting for fence");
            break;
        default:
            FR_CORE_ERROR("Unknown error while waiting for fence");
            break;
    }
    return FALSE;
}

b8 vulkan_fence_reset(vulkan_context* context, vulkan_fence* fence) {
    if (fence->is_signaled) {
        fence->is_signaled = FALSE;
        VK_CHECK_RESULT(vkResetFences(context->device.logical_device, 1, &fence->handle));
        return TRUE;
    }
    return FALSE;
}
