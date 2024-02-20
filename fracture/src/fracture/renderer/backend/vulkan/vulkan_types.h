/**
 * @file vulkan_types.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-02-19
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */

#pragma once

#include "fracture/fracture_core.h"

#include <vulkan/vulkan.h>

#define VK_CHECK_RESULT(f)                                                     \
  {                                                                            \
    VkResult res = (f);                                                        \
    if (res != VK_SUCCESS) {                                                   \
      FR_CORE_FATAL("Vulkan error %s : %d", #f, res);                          \
      return FALSE;                                                            \
    }                                                                          \
  }

typedef struct vulkan_context {
    VkInstance instance;
    VkAllocationCallbacks* allocator;

#if defined(FR_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
} vulkan_context;
