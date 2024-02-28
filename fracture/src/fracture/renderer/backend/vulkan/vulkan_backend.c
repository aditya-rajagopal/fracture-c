#include "vulkan_backend.h"

#include "fracture/renderer/backend/vulkan/vulkan_types.h"
#include "fracture/renderer/backend/vulkan/platform/vulkan_platform.h"
#include "fracture/renderer/backend/vulkan/vulkan_device.h"
#include "fracture/renderer/backend/vulkan/vulkan_swapchain.h"
#include "fracture/renderer/backend/vulkan/vulkan_renderpass.h"
#include "fracture/renderer/backend/vulkan/vulkan_commandbuffer.h"
#include "fracture/renderer/backend/vulkan/vulkan_framebuffer.h"
#include "fracture/renderer/backend/vulkan/vulkan_fence.h"

#include "fracture/core/systems/logging.h"
#include "fracture/core/containers/darrays.h"
#include "fracture/core/systems/fracture_memory.h"
#include "fracture/core/library/fracture_string.h"
#include "vulkan/vulkan_core.h"

#include <platform.h>

// HACK: This is a hack to get the window width and height
#include "fracture/engine/engine.h"

static vulkan_context context;
static u32 cached_framebuffer_width = 0;
static u32 cached_framebuffer_height = 0;

void _vulkan_load_required_instance_extensions(const char*** required_extensions);
b8 _vulkan_load_validation_layers(const char*** required_validation_layers, u32* required_validation_layer_count);
b8 _backend_create_instance(const char* app_name);
b8 _backend_create_debugger();
void _backend_destroy_debugger();

VKAPI_ATTR VkBool32 VKAPI_CALL _vulkan_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_types,
    const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *user_data);

i32 _backend_find_memory_index(u32 type_filter, VkMemoryPropertyFlags flags);

b8 _backend_create_command_buffers(renderer_backend* backend);
void _backed_destroy_command_buffers();

b8 _backend_regenerate_framebuffers(renderer_backend* backend,
                                    vulkan_swapchain* swapchain,
                                    vulkan_renderpass* renderpass);
void _backend_destroy_framebuffers(vulkan_swapchain* swapchain);

b8 _backend_create_sync_objects();
void _backend_destroy_sync_objects();

b8 vulkan_backend_initialize(renderer_backend* backend, const char* app_name, struct platform_state* plat_state) {
    if (!backend) {
        FR_CORE_ERROR("Renderer backend is NULL");
        return FALSE;
    }
    if (!plat_state) {
        FR_CORE_ERROR("Platform state is NULL");
        return FALSE;
    }
    if (backend->is_initialized) {
        FR_CORE_WARN("Renderer backend already initialized");
        return TRUE;
    }
    // TODO: Add a cutom allocator and pass it to the context
    context.allocator = NULL;
    engine_get_framebuffer_size(&cached_framebuffer_width, &cached_framebuffer_height);
    context.framebuffer_width = cached_framebuffer_width ? cached_framebuffer_width : 800;
    context.framebuffer_height = cached_framebuffer_height ? cached_framebuffer_height : 600;
    cached_framebuffer_height = 0;
    cached_framebuffer_width = 0;

    context.PFN_find_memory_type = _backend_find_memory_index;
    // Create the Vulkan instance
    if (!_backend_create_instance(app_name)) {
        FR_CORE_ERROR("Failed to create Vulkan instance");
        return FALSE;
    }
    FR_CORE_INFO("Vulkan instance created successfully");

    if (!_backend_create_debugger()) {
        FR_CORE_ERROR("Failed to create Vulkan debugger");
        return FALSE;
    }
    FR_CORE_INFO("Vulkan debugger created successfully");

    // Create the surface
    if (!vulkan_platform_create_surface(&context)) {
        FR_CORE_ERROR("Failed to create vulkan surface");
        return FALSE;
    }

    // Create device
    if (!vulkan_create_device(&context)) {
        FR_CORE_ERROR("Failed to create vulkan device");
        return FALSE;
    }
    FR_CORE_INFO("Device created successfully");

    // Create the swapchain
    if (!vulkan_swapchain_create(&context, context.framebuffer_width, context.framebuffer_height, &context.swapchain)) {
        FR_CORE_ERROR("Failed to create vulkan swapchain");
        return FALSE;
    }

    rect_2d render_aread = {
        .x = 0,
        .y = 0,
        .width = context.framebuffer_width,
        .height = context.framebuffer_height
    };

    colour clear_colour = {
        .r = 0.8f,
        .g = 0.0f,
        .b = 0.8f,
        .a = 1.0f
    };

    // Create the renderpass
    if (!vulkan_renderpass_create(&context, &context.main_renderpass, render_aread, clear_colour, 1.0f, 0)) {
        FR_CORE_ERROR("Failed to create vulkan renderpass");
        return FALSE;
    }

    // Create the framebuffers for the swapchain images
    if (!_backend_regenerate_framebuffers(backend, &context.swapchain, &context.main_renderpass)) {
        FR_CORE_ERROR("Failed to create framebuffers for swapchain images");
        return FALSE;
    }

    // Create the command buffers
    if(!_backend_create_command_buffers(backend)) {
        FR_CORE_ERROR("Failed to create vulkan command buffers");
        return FALSE;
    }

    // Create the sync objects
    if (!_backend_create_sync_objects()) {
        FR_CORE_ERROR("Failed to create vulkan sync objects");
        return FALSE;
    }
    
    backend->is_initialized = TRUE;
    FR_CORE_INFO("Vulkan backend initialized successfully");
    return TRUE;
}

void vulkan_backend_shutdown(renderer_backend* backend) {
    if (!backend) {
        return;
    }
    if (!backend->is_initialized) {
        return;
    }
    // Wait for the device to finish rendering
    vkDeviceWaitIdle(context.device.logical_device);
    // Destroy the sync objects
    _backend_destroy_sync_objects();
    // Destroy the command buffers
    _backed_destroy_command_buffers();

    // Destroy the framebuffers
    _backend_destroy_framebuffers(&context.swapchain);
    FR_CORE_INFO("Swapchain framebuffers destroyed successfully");

    // Destroy the renderpass
    vulkan_renderpass_destroy(&context, &context.main_renderpass);

    // Destroy the swapchain
    vulkan_swapchain_destroy(&context, &context.swapchain);

    // Destroy vulkan devices
    vulkan_destroy_device(&context);

    // Destroy the surface
    if (context.surface) {
        FR_CORE_INFO("Destroying Vulkan surface...");
        vkDestroySurfaceKHR(context.instance, context.surface,
                            context.allocator);
        context.surface = 0;
    }
    // Destroy the debugger
    _backend_destroy_debugger();

    // Destroy the instance
    FR_CORE_INFO("Destroying Vulkan instance...");
    vkDestroyInstance(context.instance, context.allocator);

    backend->is_initialized = FALSE;
}

b8 vulkan_backend_begin_frame(renderer_backend* backend, f64 delta_time) {
    if (!backend) {
        return FALSE;
    }
    return TRUE;
}

b8 vulkan_backend_end_frame(renderer_backend* backend, f64 delta_time) {
    if (!backend) {
        return FALSE;
    }
    return TRUE;
}

void vulkan_backend_on_window_resize(renderer_backend* backend, u32 width, u32 height) {
}


// -------------------------------------------------------------------------------------------------
// ----------------------------------- Private Functions -------------------------------------------
// -------------------------------------------------------------------------------------------------

void _vulkan_load_required_instance_extensions(const char*** required_extensions) {
    darray_push(*required_extensions,
                &VK_KHR_SURFACE_EXTENSION_NAME); // General surface extension
    vulkan_platform_get_required_instance_extensions(required_extensions);
#if defined(_DEBUG)
    // Add the debug utils extension for debugging
    darray_push(*required_extensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    FR_CORE_TRACE("Required instance extensions: ");
    u64 length = darray_length(*required_extensions);

    for (u64 i = 0; i < length; ++i) {
        FR_CORE_TRACE("%s", (*required_extensions)[i]);
    }
#endif
}

b8 _vulkan_load_validation_layers(const char*** required_validation_layers, u32* required_validation_layer_count) {
#if defined(_DEBUG)
    FR_CORE_INFO("Validation layers are enabled. Loading validation layers...");

    // Add the the requried validation layers to be enabled
    *required_validation_layers = darray_create(const char*);
    darray_push(*required_validation_layers, &"VK_LAYER_KHRONOS_validation");
    *required_validation_layer_count += darray_length(*required_validation_layers);

    // Get the list of all available validation layers
    u32 available_layer_count = 0;
    VK_CHECK_RESULT(vkEnumerateInstanceLayerProperties(&available_layer_count, NULL));
    VkLayerProperties* available_layers = darray_reserve(available_layer_count, VkLayerProperties);
    VK_CHECK_RESULT(vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers));
 
    // Check if the required validation layers are available
    for (u32 i = 0; i < *required_validation_layer_count; ++i) {
        FR_CORE_TRACE("Searching for validation layer: %s", *required_validation_layers[i]);
        b8 layer_found = FALSE;
        for (u32 j = 0; j < available_layer_count; ++j) {
            if (fr_string_compare(available_layers[j].layerName, *required_validation_layers[i]) == 0) {
                layer_found = TRUE;
                FR_CORE_TRACE("Found!");
                break;
            }
        }
        if (!layer_found) {
            FR_CORE_FATAL("Required validation layer: %s not found", *required_validation_layers[i]);
            return FALSE;
        }
    }
    FR_CORE_TRACE("All required validation layers found");
    darray_destroy(available_layers);
#endif
    return TRUE;
}

b8 _backend_create_instance(const char* app_name) {
    VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app_info.apiVersion = VK_API_VERSION_1_3;
    app_info.pApplicationName = app_name;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "Fracture Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    // Create the instance info
    VkInstanceCreateInfo instance_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    // It is safe to pass the address of the stack allocated app_info beause it
    // is all thrown away after creating the instance
    instance_info.pApplicationInfo = &app_info;

    // Get the required extensions
    const char** required_extensions = darray_create(const char*);
    _vulkan_load_required_instance_extensions(&required_extensions);
    instance_info.enabledExtensionCount = darray_length(required_extensions);
    instance_info.ppEnabledExtensionNames = required_extensions;

    // Get the required validation layers
    const char** required_validation_layers = 0;
    u32 required_validation_layer_count = 0;
    if(!_vulkan_load_validation_layers(&required_validation_layers, &required_validation_layer_count)) {
        return FALSE;
    }
    instance_info.enabledLayerCount = required_validation_layer_count;
    instance_info.ppEnabledLayerNames = required_validation_layers;

    VK_CHECK_RESULT(vkCreateInstance(&instance_info, context.allocator, &context.instance));
    darray_destroy(required_extensions);
    darray_destroy(required_validation_layers);
    return TRUE;
}

b8 _backend_create_debugger() {
#if defined(_DEBUG)
    FR_CORE_INFO("Creating Vulkan debugger...");
    u32 log_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT; // |
    // VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
    // VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;

    VkDebugUtilsMessengerCreateInfoEXT debug_info = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
    debug_info.messageSeverity = log_severity;
    debug_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_info.pfnUserCallback = _vulkan_debug_callback;
    debug_info.pUserData = NULL;

    // Since the debug messenger is an extension, we need to load it manually by getting the function pointer from the instance
    PFN_vkCreateDebugUtilsMessengerEXT debug_func =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            context.instance, "vkCreateDebugUtilsMessengerEXT");
    FR_CORE_ASSERT(debug_func);
    VK_CHECK_RESULT(debug_func(context.instance, &debug_info, context.allocator, &context.debug_messenger));
#endif
    return TRUE;
}

void _backend_destroy_debugger() {
#if defined(_DEBUG)
    // Destroy the debugger
    if (context.debug_messenger) {
        FR_CORE_INFO("Destroying Vulkan debugger...");
        PFN_vkDestroyDebugUtilsMessengerEXT debug_func =
            (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
                context.instance, "vkDestroyDebugUtilsMessengerEXT");
        FR_CORE_ASSERT(debug_func);
        debug_func(context.instance, context.debug_messenger, context.allocator);
    }
#endif
}

VKAPI_ATTR VkBool32 VKAPI_CALL _vulkan_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_types,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data) {
    switch (message_severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            FR_CORE_TRACE("Vulkan Validation Layer: %s", callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            FR_CORE_INFO("Vulkan Validation Layer: %s", callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            FR_CORE_WARN("Vulkan Validation Layer: %s", callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            FR_CORE_ERROR("Vulkan Validation Layer: %s", callback_data->pMessage);
            break;
        default:
            FR_CORE_ERROR("Vulkan Validation Layer: %s", callback_data->pMessage);
            break;
    }
    return VK_FALSE;
}

i32 _backend_find_memory_index(u32 type_filter, VkMemoryPropertyFlags flags) {
    VkPhysicalDeviceMemoryProperties mem_properties = context.device.memory_properties;
    for (u32 i = 0; i < mem_properties.memoryTypeCount; ++i) {
        // Check each memory type to see if its bit is set to 1. 
        if ((type_filter & (1 << i)) &&
            (mem_properties.memoryTypes[i].propertyFlags & flags) == flags) {
            return i;
        }
    }
    FR_CORE_WARN("Unable to find suitable memory type for buffer");
    return -1;
}

b8 _backend_create_command_buffers(renderer_backend* backend) {
    // Create our graphics command buffers
    // We want to create atleast 1 command buffer for each swapchain image so
    // that we can record commands for each image in the swapchain while the
    // other is being presented
    if (!context.graphics_command_buffers) {
        context.graphics_command_buffers = darray_reserve(
            context.swapchain.image_count, vulkan_command_buffer);
    }

    for (u32 i = 0; i < context.swapchain.image_count; ++i) {
        if (context.graphics_command_buffers[i].handle) {
            vulkan_command_buffer_free(&context,
                                       context.device.graphics_command_pool,
                                       &context.graphics_command_buffers[i]);
        }
        fr_memory_zero(&context.graphics_command_buffers[i], sizeof(vulkan_command_buffer));
        if (!vulkan_command_buffer_allocate(
                &context, context.device.graphics_command_pool, TRUE,
                &context.graphics_command_buffers[i])) {
            FR_CORE_ERROR("Failed to allocate command buffer for index: %d", i);
            return FALSE;
        }
    }
    FR_CORE_INFO("Graphics command buffers created successfully");
    return TRUE;
}

void _backed_destroy_command_buffers() {
    if (context.graphics_command_buffers) {
        u64 length = darray_length(context.graphics_command_buffers);
        for (u32 i = 0; i < length; ++i) {
            vulkan_command_buffer_free(&context, context.device.graphics_command_pool,
                                       &context.graphics_command_buffers[i]);
            context.graphics_command_buffers[i].handle = VK_NULL_HANDLE;
        }
        darray_destroy(context.graphics_command_buffers);
        context.graphics_command_buffers = 0;
        FR_CORE_INFO("Graphics command buffers destroyed successfully");
    }
}

b8 _backend_regenerate_framebuffers(renderer_backend* backend,
                                    vulkan_swapchain* swapchain,
                                    vulkan_renderpass* renderpass) {
    if (swapchain->framebuffers) {
        _backend_destroy_framebuffers(swapchain);
    }
    if (!swapchain->framebuffers) {
        swapchain->framebuffers = darray_reserve(swapchain->image_count, vulkan_frame_buffer);
    }
    FR_CORE_TRACE("Regenerating framebuffers for swapchain images...");
    // Anytime we change the window size we need to recreate the framebuffers and the swapchain
    for (u32 i = 0; i < swapchain->image_count; ++i) {
        // TODO: We might have more attachments in the future make this dynamic
        u32 attachment_count = 2;
        VkImageView attachments[] = {swapchain->image_views[i], swapchain->depth_attachment.image_view};
        if (!vulkan_framebuffer_create(
                &context, renderpass, context.framebuffer_width,
                context.framebuffer_height, attachment_count, attachments,
                &swapchain->framebuffers[i])) {
            FR_CORE_ERROR("Failed to create framebuffer for swapchain image index: %d", i);
            return FALSE;
        }
    }
    return TRUE;
}

void _backend_destroy_framebuffers(vulkan_swapchain* swapchain) {
    if (swapchain->framebuffers) {
        for (u32 i = 0; i < swapchain->image_count; ++i) {
            vulkan_framer_buffer_destroy(&context, &swapchain->framebuffers[i]);
        }
        darray_destroy(swapchain->framebuffers);
        swapchain->framebuffers = 0;
        return;
    }
    FR_CORE_WARN("Attempting to destroy swapchain framebuffers that are NULL or not allocated");
}

b8 _backend_create_sync_objects() {
    context.image_available_semaphores =
        darray_reserve(context.swapchain.max_frames_in_flight, VkSemaphore);
    context.queue_complete_semaphores =
        darray_reserve(context.swapchain.max_frames_in_flight, VkSemaphore);
    context.in_flight_fences =
        darray_reserve(context.swapchain.max_frames_in_flight, vulkan_fence);
    context.in_flight_fence_count = context.swapchain.max_frames_in_flight;
    
    for (u32 i = 0; i < context.swapchain.max_frames_in_flight; ++i) {
        VkSemaphoreCreateInfo semaphore_info = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        VK_CHECK_RESULT(vkCreateSemaphore(
            context.device.logical_device, &semaphore_info, context.allocator,
            &context.image_available_semaphores[i]));
        VK_CHECK_RESULT(vkCreateSemaphore(
            context.device.logical_device, &semaphore_info, context.allocator,
            &context.queue_complete_semaphores[i]));
        // When we create the fence we will set the is_signaled to true which
        // essentianlly means that the first frame has finished rendering
        // This will stop the application from waiting forever for the first
        // frame to render because it will have to wait for the frame before it
        // to finish rendering(which is not possible)
        if(!vulkan_fence_create(&context, &context.in_flight_fences[i], VK_FENCE_CREATE_SIGNALED_BIT)) {
            FR_CORE_ERROR("Failed to create in flight fence for index: %d", i);
            return FALSE;
        }
    }

    // Create the images in flight array
    // In imaged_in_flight fences dont exist yet because no image has been
    // rendered yet or is in flight. We will just create the array and set the
    // pointers to 0. The actual fences are not owned by the images_in_flight
    // array just the pointer to a fence in the in_flight_fences array. When an
    // image fence is not used it is set to NULL.
    context.images_in_flight = darray_reserve(context.swapchain.image_count, vulkan_fence*);
    for (u32 i = 0; i < context.swapchain.image_count; ++i) {
        context.images_in_flight[i] = NULL;
    }
    FR_CORE_INFO("Sync objects created successfully for %d frames in flight", context.swapchain.max_frames_in_flight);
    return TRUE;
}

void _backend_destroy_sync_objects() {
    if (context.image_available_semaphores) {
        for (u32 i = 0; i < context.swapchain.max_frames_in_flight; ++i) {
            vkDestroySemaphore(context.device.logical_device,
                               context.image_available_semaphores[i],
                               context.allocator);
        }
        darray_destroy(context.image_available_semaphores);
        context.image_available_semaphores = NULL;
        FR_CORE_TRACE("Image available semaphores destroyed successfully");
    }
    if (context.images_in_flight) {
        darray_destroy(context.images_in_flight);
        context.images_in_flight = NULL;
        FR_CORE_TRACE("Images in flight destroyed successfully");
    }
    if (context.queue_complete_semaphores) {
        for (u32 i = 0; i < context.swapchain.max_frames_in_flight; ++i) {
            vkDestroySemaphore(context.device.logical_device,
                               context.queue_complete_semaphores[i],
                               context.allocator);
        }
        darray_destroy(context.queue_complete_semaphores);
        context.queue_complete_semaphores = NULL;
        FR_CORE_TRACE("Queue complete semaphores destroyed successfully");
    }
    if (context.in_flight_fences) {
        for (u32 i = 0; i < context.swapchain.max_frames_in_flight; ++i) {
            vulkan_fence_destroy(&context, &context.in_flight_fences[i]);
        }
        darray_destroy(context.in_flight_fences);
        context.in_flight_fences = NULL;
        context.in_flight_fence_count = 0;
        FR_CORE_TRACE("In flight fences destroyed successfully");
    }
    FR_CORE_INFO("Sync objects destroyed successfully for %d frames in flight", context.swapchain.max_frames_in_flight);
}
