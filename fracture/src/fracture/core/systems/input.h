/**
 * @file input.h
 * @author Aditya Rajagopal
 * @brief System for handling input events and providing api for querying input
 * state
 * @details This system is responsible for handling input events and providing
 * an api for querying input state for the application. Here we define functions
 * for the platform layer to call when input events occur, and we also define
 * functions for the application layer to query the state of input devices.
 * @version 0.0.1
 * @date 2024-02-18
 *
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 *
 */
#pragma once

#include "fracture/core/defines.h"
#include "fracture/core/includes/input_codes.h"

/**
 * @brief Initializes the input system
 * @return b8 TRUE if the input system was initialized successfully, FALSE otherwise
 */
FR_API b8 fr_input_initialize();

/**
 * @brief Shuts down the input system
 * @return b8 TRUE if the input system was shut down successfully, FALSE otherwise
 */
FR_API b8 fr_input_shutdown();

/**
 * @brief Updates the input system state
 * @details This function should be called once per frame to update the input
 * system. It moves the current input state to the previous input state.
 * @param delta_time The time since the last frame
 */
FR_API void fr_input_update(f64 delta_time);

// Keyboard events

/**
 * @brief Processes a key press event
 * @details This function should be called by the platform layer when a key
 * press event occurs. It updates the current input state with the new key press
 * event and dispatches an event to listeners if necessary. The context provided
 * during the event is an array of u16s with the structure [key, is_repeated,
 * mouse_x, mouse_y]. If allow_repeats is set to TRUE then an event is dipatched
 * if the key is held down. This is not the usual behavior expected to be used.
 * @param key The key that was pressed
 * @param pressed TRUE if the key was pressed, FALSE if the key was released
 */
void fr_input_process_keypress(keys key, b8 pressed);

/**
 * @brief Sets whether or not the input system should allow key repeats
 * @param enable TRUE if the input system should allow key repeats, FALSE
 * otherwise
 */
FR_API void fr_input_set_repeats(b8 enable);

/**
 * @brief Checks if a key is currently down
 * @param key The key to check
 * @return b8 TRUE if the key is currently down, FALSE otherwise
 */
FR_API b8 fr_input_is_key_down(keys key);

/**
 * @brief Checks if a key was down in the previous frame
 * @param key The key to check
 * @return b8 TRUE if the key was down in the previous frame, FALSE otherwise
 */
FR_API b8 fr_input_was_key_down(keys key);

/**
 * @brief Checks if a key was pressed in the current frame
 * @param key The key to check
 * @return b8 TRUE if the key was pressed in the current frame, FALSE otherwise
 */
FR_API b8 fr_input_is_key_pressed_this_frame(keys key);

/**
 * @brief Checks if a key is currently up
 * @param key The key to check
 * @return b8 TRUE if the key is currently up, FALSE otherwise
 */
FR_API b8 fr_input_is_key_up(keys key);

/**
 * @brief Checks if a key was up in the previous frame
 * @param key The key to check
 * @return b8 TRUE if the key was up in the previous frame, FALSE otherwise
 */
FR_API b8 fr_input_was_key_up(keys key);

/**
 * @brief Checks if a key was released in the current frame
 * @param key The key to check
 * @return b8 TRUE if the key was released in the current frame, FALSE otherwise
 */
FR_API b8 fr_input_is_key_released_this_frame(keys key);

// Mouse events

/**
 * @brief Processes a mouse button event
 * @details This function should be called by the platform layer when a mouse
 * button event occurs. It updates the current input state with the new mouse
 * button event and dispatches an event to listeners if necessary. The context
 * provided during the event is an array of u16s with the structure [button,
 * is_repeated, mouse_x, mouse_y].
 * @param button The mouse button that was pressed
 * @param pressed TRUE if the button was pressed, FALSE if the button was
 * released
 */
void fr_input_process_mouse_button(mouse_button button, b8 pressed);

/**
 * @brief Processes a mouse move event
 * @details This function should be called by the platform layer when a mouse
 * move event occurs. It updates the current input state with the new mouse
 * position and dispatches an event to listeners if necessary. The context
 * provided during the event is an array of u16s with the structure [mouse_x,
 * mouse_y].
 * @param x The x position of the mouse
 * @param y The y position of the mouse
 */
void fr_input_process_mouse_move(i16 x, i16 y);

/**
 * @brief Processes a mouse wheel event
 * @details This function should be called by the platform layer when a mouse
 * wheel event occurs. It updates the current input state with the new mouse
 * wheel position and dispatches an event to listeners if necessary. The context
 * provided during the event is an array of u16s with the structure [z_delta,
 * mouse_x, mouse_y].
 * @param z_delta The change in the mouse wheel position
 */
void fr_input_process_mouse_wheel(i8 z_delta);

/**
 * @brief Checks if a mouse button is currently down
 * @param button The mouse button to check
 * @return b8 TRUE if the mouse button is currently down, FALSE otherwise
 */
FR_API b8 fr_input_is_button_down(mouse_button button);

/**
 * @brief Checks if a mouse button was down in the previous frame
 * @param button The mouse button to check
 * @return b8 TRUE if the mouse button was down in the previous frame, FALSE
 * otherwise
 */
FR_API b8 fr_input_was_button_down(mouse_button button);

/**
 * @brief Checks if a mouse button was pressed in the current frame
 * @param button The mouse button to check
 * @return b8 TRUE if the mouse button was pressed in the current frame, FALSE
 * otherwise
 */
FR_API b8 fr_input_is_button_pressed_this_frame(mouse_button button);

/**
 * @brief Checks if a mouse button is currently up
 * @param button The mouse button to check
 * @return b8 TRUE if the mouse button is currently up, FALSE otherwise
 */
FR_API b8 fr_input_is_button_up(mouse_button button);

/**
 * @brief Checks if a mouse button was up in the previous frame
 * @param button The mouse button to check
 * @return b8 TRUE if the mouse button was up in the previous frame, FALSE
 * otherwise
 */
FR_API b8 fr_input_was_button_up(mouse_button button);

/**
 * @brief Checks if a mouse button was released in the current frame
 * @param button The mouse button to check
 * @return b8 TRUE if the mouse button was released in the current frame, FALSE
 * otherwise
 */
FR_API b8 fr_input_is_button_released_this_frame(mouse_button button);

/**
 * @brief Gets the current mouse position
 * @param x A pointer to an i32 to store the x position of the mouse
 * @param y A pointer to an i32 to store the y position of the mouse
 */
FR_API void fr_input_get_current_mouse_position(i32* x, i32* y);

/**
 * @brief Gets the last mouse position
 * @param x A pointer to an i32 to store the x position of the mouse
 * @param y A pointer to an i32 to store the y position of the mouse
 */
FR_API void fr_input_get_last_mouse_position(i32* x, i32* y);

/**
 * @brief Gets the change in mouse position since the last frame
 * @param delta_x A pointer to an i32 to store the change in x position of the
 * mouse
 * @param delta_y A pointer to an i32 to store the change in y position of the
 * mouse
 */
FR_API void fr_input_get_delta_mouse_position(i32* delta_x, i32* delta_y);

FR_API const char* fr_input_get_key_name(keys key);
