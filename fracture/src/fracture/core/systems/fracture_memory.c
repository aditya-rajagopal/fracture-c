#include "fracture_memory.h"
#include "fracture/core/systems/logging.h"

#include <excpt.h>
#include <platform.h>
#include <stdio.h>
#include <string.h>

typedef struct memory_statictics {
    u64 current_allocated;
    u64 current_allocated_per_type[TOTAL_MEMORY_TYPES];
    u64 peak_allocated;
    u64 peak_allocated_per_type[TOTAL_MEMORY_TYPES];
} memory_statictics;

static const char* memory_type_strings[] = {
    "UNKNOWN",
    "ARRAY",
    "DARRAY",
    "LIST",
    "RING_QUEUE",
    "BST",
    "HASH_TABLE",
    "MATRIX",
    "VECTOR",
    "STRING",
    "STACK",
    "QUEUE",
    "GRAPH",
    "TREE",
    "APPLICATION",
    "JOB",
    "THREAD",
    "TEXTURE",
    "MATERIAL_INSTANCE",
    "MESH",
    "TRANSFORM",
    "ENTITY",
    "COMPONENT",
    "SYSTEM",
    "SCENE",
    "PHYSICS",
    "AUDIO",
    "PARTICLE",
    "UI",
};

static memory_statictics stats = {0};

b8 fr_memory_initialize() {
    platform_zero_memory(&stats, sizeof(memory_statictics));
    return TRUE;
}

b8 fr_memory_shutdown() {
    if (stats.current_allocated != 0) {
        FR_CORE_FATAL("Memory leak detected: %llu bytes still allocated",
                      stats.current_allocated);
        FR_CORE_FATAL("Memory statistics: ");
        FR_CORE_FATAL("%s", fr_memory_get_stats());
    }

    platform_zero_memory(&stats, sizeof(memory_statictics));
    return TRUE;
}

void* fr_memory_allocate(u64 size, memory_types tag) {
    if (tag == MEMORY_TYPE_UNKNOWN) {
        FR_CORE_FATAL("Allocating unknown memory type: %llu bytes", size);
        return NULL_PTR;
    }

    void* ptr = platform_allocate(size, FALSE);
    if (ptr == NULL_PTR) {
        FR_CORE_FATAL("Failed to allocate memory: %llu bytes", size);
        return NULL_PTR;
    }

    platform_zero_memory(ptr, size);


#if defined(FR_DEBUG)
    stats.current_allocated += size;
    stats.current_allocated_per_type[tag] += size;

    if (stats.current_allocated > stats.peak_allocated) {
        stats.peak_allocated = stats.current_allocated;
    }

    if (stats.current_allocated_per_type[tag] > stats.peak_allocated_per_type[tag]) {
        stats.peak_allocated_per_type[tag] = stats.current_allocated_per_type[tag];
    }
#endif
    return ptr; 
}

void fr_memory_free(void* ptr, u64 size, memory_types tag) {
    if (ptr == NULL_PTR) {
        FR_CORE_FATAL("Attempting to free NULL pointer");
        return;
    }

    platform_free(ptr, FALSE);

#if defined(FR_DEBUG)
    stats.current_allocated -= size;
    stats.current_allocated_per_type[tag] -= size;

    if (stats.current_allocated < 0) {
        FR_CORE_FATAL("Memory corruption detected: %llu bytes freed", size);
    }

    if (stats.current_allocated_per_type[tag] < 0) {
        FR_CORE_FATAL("Memory corruption detected: %llu bytes freed of type %d", size, tag);
    }
#endif
}

void* fr_memory_reallocate(void* ptr, u64 size, u64 new_size, memory_types tag) {
    if (ptr == NULL_PTR) {
        FR_CORE_FATAL("Attempting to reallocate NULL pointer");
        return NULL_PTR;
    }

    void* new_ptr = platform_allocate(new_size, FALSE);
    if (new_ptr == NULL_PTR) {
        FR_CORE_FATAL("Failed to reallocate memory: %llu bytes", size);
        return NULL_PTR;
    }

    platform_zero_memory(new_ptr, new_size);
    platform_copy_memory(new_ptr, ptr, size);
    fr_memory_free(ptr, size, tag);
#if defined(FR_DEBUG)
    stats.current_allocated += new_size;
    stats.current_allocated_per_type[tag] += new_size;

    if (stats.current_allocated > stats.peak_allocated) {
        stats.peak_allocated = stats.current_allocated;
    }

    if (stats.current_allocated_per_type[tag] > stats.peak_allocated_per_type[tag]) {
        stats.peak_allocated_per_type[tag] = stats.current_allocated_per_type[tag];
    }
#endif
    return new_ptr;
}

void* fr_memory_zero(void* ptr, u64 size) {
    platform_zero_memory(ptr, size);
    return ptr;
}

void* fr_memory_copy(void* dest, const void* src, u64 size) {
    platform_copy_memory(dest, src, size);
    return dest;
}

void* fr_memory_set(void* ptr, i32 value, u64 size) {
    platform_set_memory(ptr, value, size);
    return ptr;
}

char* fr_memory_get_stats() {
    const f32 kib = 1024.0f;
    const f32 mib = 1024.0f * kib;
    const f32 gib = 1024.0f * mib;

    char buffer[10000] = "\nSystem Memory usage Statistics: \n";
    u32 offset = strlen(buffer);
    for (u32 i = 0; i < TOTAL_MEMORY_TYPES; ++i) {
        char current_value_unit[4] = "XiB";
        char peak_value_unit[4] = "XiB";
        f32 current_value = 0.0f;
        f32 peak_value = 0.0f;
        if (stats.current_allocated_per_type[i] > gib) {
            current_value = (f32)stats.current_allocated_per_type[i] / (f32)gib;
            current_value_unit[0] = 'G';
        } else if (stats.current_allocated_per_type[i] > mib) {
            current_value = (f32)stats.current_allocated_per_type[i] / (f32)mib;
            current_value_unit[0] = 'M';
        } else if (stats.current_allocated_per_type[i] > kib) {
            current_value = (f32)stats.current_allocated_per_type[i] / (f32)kib;
            current_value_unit[0] = 'K';
        } else {
            current_value = (f32)stats.current_allocated_per_type[i];
            current_value_unit[0] = 'B';
            current_value_unit[1] = '\0';
        }

        offset += snprintf(
            buffer + offset,
            10000 - offset,
            "%-25.25s%.2f%s",
            memory_type_strings[i],
            current_value,
            current_value_unit
        );

        if (stats.peak_allocated_per_type[i] > gib) {
            peak_value = (f32)stats.peak_allocated_per_type[i] / (f32)gib;
            peak_value_unit[0] = 'G';
        } else if (stats.peak_allocated_per_type[i] > mib) {
            peak_value = (f32)stats.peak_allocated_per_type[i] / (f32)mib;
            peak_value_unit[0] = 'M';
        } else if (stats.peak_allocated_per_type[i] > kib) {
            peak_value = (f32)stats.peak_allocated_per_type[i] / (f32)kib;
            peak_value_unit[0] = 'K';
        } else {
            peak_value_unit[0] = 'B';
            peak_value_unit[1] = '\0';
            peak_value = (f32)stats.peak_allocated_per_type[i];
        }
        
        offset += snprintf(buffer + offset, 10000 - offset, "\t\t(Peak: %.2f%s)\n", peak_value, peak_value_unit);
    }

    // Add the total memory usage
    char current_value_unit[4] = "XiB";
    char peak_value_unit[4] = "XiB";
    f32 current_value = 0.0f;
    f32 peak_value = 0.0f;
    if (stats.current_allocated > gib) {
        current_value = (f32)stats.current_allocated / (f32)gib;
        current_value_unit[0] = 'G';
    } else if (stats.current_allocated > mib) {
        current_value = (f32)stats.current_allocated / (f32)mib;
        current_value_unit[0] = 'M';
    } else if (stats.current_allocated > kib) {
        current_value = (f32)stats.current_allocated / (f32)kib;
        current_value_unit[0] = 'K';
    } else {
        current_value = (f32)stats.current_allocated;
        current_value_unit[0] = 'B';
        current_value_unit[1] = '\0';
    }

    offset += snprintf(
        buffer + offset,
        10000 - offset,
        "%-25.25s%.2f%s",
        "Total value:",
        current_value,
        current_value_unit
    );

    if (stats.peak_allocated > gib) {
        peak_value = (f32)stats.peak_allocated / (f32)gib;
        peak_value_unit[0] = 'G';
    } else if (stats.peak_allocated > mib) {
        peak_value = (f32)stats.peak_allocated / (f32)mib;
        peak_value_unit[0] = 'M';
    } else if (stats.peak_allocated > kib) {
        peak_value = (f32)stats.peak_allocated / (f32)kib;
        peak_value_unit[0] = 'K';
    } else {
        peak_value_unit[0] = 'B';
        peak_value_unit[1] = '\0';
        peak_value = (f32)stats.peak_allocated;
    }

    offset += snprintf(buffer + offset, 10000 - offset, "\t(Peak: %.2f%s)\n", peak_value, peak_value_unit);
    char* result = _strdup(buffer);
    return result;
}

u64 fr_memory_get_current_usage() {
    return stats.current_allocated;
}

u64 fr_memory_get_peak_usage() {
    return stats.peak_allocated;
}

u64 fr_memory_get_current_usage_per_type(memory_types type) {
    return stats.current_allocated_per_type[type];
}

u64 fr_memory_get_peak_usage_per_type(memory_types type) {
    return stats.peak_allocated_per_type[type];
}
