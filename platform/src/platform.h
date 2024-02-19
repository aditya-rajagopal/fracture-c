/**
 * @file platform.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-02-13
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include "fracture/core/defines.h"
#include "fracture/core/types/input_codes.h"

// Function pointers to handle input event callbacks
typedef void (*PFN_on_key_event)(keys key_code, b8 is_pressed);
typedef void (*PFN_on_mouse_move)(i16 x_pos, i16 y_pos);
typedef void (*PFN_on_mouse_button_event)(mouse_button button, b8 is_pressed);
typedef void (*PFN_on_mouse_scroll)(i8 z_delta);

// Function pointers to handle window event callbacks
typedef void (*PFN_on_window_close)();

/**
 * @brief A typeless pointer to a platform state that will be implemented by the platform layer per platform. 
 *        This will be used to communicate with the rest of the systems in a decoupled manner.
 */
typedef struct platform_state {
    void* internal_state;

    // Function pointers to handle input event callbacks
    PFN_on_key_event on_key_event;
    PFN_on_mouse_move on_mouse_move;
    PFN_on_mouse_button_event on_mouse_button_event;
    PFN_on_mouse_scroll on_mouse_scroll;

    // Function pointers to handle window event callbacks
    PFN_on_window_close on_window_close;
} platform_state;

/**
 * @brief Initializes the platform layer with the given parameters and returns a platform state that can be used to communicate with the platform layer.
 * 
 * @param platform_state The platform state to be initialized 
 * @param window_title The title of the window mostly used for debugging purposes
 * @param window_width Width of the window
 * @param window_height Height of the window
 * @param x_pos The x position of the window
 * @param y_pos The y position of the window
 * @return b8 returns TRUE if the platform was initialized successfully, FALSE otherwise
 */
b8 platform_startup(
    platform_state* platform_state,
    const char* window_title,
    u32 window_width,
    u32 window_height,
    u32 x_pos,
    u32 y_pos 
);

/**
 * @brief Shuts down the platform layer and frees any resources that were allocated during the platform_startup function.
 * 
 * @param platform_state The platform state to be shut down
 */
void platform_shutdown(platform_state* platform_state);

/**
 * @brief Polls the platform layer for any messages that have been sent to the window and returns them.
 * 
 * @param platform_state The platform state to be polled
 * @return b8 returns TRUE if the platform was initialized successfully, FALSE otherwise
 */
b8 platform_pump_messages(platform_state* platform_state);

/**
 * @brief Allocates a block of memory of the given size and alignment.
 * 
 * @param size The size of the block of memory to be allocated
 * @param aligned The alignment of the block of memory to be allocated
 * @return void* A pointer to the block of memory that was allocated
 */
void* platform_allocate(u64 size, b8 aligned);

/**
 * @brief Frees a block of memory that was allocated by the platform_allocate function.
 * 
 * @param block The block of memory to be freed
 * @param aligned The alignment of the block of memory to be freed
 */
void platform_free(void* block, b8 aligned);

/**
 * @brief Zeros out a block of memory of the given size.
 * 
 * @param block The block of memory to be zeroed out
 * @param size The size of the block of memory to be zeroed out
 */
void* platform_zero_memory(void* block, u64 size);

/**
 * @brief Copies a block of memory from the source to the destination of the given size.
 * 
 * @param dest The destination block of memory
 * @param source The source block of memory
 * @param size The size of the block of memory to be copied
 * @return void* A pointer to the destination block of memory
 */
void* platform_copy_memory(void* dest, const void* source, u64 size);

/**
 * @brief Sets a block of memory to the given value of the given size.
 * 
 * @param dest The destination block of memory
 * @param value The value to be set
 * @param size The size of the block of memory to be set
 * @return void* A pointer to the destination block of memory
 */
void* platform_set_memory(void* dest, i32 value, u64 size);

/**
 * @brief Logs a message to the console with the given color.
 * 
 * @param message The message to be logged
 * @param color The color of the message to be logged
 */
void platform_console_write(const char* message, u8 color);

/**
 * @brief Logs an error message to the console with the given color.
 * 
 * @param message The error message to be logged
 * @param color The color of the error message to be logged
 */
void platform_console_write_error(const char* message, u8 color);

/**
 * @brief Gets the absolute time in nanoseconds. 
 * 
 * @return f64 
 */
f64 platform_get_absolute_time();

/**
 * @brief Puts the current thread to sleep for the given number of milliseconds.
 * 
 * @param milliseconds The number of milliseconds to sleep
 */
void platform_sleep(u64 milliseconds);
