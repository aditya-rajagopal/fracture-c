/**
 * @file fracture_memory.h
 * @author Aditya Rajagopal
 * @brief Contains memory management utilities for the Fracture Game Engine.
 * @version 0.0.1
 * @date 2024-02-16
 *
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 *
 */
#pragma once

#include "fracture/core/defines.h"

typedef enum memory_types {
    MEMORY_TYPE_UNKNOWN = 0,
    // Memory types for core data structures
    MEMORY_TYPE_ARRAY,
    MEMORY_TYPE_DARRAY,
    MEMORY_TYPE_LLIST,
    MEMORY_TYPE_RING_QUEUE,
    MEMORY_TYPE_BST,
    MEMORY_TYPE_HASH_TABLE,
    MEMORY_TYPE_MATRIX,
    MEMORY_TYPE_VECTOR,
    MEMORY_TYPE_STRING,
    MEMORY_TYPE_STACK,
    MEMORY_TYPE_QUEUE,
    MEMORY_TYPE_GRAPH,
    MEMORY_TYPE_TREE,

    // Memory types for client application
    MEMORY_TYPE_APPLICATION,

    // Memory types for job system
    MEMORY_TYPE_JOB,
    MEMORY_TYPE_THREAD,

    // Memory types for rendering
    MEMORY_TYPE_RENDERER,
    MEMORY_TYPE_TEXTURE,
    MEMORY_TYPE_MATERIAL_INSTANCE,
    MEMORY_TYPE_MESH,
    MEMORY_TYPE_TRANSFORM,
    MEMORY_TYPE_ENTITY,
    MEMORY_TYPE_COMPONENT,
    MEMORY_TYPE_SYSTEM,
    MEMORY_TYPE_SCENE,
    MEMORY_TYPE_PHYSICS,
    MEMORY_TYPE_AUDIO,
    MEMORY_TYPE_PARTICLE,
    MEMORY_TYPE_UI,

    TOTAL_MEMORY_TYPES
} memory_types;

/**
 * @brief Initializes the memory system for the Fracture Game Engine.
 *
 */
FR_API b8 fr_memory_initialize();

/**
 * @brief Shuts down the memory system for the Fracture Game Engine.
 *
 */
FR_API b8 fr_memory_shutdown();

/**
 * @brief Allocates memory and zeroes it.
 *
 * @param size The size of the memory to allocate.
 * @param type The type of memory to allocate.
 * @return void* A pointer to the allocated memory.
 */
FR_API void* fr_memory_allocate(u64 size, memory_types type);

/**
 * @brief Frees memory at the given pointer.
 *
 * @param ptr A pointer to the memory to free.
 * @param size The size of the memory to free.
 * @param type The type of memory to free.
 */
FR_API void fr_memory_free(void* ptr, u64 size, memory_types type);

/**
 * @brief Reallocates memory to a new location with the new_size and frees the old memory.
 * When new_size = size this function just moves the memory to a new location and frees the old memory.
 *
 * @param ptr A pointer to the memory to reallocate.
 * @param size The size of the memory to reallocate.
 * @param new_size The new size of the memory to reallocate.
 * @param type The type of memory to reallocate.
 * @return void* A pointer to the reallocated memory.
 */
FR_API void* fr_memory_reallocate(void* ptr, u64 size, u64 new_size, memory_types type);

/**
 * @brief Allocates zeroed memory for the Fracture Game Engine.
 *
 * @param size The size of the memory to allocate.
 * @return void* A pointer to the allocated memory.
 */
FR_API void* fr_memory_zero(void* ptr, u64 size);

/**
 * @brief Copies data from one memory location to another.
 *
 * @param dest The destination we are copying to.
 * @param src The source we are copying from.
 * @param size The size of the memory to copy.
 * @param dest_type The type of memory to copy to.
 * @return void* A pointer to the copied memory.
 */
FR_API void* fr_memory_copy(void* dest, const void* src, u64 size);

/**
 * @brief Sets memory to a given value.
 *
 * @param dest The destination we are setting.
 * @param value The value we are setting to.
 * @param size The size of the memory to set.
 * @return void* A pointer to the set memory.
 */
FR_API void* fr_memory_set(void* dest, i32 value, u64 size);

/**
 * @brief Gets the current memory statistics for the Fracture Game Engine.
 *
 * @return char* A string containing the memory statistics.
 */
FR_API char* fr_memory_get_stats();

/**
 * @brief Prints the current memory statistics for the Fracture Game Engine.
 * @details Function logs the memory statistics to the console at the INFO
 * level. This exists to handle the memory deallocation of the string returned
 * by fr_memory_get_stats.
 */
FR_API void fr_memory_print_stats();

/**
 * @brief Gets the current memory usage for the Fracture Game Engine.
 *
 * @return u64 The current memory usage.
 */
FR_API u64 fr_memory_get_current_usage();

/**
 * @brief Gets the current memory peak usage for the Fracture Game Engine.
 *
 * @return u64 The current memory peak usage.
 */
FR_API u64 fr_memory_get_peak_usage();

/**
 * @brief Gets the current memory total usage for the Fracture Game Engine.
 *
 * @param type The type of memory to get the total usage for.
 * @return u64 The current memory total usage for the given type.
 */
FR_API u64 fr_memory_get_total_usage_for_type(memory_types type);

/**
 * @brief Gets the current memory peak usage for the Fracture Game Engine.
 *
 * @param type The type of memory to get the peak usage for.
 * @return u64 The current memory peak usage for the given type.
 */
FR_API u64 fr_memory_get_peak_usage_for_type(memory_types type);
