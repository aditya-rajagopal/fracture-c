#include "vulkan_platform.h"

#include "fracture/core/containers/darrays.h"

#if defined(PLATFORM_WINDOWS)

void vulkan_platform_get_required_instance_extensions(const char*** required_extensions) {
    darray_push(*required_extensions, &"VK_KHR_win32_surface");
}

#endif