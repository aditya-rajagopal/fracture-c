/**
 * @file vulkan_commandbuffer.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-02-27
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include "vulkan/vulkan_core.h"
#include "vulkan_types.h"

/**
 * @brief Allocate a vulkan command buffer.
 * @details This function allocates a vulkan command buffer from the given
 * command pool. Command buffers are allocated and not created. The command
 * buffer can be either primary or secondary. Primary command buffers are the
 * ones that can be submitted to a queue for execution, but cannot be called
 * from other command buffers. Secondary command buffers can be called from
 * primary command buffers.
 *
 * @param context The vulkan context.
 * @param command_pool The command pool to allocate the command buffer from.
 * @param is_primary Whether the command buffer is primary or secondary.
 * @param out_command_buffers The vulkan command buffer struct to be allocated.
 * @return b8 Whether the command buffer was allocated successfully.
 */
b8 vulkan_command_buffer_allocate(vulkan_context* context,
                                  VkCommandPool command_pool, b8 is_primary,
                                  vulkan_command_buffer* out_command_buffers);

/**
 * @brief Returns the command buffer resources to the command pool.
 * @details Command buffers are allocated from a command pool. Once we are done
 * we do not delete the command buffer, we return it to the command pool to be
 * used again.
 *
 * @param context The vulkan context.
 * @param command_pool The command pool to return the command buffer to.
 * @param command_buffers The vulkan command buffer struct to be freed.
 */
void vulkan_command_buffer_free(vulkan_context* context,
                                VkCommandPool command_pool,
                                vulkan_command_buffer* command_buffers);

/**
 * @brief Begin recording commands to the given vulkan command buffer.
 * 
 * @param command_buffer The vulkan command buffer to record commands to.
 * @param is_single_use If the command buffer is to be used only once.
 * @param is_renderpass_continuation If the command buffer is a continuation of a renderpass.
 * @param is_simultaneous_use If the command buffer can be used simultaneously.
 * @return b8 Whether the command buffer was begun successfully.
 */
b8 vulkan_command_buffer_begin(vulkan_command_buffer* command_buffer, b8 is_single_use,
                                 b8 is_renderpass_continuation,
                                 b8 is_simultaneous_use);

/**
 * @brief End recording commands to the given vulkan command buffer.
 * 
 * @param command_buffer The vulkan command buffer to record commands to.
 * @return b8 Whether the command buffer was ended successfully.
 */
b8 vulkan_command_buffer_end(vulkan_command_buffer* command_buffer);

/**
 * @brief updates the current command buffer state to submitted 
 * @param command_buffer The vulkan command buffer to submit.
 */
void vulkan_command_buffer_update_submitted(vulkan_command_buffer* command_buffer);

/**
 * @brief Reset the given vulkan command buffer back to the ready state.
 * 
 * @param command_buffer The vulkan command buffer to reset.
 */
b8 vulkan_command_buffer_reset(vulkan_command_buffer* command_buffer);

/**
 * @brief Allocate and begin a single use command buffer.
 * @details There are often situations where we need to record commands to a
 * command buffer and submit it to the queue only once. This function allocates
 * a command buffer from the given command pool and begins recording commands to
 * it. The command buffer is allocated as a primary command buffer and is used
 * only once.
 *
 * @param context The vulkan context.
 * @param command_pool The command pool to allocate the command buffer from.
 * @param out_command_buffer The vulkan command buffer struct to be allocated.
 */
void vulkan_command_buffer_allocate_and_begin_single_use(vulkan_context* context,
                                                         VkCommandPool command_pool,
                                                         vulkan_command_buffer* out_command_buffer);

/**
 * @brief End the given single use command buffer and submit it to the queue.
 * @details Once we are done recording commands to a single use command buffer,
 * we end the recording and submit the command buffer to the queue. The command
 * buffer is then returned to the command pool to be used again.
 *
 * @param context The vulkan context.
 * @param command_pool The command pool to return the command buffer to.
 * @param command_buffer The vulkan command buffer to be submitted.
 * @param queue The queue to submit the command buffer to.
 * @return b8 Whether the command buffer was ended and submitted successfully.
 */
b8 vulkan_command_buffer_end_single_use(vulkan_context* context,
                                          VkCommandPool command_pool,
                                          vulkan_command_buffer* command_buffer,
                                          VkFence fence,
                                          VkQueue queue);
