#include "vulkan_device.h"

#include "fracture/core/containers/darrays.h"
#include "fracture/core/library/fracture_string.h"
#include "fracture/core/systems/fracture_memory.h"
#include "fracture/core/systems/logging.h"
#include "vulkan/vulkan_core.h"
#include "vulkan_types.h"

typedef struct vulkan_physical_device_requriements {
    /** @brief Boolean if we have a graphics queue */
    b8 graphics;
    /** @brief Boolean if we have a present queue */
    b8 present;
    /** @brief Boolean if we have a compute queue for compute shaders*/
    b8 compute;
    /** @brief Boolean if we have a transfer queue for tansfering data*/
    b8 transfer;
    /** @brief Keep a copy of the device extension names */
    const char** device_extension_names;
    /** @brief If sampler_anisotropy is avialable */
    b8 sampler_anisotropy;
    /** @brief If the device is a discrete GPU */
    b8 discrete_gpu;
} vulkan_physical_device_requriements;

typedef struct vulkan_physical_device_queue_families {
    u32 graphics_family_index;
    u32 present_family_index;
    u32 compute_family_index;
    u32 transfer_family_index;
} vulkan_physical_device_queue_families_info;

b8 _device_select_physical_device(vulkan_context* contect);

b8 _device_physical_device_meets_requirements(VkPhysicalDevice device,
                                              VkSurfaceKHR surface,
                                              const VkPhysicalDeviceProperties* properties,
                                              const VkPhysicalDeviceFeatures* features,
                                              const VkPhysicalDeviceMemoryProperties* memory,
                                              const vulkan_physical_device_requriements* requirements,
                                              vulkan_physical_device_queue_families_info* out_queue_families,
                                              vulkan_swapchain_support_info* out_swapchain_support);

b8 _device_check_physical_device_extension_support(const vulkan_physical_device_requriements* requirements,
                                                   VkPhysicalDevice device,
                                                   const VkPhysicalDeviceProperties* properties);

b8 _device_create_logical_device(vulkan_context* context);
b8 _device_get_device_queue_handles(vulkan_context* context);
b8 _device_create_graphics_command_pool(vulkan_context* context);
void _device_destroy_graphics_command_pool(vulkan_context* context);

b8 vulkan_create_device(vulkan_context* context) {
    if (!_device_select_physical_device(context)) {
        FR_CORE_FATAL("Failed to select a physical device");
        return FALSE;
    }
    FR_CORE_INFO("Physical device selected");
    // Create the logical device
    if (!_device_create_logical_device(context)) {
        FR_CORE_FATAL("Failed to create the logical device");
        return FALSE;
    }

    // Get the device queue handles
    if (!_device_get_device_queue_handles(context)) {
        FR_CORE_FATAL("Failed to get the device queue handles");
        return FALSE;
    }

    if (!_device_create_graphics_command_pool(context)) {
        FR_CORE_FATAL("Failed to create the command pool");
        return FALSE;
    }

    return TRUE;
}

void vulkan_destroy_device(vulkan_context* context) {
    // Destroy the graphics command pool
    _device_destroy_graphics_command_pool(context);
    // Unset the queue handles
    context->device.graphics_queue_handle = 0;
    context->device.present_queue_handle = 0;
    context->device.compute_queue_handle = 0;
    context->device.transfer_queue_handle = 0;
    FR_CORE_INFO("Vulkan queue handles unset");
    // Destroy the logical device
    if (context->device.logical_device) {
        vkDestroyDevice(context->device.logical_device, 0);
        FR_CORE_INFO("Vulkan Logical device destroyed");
    }
    // Free the swapchain support
    if (context->device.swapchain_support.formats) {
        fr_memory_free(context->device.swapchain_support.formats,
                       sizeof(VkSurfaceFormatKHR) * context->device.swapchain_support.format_count,
                       MEMORY_TYPE_RENDERER);
    }
    if (context->device.swapchain_support.present_modes) {
        fr_memory_free(context->device.swapchain_support.present_modes,
                       sizeof(VkPresentModeKHR) * context->device.swapchain_support.present_mode_count,
                       MEMORY_TYPE_RENDERER);
    }
    fr_memory_zero(&context->device.swapchain_support, sizeof(vulkan_swapchain_support_info));
    context->device.graphics_family_index = UINT32_MAX;
    context->device.present_family_index = UINT32_MAX;
    context->device.compute_family_index = UINT32_MAX;
    context->device.transfer_family_index = UINT32_MAX;
    FR_CORE_INFO("Vulkan swapchain support released");
    // Release the physical device
    context->device.physical_device = 0;
    FR_CORE_INFO("Vulkan Physical device released");
}

b8 vulkan_device_query_swapchain_support(VkPhysicalDevice device,
                                         VkSurfaceKHR surface,
                                         vulkan_swapchain_support_info* out_support_info) {
    // Get surface capabilities
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &out_support_info->capabilities));

    // Get surface fromats
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &out_support_info->format_count, 0));
    if (out_support_info->format_count != 0) {
        if (!out_support_info->formats) {
            out_support_info->formats =
                fr_memory_allocate(sizeof(VkSurfaceFormatKHR) * out_support_info->format_count, MEMORY_TYPE_RENDERER);
        }
        VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(
            device, surface, &out_support_info->format_count, out_support_info->formats));
    }

    // Get present modes
    VK_CHECK_RESULT(
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &out_support_info->present_mode_count, 0));
    if (out_support_info->present_mode_count != 0) {
        if (!out_support_info->present_modes) {
            out_support_info->present_modes = fr_memory_allocate(
                sizeof(VkPresentModeKHR) * out_support_info->present_mode_count, MEMORY_TYPE_RENDERER);
        }
        VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(
            device, surface, &out_support_info->present_mode_count, out_support_info->present_modes));
    }

    return TRUE;
}

b8 vulkan_device_detect_depth_format(vulkan_device* device) {
    const u64 candidate_count = 3;
    // We prefere 3 formats in this order
    // 1. VK_FORMAT_D32_SFLOAT: One component 32-bit float depth buffer format that uses all 32 bits for depth
    // 2. VK_FORMAT_D32_SFLOAT_S8_UINT: A combined depth/stencil format with 32-bit float depth and 8-bit stencil
    // 3. VK_FORMAT_D24_UNORM_S8_UINT: A combined depth/stencil format with 24-bit depth and 8-bit stencil
    VkFormat candidates[3] = {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT};
    u32 flags = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
    for (u64 i = 0; i < candidate_count; ++i) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(device->physical_device, candidates[i], &props);

        if ((props.linearTilingFeatures & flags) == flags || (props.optimalTilingFeatures & flags) == flags) {
            device->depth_format = candidates[i];
            return TRUE;
        }
    }
    FR_CORE_ERROR("Failed to find a suitable depth format");
    return FALSE;
}

// ------------------------------------------------------------------
// ------------------------ PRIVATE FUNCTIONS -----------------------
// ------------------------------------------------------------------

b8 _device_select_physical_device(vulkan_context* context) {
    u32 physical_device_count = 0;
    VK_CHECK_RESULT(vkEnumeratePhysicalDevices(context->instance, &physical_device_count, 0));
    if (physical_device_count == 0) {
        FR_CORE_FATAL("No physical devices which support Vulkan were found! terminating");
        return FALSE;
    }
    VkPhysicalDevice physical_devices[physical_device_count];
    VK_CHECK_RESULT(vkEnumeratePhysicalDevices(context->instance, &physical_device_count, physical_devices));
    // TODO: Get requirements from the engine/application
    vulkan_physical_device_requriements requirements = {};
    requirements.graphics = TRUE;
    requirements.present = TRUE;
    requirements.compute = TRUE;
    requirements.transfer = TRUE;
    requirements.device_extension_names = darray_create(const char*);
    darray_push(requirements.device_extension_names, &VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    requirements.sampler_anisotropy = TRUE;
    requirements.discrete_gpu = TRUE;

    for (u32 i = 0; i < physical_device_count; ++i) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physical_devices[i], &properties);

        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(physical_devices[i], &features);

        VkPhysicalDeviceMemoryProperties memory;
        vkGetPhysicalDeviceMemoryProperties(physical_devices[i], &memory);

        vulkan_physical_device_queue_families_info queue_families = {};
        b8 result = _device_physical_device_meets_requirements(physical_devices[i],
                                                               context->surface,
                                                               &properties,
                                                               &features,
                                                               &memory,
                                                               &requirements,
                                                               &queue_families,
                                                               &context->device.swapchain_support);
        if (result) {
            FR_CORE_TRACE("Selected physical device: %s", properties.deviceName);
            switch (properties.deviceType) {
                case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                    FR_CORE_TRACE("Device type: Other");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                    FR_CORE_TRACE("Device type: Integrated GPU");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                    FR_CORE_TRACE("Device type: Discrete GPU");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                    FR_CORE_TRACE("Device type: Virtual GPU");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_CPU:
                    FR_CORE_TRACE("Device type: CPU");
                    break;
                default:
                    FR_CORE_TRACE("Device type: Unknown");
                    break;
            }
            FR_CORE_TRACE("GPU Driver version: %d.%d.%d",
                          VK_VERSION_MAJOR(properties.driverVersion),
                          VK_VERSION_MINOR(properties.driverVersion),
                          VK_VERSION_PATCH(properties.driverVersion));
            FR_CORE_TRACE("API Version: %d.%d.%d",
                          VK_VERSION_MAJOR(properties.apiVersion),
                          VK_VERSION_MINOR(properties.apiVersion),
                          VK_VERSION_PATCH(properties.apiVersion));

            for (u32 j = 0; j < memory.memoryTypeCount; ++j) {
                f32 memory_size_gb = (f32)memory.memoryHeaps[j].size / (1024.0f * 1024.0f * 1024.0f);
                if (memory.memoryTypes[j].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
                    FR_CORE_TRACE("Device local memory: %.2f GB", memory_size_gb);
                } else {
                    FR_CORE_TRACE("Shared System memory: %.2f GB", memory_size_gb);
                }
            }

            context->device.physical_device = physical_devices[i];
            context->device.properties = properties;
            context->device.features = features;
            context->device.memory_properties = memory;

            context->device.graphics_family_index = queue_families.graphics_family_index;
            context->device.present_family_index = queue_families.present_family_index;
            context->device.compute_family_index = queue_families.compute_family_index;
            context->device.transfer_family_index = queue_families.transfer_family_index;

            break;
        }
    }

    darray_destroy(requirements.device_extension_names);
    if (!context->device.physical_device) {
        FR_CORE_ERROR("Failed to find a suitable physical device");
        return FALSE;
    }

    FR_CORE_INFO("Selected physical device: %s", context->device.properties.deviceName);
    return TRUE;
}

b8 _device_physical_device_meets_requirements(VkPhysicalDevice device,
                                              VkSurfaceKHR surface,
                                              const VkPhysicalDeviceProperties* properties,
                                              const VkPhysicalDeviceFeatures* features,
                                              const VkPhysicalDeviceMemoryProperties* memory,
                                              const vulkan_physical_device_requriements* requirements,
                                              vulkan_physical_device_queue_families_info* out_queue_families,
                                              vulkan_swapchain_support_info* out_swapchain_support) {
    // Set the queue families to an invalid index
    out_queue_families->graphics_family_index = UINT32_MAX;
    out_queue_families->present_family_index = UINT32_MAX;
    out_queue_families->compute_family_index = UINT32_MAX;
    out_queue_families->transfer_family_index = UINT32_MAX;

    // Check if the device is a discrete GPU
    if (requirements->discrete_gpu && properties->deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        FR_CORE_TRACE("Device does not meet discrete GPU requirements. Skipping");
        return FALSE;
    }

    // Check if the device has the required queue families
    u32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, 0);
    VkQueueFamilyProperties queue_families[queue_family_count];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);

    FR_CORE_INFO("Queue family count: %d", queue_family_count);
    FR_CORE_INFO("G | P | C | T | Name");
    // We want to find the best queue family for each type of queue
    // Specifically for the transfer queue, we want to find the queue family
    // dedicated to transfer operations so we maintain a score.
    u8 min_transfer_score = UINT8_MAX;
    for (u32 i = 0; i < queue_family_count; ++i) {
        u8 current_transfer_score = 0;
        // Check if the queue family supports graphics commands
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            if (requirements->graphics) {
                out_queue_families->graphics_family_index = i;
                ++current_transfer_score;
            }
        }
        // Check if the queue family supports compute queues
        if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            if (requirements->compute) {
                out_queue_families->compute_family_index = i;
                ++current_transfer_score;
            }
        }
        // Check if the queue family supports transfer queues
        if (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
            if (requirements->transfer) {
                if (current_transfer_score < min_transfer_score) {
                    // If we chose the queue family with the lowest score for transfer
                    // It is likely it is a dedicated transfer queue family
                    min_transfer_score = current_transfer_score;
                    out_queue_families->transfer_family_index = i;
                }
            }
        }

        // Check if the queue family supports present queues
        VkBool32 present_support = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);
        if (present_support) {
            if (requirements->present) {
                out_queue_families->present_family_index = i;
            }
        }
    }
    // Print the queue family information
    FR_CORE_INFO("%d | %d | %d | %d | %s",
                 out_queue_families->graphics_family_index != UINT32_MAX,
                 out_queue_families->present_family_index != UINT32_MAX,
                 out_queue_families->compute_family_index != UINT32_MAX,
                 out_queue_families->transfer_family_index != UINT32_MAX,
                 properties->deviceName);

    // Check if we found a queue family for each type of queue that we need else return false
    if ((!requirements->graphics ||
         (requirements->graphics && out_queue_families->graphics_family_index != UINT32_MAX) ||
         !requirements->present || (requirements->present && out_queue_families->present_family_index != UINT32_MAX) ||
         !requirements->compute || (requirements->compute && out_queue_families->compute_family_index != UINT32_MAX) ||
         !requirements->transfer ||
         (requirements->transfer && out_queue_families->transfer_family_index != UINT32_MAX))) {
        FR_CORE_INFO("Device meets all queue requirements");
        FR_CORE_TRACE("Queue family indices: G: %d, P: %d, C: %d, T: %d",
                      out_queue_families->graphics_family_index,
                      out_queue_families->present_family_index,
                      out_queue_families->compute_family_index,
                      out_queue_families->transfer_family_index);

        // Check if the device supports the swapchain extension
        vulkan_device_query_swapchain_support(device, surface, out_swapchain_support);
        if (out_swapchain_support->format_count < 1 || out_swapchain_support->present_mode_count < 1) {
            // Free the memory allocated for the swapchain support
            if (out_swapchain_support->formats) {
                fr_memory_free(out_swapchain_support->formats,
                               sizeof(VkSurfaceFormatKHR) * out_swapchain_support->format_count,
                               MEMORY_TYPE_RENDERER);
            }
            if (out_swapchain_support->present_modes) {
                fr_memory_free(out_swapchain_support->present_modes,
                               sizeof(VkPresentModeKHR) * out_swapchain_support->present_mode_count,
                               MEMORY_TYPE_RENDERER);
            }
            FR_CORE_TRACE("Device does not support the swapchain extension. Skipping");
            return FALSE;
        }

        // Check if the device has the required device extensions
        if (!_device_check_physical_device_extension_support(requirements, device, properties)) {
            return FALSE;
        }

        // Check if the device has anisotropic filtering
        if (requirements->sampler_anisotropy && !features->samplerAnisotropy) {
            FR_CORE_TRACE(
                "Device does not meet anisotropic filtering "
                "requirements. Skipping");
            return FALSE;
        }
        // If we have met all the requirements, return true
        return TRUE;
    }

    // We did not meet the queue requirements
    return FALSE;
}

b8 _device_check_physical_device_extension_support(const vulkan_physical_device_requriements* requirements,
                                                   VkPhysicalDevice device,
                                                   const VkPhysicalDeviceProperties* properties) {
    if (requirements->device_extension_names) {
        u32 available_extension_count = 0;
        VkExtensionProperties* available_extensions = 0;
        VK_CHECK_RESULT(vkEnumerateDeviceExtensionProperties(device, 0, &available_extension_count, 0));
        if (available_extension_count != 0) {
            available_extensions =
                fr_memory_allocate(sizeof(VkExtensionProperties) * available_extension_count, MEMORY_TYPE_RENDERER);
            VK_CHECK_RESULT(
                vkEnumerateDeviceExtensionProperties(device, 0, &available_extension_count, available_extensions));
            u64 required_extension_count = darray_length(requirements->device_extension_names);
            for (u64 i = 0; i < required_extension_count; ++i) {
                b8 found = FALSE;
                for (u64 j = 0; j < available_extension_count; ++j) {
                    if (fr_string_compare(requirements->device_extension_names[i],
                                          available_extensions[j].extensionName)) {
                        found = TRUE;
                        break;
                    }
                }
                if (!found) {
                    fr_memory_free(available_extensions,
                                   sizeof(VkExtensionProperties) * available_extension_count,
                                   MEMORY_TYPE_RENDERER);
                    FR_CORE_TRACE(
                        "Device: %s does not support the required "
                        "extension: %s",
                        properties->deviceName,
                        requirements->device_extension_names[i]);
                    return FALSE;
                }
            }
        }
        fr_memory_free(
            available_extensions, sizeof(VkExtensionProperties) * available_extension_count, MEMORY_TYPE_RENDERER);
    }
    return TRUE;
}

b8 _device_create_logical_device(vulkan_context* context) {
    // Check if there are shared queue families
    b8 present_shares_graphics_queue = context->device.graphics_family_index == context->device.present_family_index;
    b8 transfer_shares_graphics_queue = context->device.graphics_family_index == context->device.transfer_family_index;
    b8 compute_shares_present_queue = context->device.present_family_index == context->device.compute_family_index;
    u32 index_count = 1;
    if (!present_shares_graphics_queue) {
        ++index_count;
    }
    if (!transfer_shares_graphics_queue) {
        ++index_count;
    }
    if (!compute_shares_present_queue) {
        ++index_count;
    }
    u32 unique_queue_family_indices[index_count];
    u32 i = 0;
    unique_queue_family_indices[i++] = context->device.graphics_family_index;
    if (!present_shares_graphics_queue) {
        unique_queue_family_indices[i++] = context->device.present_family_index;
    }
    if (!transfer_shares_graphics_queue) {
        unique_queue_family_indices[i++] = context->device.transfer_family_index;
    }
    if (!compute_shares_present_queue) {
        unique_queue_family_indices[i++] = context->device.compute_family_index;
    }

    // Create the queue create info
    VkDeviceQueueCreateInfo queue_create_infos[index_count];
    for (i = 0; i < index_count; ++i) {
        queue_create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_infos[i].queueFamilyIndex = unique_queue_family_indices[i];
        queue_create_infos[i].queueCount = 1;
        // TODO: Check if we need more than one queue for the graphics family
        // some graphics cards might not have more than one if
        // (unique_queue_family_indices[i] ==
        // context->device.graphics_family_index) {
        //     queue_create_infos[i].queueCount = 2;
        // }
        queue_create_infos[i].flags = 0;
        queue_create_infos[i].pNext = 0;
        f32 queue_priority = 1.0f;
        queue_create_infos[i].pQueuePriorities = &queue_priority;

        FR_CORE_TRACE("Queue family index: %d, Queue count: %d",
                      queue_create_infos[i].queueFamilyIndex,
                      queue_create_infos[i].queueCount);
    }
    // Required device features
    // TODO: Get the required features from the engine/application
    VkPhysicalDeviceFeatures device_features = {};
    device_features.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo device_create_info = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
    device_create_info.queueCreateInfoCount = index_count;
    device_create_info.pQueueCreateInfos = queue_create_infos;
    device_create_info.pEnabledFeatures = &device_features;
    device_create_info.enabledExtensionCount = 1;
    const char* extension_name = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    device_create_info.ppEnabledExtensionNames = &extension_name;
    // We dont use any layers here as iut is not a thing anymore
    device_create_info.enabledLayerCount = 0;
    device_create_info.ppEnabledLayerNames = 0;

    // Create the logical device. Physical devices are not created directly,
    // instead their handles are retrieved. The logical device needs to be
    // created with the queue family indices. In most cases the logical device
    // is what we work with
    VK_CHECK_RESULT(
        vkCreateDevice(context->device.physical_device, &device_create_info, 0, &context->device.logical_device));
    FR_CORE_INFO("Logical device created");

    return TRUE;
}

b8 _device_get_device_queue_handles(vulkan_context* context) {
    // Get the queue handles
    vkGetDeviceQueue(context->device.logical_device,
                     context->device.graphics_family_index,
                     0,
                     &context->device.graphics_queue_handle);
    vkGetDeviceQueue(
        context->device.logical_device, context->device.present_family_index, 0, &context->device.present_queue_handle);
    vkGetDeviceQueue(
        context->device.logical_device, context->device.compute_family_index, 0, &context->device.compute_queue_handle);
    vkGetDeviceQueue(context->device.logical_device,
                     context->device.transfer_family_index,
                     0,
                     &context->device.transfer_queue_handle);
    FR_CORE_INFO("Device queue handles retrieved");
    return TRUE;
}

b8 _device_create_graphics_command_pool(vulkan_context* context) {
    // Create the command pool
    VkCommandPoolCreateInfo pool_create_info = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    pool_create_info.queueFamilyIndex = context->device.graphics_family_index;
    // This flag specifies that we can reset the command buffers from this pool
    // with eitehr the vkResetCommandBuffer or implicitly within the vkBeginCommandBuffer calls.
    // If VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT is not set then we must not call vkResetCommandBuffer.
    // This is useful for performance reasons as we can reuse the command buffers
    pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    VK_CHECK_RESULT(vkCreateCommandPool(
        context->device.logical_device, &pool_create_info, context->allocator, &context->device.graphics_command_pool));
    FR_CORE_INFO("Graphics Command pool created");
    return TRUE;
}

void _device_destroy_graphics_command_pool(vulkan_context* context) {
    if (context->device.graphics_command_pool) {
        vkDestroyCommandPool(context->device.logical_device, context->device.graphics_command_pool, context->allocator);
        context->device.graphics_command_pool = 0;
        FR_CORE_INFO("Graphics Command pool destroyed");
        return;
    }
    FR_CORE_WARN("Attempting to destroy a command pool handle that is NULL");
}
