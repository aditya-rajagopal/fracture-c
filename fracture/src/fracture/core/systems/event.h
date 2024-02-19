/**
 * @file event.h
 * @author Aditya Rajagopal
 * @brief Contains the implementation of the event system.
 * @details The event system is responsible for handling events in the Fracture
 * Game Engine. To use the event system, you must first create an event handler
 * and then register it with the event system. Once registered, the event
 * handler will receive events from the event system and can process them
 * accordingly.
 * @version 0.0.1
 * @date 2024-02-17
 *
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 *
 */
#pragma once

#include "fracture/fracture_core.h"

typedef struct event_data {
    // 128 bytes of data
    union {
        u64 du64[2];
        i64 di64[2];
        f64 df64[2];

        u32 du32[4];
        i32 di32[4];
        f32 df32[4];

        u16 du16[8];
        i16 di16[8];

        u8 du8[16];
        i8 di8[16];

        char dchar[16];
    } data;
} event_data;

/** @brief  Function pointer definition for registering event handlers */
typedef b8 (*PFN_on_event_callback)(u16 event_code, void* sender, void* listener_instance, event_data data);

/**
 * @brief Initializes the event system for the Fracture Game Engine.
 * @details Initializes the event system and sets up the necessary data
 * structures for handling event registration and dispatch.
 * @return b8 True if the event system was successfully initialized, false otherwise.
 */
b8 fr_event_initialize();

/**
 * @brief Shuts down the event system for the Fracture Game Engine.
 * @details Shuts down the event system and frees any resources that were allocated during initialization.
 * @return b8 True if the event system was successfully shut down, false otherwise.
 */
b8 fr_event_shutdown();

/**
 * @brief Registers an event handler with the event system.
 * @details Registers an event handler with the event system. The event handler
 * will receive events from the event system and can process them accordingly.
 * @param event_code The ID of the event to register the handler for.
 * @param listener_instance A pointer to the instance of the listener that will
 * receive the event.
 * @param callback A function pointer to the event handler function.
 * @return b8 True if the event handler was successfully registered, false otherwise.
 */
FR_API b8 fr_event_register_handler(u16 event_code, void* listener_instance, PFN_on_event_callback callback);

/**
 * @brief Deregisters an event handler from the event system.
 * @details Deregisters an event handler from the event system. The event handler
 * will no longer receive events from the event system.
 * @param event_code The ID of the event to deregister the handler for.
 * @param listener_instance A pointer to the instance of the listener that will
 * no longer receive the event.
 * @param callback A function pointer to the event handler function.
 * @return b8 True if the event handler was successfully deregistered, false otherwise.
 */
FR_API b8 fr_event_deregister_handler(u16 event_code, void* listener_instance, PFN_on_event_callback callback);

/**
 * @brief Dispatches an event to all registered event handlers.
 * @details Dispatches an event to all registered event handlers. The event
 * handlers will receive the event and can process it accordingly.
 * @param event_code The ID of the event to dispatch.
 * @param sender A pointer to the instance of the sender that is dispatching the event.
 * @param data The data associated with the event.
 * @return b8 True if the event was successfully dispatched, false otherwise.
 */
FR_API b8 fr_event_dispatch(u16 event_code, void* sender, event_data data);

typedef enum system_event_code {
  // Event to shut down the application in the next frame
  EVENT_CODE_APPLICATION_QUIT = 0x01,

  /**
   * @brief Key press event
   * @details context: u16 key_code = data.data.du16[0]
   *                   u16 is_repeated = data.data.du16[1]
   *                   u16 mouse_x = data.data.du16[2]
   *                   u16 mouse_y = data.data.du16[3]
   */
  EVENT_CODE_KEY_PRESS = 0x02,

  /**
   * @brief Key release event
   * @details context: u16 key_code = data.data.du16[0]
   *                   u16 is_repeated = data.data.du16[1]
   *                   u16 mouse_x = data.data.du16[2]
   *                   u16 mouse_y = data.data.du16[3]
   */
  EVENT_CODE_KEY_RELEASE = 0x03,

  /**
   * @brief Mouse button press event
   * @details context: u16 button_code = data.data.du16[0]
   *                   u16 mouse_x = data.data.du16[1]
   *                   u16 mouse_y = data.data.du16[2]
   */
  EVENT_CODE_MOUSE_BUTTON_PRESS = 0x04,

  /**
   * @brief Mouse button release event
   * @details context: u16 button_code = data.data.du16[0]
   *                  u16 mouse_x = data.data.du16[1]
   *                  u16 mouse_y = data.data.du16[2]
   */
  EVENT_CODE_MOUSE_BUTTON_RELEASE = 0x05,

  /**
   * @brief Mouse move event
   * @details context: i16 x = data.data.di16[0], f32 y = data.data.di16[1]
   */
  EVENT_CODE_MOUSE_MOVE = 0x06,

  /**
   * @brief Mouse scroll event
   * @details context: z_delta = data.data.u8[0]
   *                  u16 mouse_x = data.data.du16[1]
   *                  u16 mouse_y = data.data.du16[2]
   */
  EVENT_CODE_MOUSE_SCROLL = 0x07,

  /**
   * @brief Window resize event
   * @details context: u16 width = data.data.du16[0], u16 height =
   * data.data.du16[1]
   */
  EVENT_CODE_WINDOW_RESIZE = 0x08,

  // Engine event signals
  EVENT_CODE_ENGINE_MAIN_LOOP_BEGIN = 0x09,

  EVENT_CODE_ENGINE_MAIN_LOOP_END = 0x0A,

  EVENT_CODE_ENGINE_FRAME_BEGIN = 0x0B,

  EVENT_CODE_ENGINE_FRAME_END = 0x0C,

  EVENT_CODE_ENGINE_RENDER_BEGIN = 0x0D,

  EVENT_CODE_ENGINE_RENDER_END = 0x0E,

  EVENT_CODE_ENGINE_UPDATE_BEGIN = 0x0F,

  EVENT_CODE_ENGINE_UPDATE_END = 0x10,

  EVENT_CODE_ENGINE_PHYSICS_BEGIN = 0x11,

  EVENT_CODE_ENGINE_PHYSICS_END = 0x12,

  MAX_SYSTEM_EVENT_CODE = 0xFF
} system_event_code;
