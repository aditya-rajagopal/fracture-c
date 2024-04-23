#include "vulkan_commandbuffer.h"

#include "fracture/core/systems/fracture_memory.h"
#include "fracture/core/systems/logging.h"
#include "vulkan/vulkan_core.h"

b8 vulkan_command_buffer_allocate(vulkan_context* context,
                                  VkCommandPool command_pool,
                                  b8 is_primary,
                                  vulkan_command_buffer* out_command_buffers) {
    fr_memory_zero(out_command_buffers, sizeof(vulkan_command_buffer));
    out_command_buffers->state = COMMAND_BUFFER_STATE_NOT_ALLOCATED;
    // Allocate the command buffer
    VkCommandBufferAllocateInfo alloc_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    // Set the command pool to allocate from
    alloc_info.commandPool = command_pool;
    // Set the level of the command buffer
    // If the command buffer is primary, it can be submitted to a queue for
    // execution If the command buffer is secondary, it can't be submitted
    // directly, but can be executed from a primary command buffer
    alloc_info.level = is_primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    // Set the number of command buffers to allocate we are going to allocate 1 as a hard coded value
    alloc_info.commandBufferCount = 1;
    alloc_info.pNext = NULL;
    VK_CHECK_RESULT(
        vkAllocateCommandBuffers(context->device.logical_device, &alloc_info, &out_command_buffers->handle));
    out_command_buffers->state = COMMAND_BUFFER_STATE_READY;
    return TRUE;
}

void vulkan_command_buffer_free(vulkan_context* context,
                                VkCommandPool command_pool,
                                vulkan_command_buffer* command_buffers) {
    if (command_buffers->handle != VK_NULL_HANDLE) {
        vkFreeCommandBuffers(context->device.logical_device, command_pool, 1, &command_buffers->handle);
        command_buffers->handle = VK_NULL_HANDLE;
        command_buffers->state = COMMAND_BUFFER_STATE_NOT_ALLOCATED;
        return;
    }
    FR_CORE_WARN("Attempting to free a command buffer handle that is NULL or not allocated");
}

b8 vulkan_command_buffer_begin(vulkan_command_buffer* command_buffer,
                               b8 is_single_use,
                               b8 is_renderpass_continuation,
                               b8 is_simultaneous_use) {
    VkCommandBufferBeginInfo begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    // We are not currently using the pInheritanceInfo and pNext fields
    begin_info.flags = 0;
    begin_info.pInheritanceInfo = NULL;
    begin_info.pNext = NULL;
    if (is_single_use) {
        // You should use this flag if you know for sure that you will only
        // submit the command buffer once and then reset it
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    }
    if (is_renderpass_continuation) {
        // This flag is for secondary command buffers that will be executed within a renderpass
        // This is ignored for primary command buffers
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    }
    if (is_simultaneous_use) {
        // This flag is for command buffers that can submitted to a queue
        // multiple times while it is in the pending state.
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    }
    VK_CHECK_RESULT(vkBeginCommandBuffer(command_buffer->handle, &begin_info));
    command_buffer->state = COMMAND_BUFFER_STATE_RECORDING;
    return TRUE;
}

b8 vulkan_command_buffer_end(vulkan_command_buffer* command_buffer) {
    // TODO: Check if the command buffer is in a valid state to transition to recording finished
    VK_CHECK_RESULT(vkEndCommandBuffer(command_buffer->handle));
    command_buffer->state = COMMAND_BUFFER_STATE_RECORDING_FINISHED;
    return TRUE;
}

void vulkan_command_buffer_update_submitted(vulkan_command_buffer* command_buffer) {
    // TODO: Check if the command buffer is in a valid state to transition to submitted
    command_buffer->state = COMMAND_BUFFER_STATE_SUBMITTED;
}

b8 vulkan_command_buffer_reset(vulkan_command_buffer* command_buffer) {
    // TODO: Check if the command buffer is in a valid state to reset
    command_buffer->state = COMMAND_BUFFER_STATE_READY;
    return TRUE;
}

void vulkan_command_buffer_allocate_and_begin_single_use(vulkan_context* context,
                                                         VkCommandPool command_pool,
                                                         vulkan_command_buffer* out_command_buffer) {
    // Single use command buffers are usually primary command buffers
    vulkan_command_buffer_allocate(context, command_pool, TRUE, out_command_buffer);
    vulkan_command_buffer_begin(out_command_buffer, TRUE, FALSE, FALSE);
}

b8 vulkan_command_buffer_end_single_use(vulkan_context* context,
                                        VkCommandPool command_pool,
                                        vulkan_command_buffer* command_buffer,
                                        VkFence fence,
                                        VkQueue queue) {
    // End the command buffer
    vulkan_command_buffer_end(command_buffer);

    // Submit the command buffer
    VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer->handle;

    VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submit_info, fence));
    vulkan_command_buffer_update_submitted(command_buffer);
    VK_CHECK_RESULT(vkQueueWaitIdle(queue));
    vulkan_command_buffer_free(context, command_pool, command_buffer);
    return TRUE;
}
