#include "vulkan_platform.h"

#include "fracture/core/containers/darrays.h"
#include "fracture/core/systems/fracture_memory.h"
#include "vulkan/vulkan_core.h"

#include <platform.h>

#if defined(PLATFORM_WINDOWS)
#include <Windows.h>
#include <vulkan/vulkan_win32.h>

typedef struct internal_state {
    HINSTANCE hinstance;
    HWND hwnd;
} internal_state;

void vulkan_platform_get_required_instance_extensions(const char*** required_extensions) {
    darray_push(*required_extensions, &"VK_KHR_win32_surface");
}

b8 vulkan_platform_create_surface(struct vulkan_context *context) {
    u64 platform_state_size = 0;
    platform_get_handle_info(&platform_state_size, 0);
    void *mem_block = fr_memory_allocate(platform_state_size, MEMORY_TYPE_RENDERER);
    platform_get_handle_info(&platform_state_size, mem_block);

    internal_state* handle = (internal_state*)mem_block;
    if (!handle) {
        return FALSE;
    }

    VkWin32SurfaceCreateInfoKHR create_info = {VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR};
    create_info.hinstance = handle->hinstance;
    create_info.hwnd = handle->hwnd;

    VK_CHECK_RESULT(vkCreateWin32SurfaceKHR(context->instance, &create_info, context->allocator, &context->surface));
    return TRUE;
}

#endif