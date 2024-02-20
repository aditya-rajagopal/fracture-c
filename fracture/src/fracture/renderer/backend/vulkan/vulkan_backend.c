#include "vulkan_backend.h"

#include "fracture/renderer/backend/vulkan/vulkan_types.h"
#include "vulkan/vulkan_core.h"

#include "fracture/core/systems/logging.h"

static vulkan_context context;

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

    VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app_info.apiVersion = VK_API_VERSION_1_3;
    app_info.pApplicationName = app_name;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "Fracture Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo instance_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    // It is safe to pass the address of the stack allocated app_info beause it
    // is all thrown away after creating the instance
    instance_info.pApplicationInfo = &app_info;
    instance_info.enabledExtensionCount = 0;
    instance_info.ppEnabledExtensionNames = 0;
    instance_info.enabledLayerCount = 0;
    instance_info.ppEnabledLayerNames = 0;

    VkResult result = vkCreateInstance(&instance_info, context.allocator, &context.instance);
    if (result != VK_SUCCESS) {
        FR_CORE_FATAL("Failed to create Vulkan instance with error code: %d", result);
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
