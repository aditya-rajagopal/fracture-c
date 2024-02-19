/**
 * @file system_event_codes.h
 * @author Aditya Rajagopal
 * @brief Contains all the system event codes for the Fracture Game Engine.
 * @version 0.0.1
 * @date 2024-02-19
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include "fracture/core/types/system_event_codes.h"

typedef enum system_event_code {
  // Mouse button codes
  /** @brief Left mouse button */
  EVENT_CODE_MOUSE_BUTTON_LEFT = 0x00,

  /** @brief Right mouse button */
  EVENT_CODE_MOUSE_BUTTON_RIGHT = 0x01,

  /** @brief Middle mouse button */
  EVENT_CODE_MOUSE_BUTTON_MIDDLE = 0x02,

  /** @brief The backspace key. */
  EVENT_CODE_KEY_BACKSPACE = 0x08,
  /** @brief The enter key. */
  EVENT_CODE_KEY_ENTER = 0x0D,
  /** @brief The tab key. */
  EVENT_CODE_KEY_TAB = 0x09,
  /** @brief The shift key. */
  EVENT_CODE_KEY_SHIFT = 0x10,
  /** @brief The Control/Ctrl key. */
  EVENT_CODE_KEY_CONTROL = 0x11,

  /** @brief The pause key. */
  EVENT_CODE_KEY_PAUSE = 0x13,
  /** @brief The Caps Lock key. */
  EVENT_CODE_KEY_CAPITAL = 0x14,

  /** @brief The Escape key. */
  EVENT_CODE_KEY_ESCAPE = 0x1B,

  EVENT_CODE_KEY_CONVERT = 0x1C,
  EVENT_CODE_KEY_NONCONVERT = 0x1D,
  EVENT_CODE_KEY_ACCEPT = 0x1E,
  EVENT_CODE_KEY_MODECHANGE = 0x1F,

  /** @brief The spacebar key. */
  EVENT_CODE_KEY_SPACE = 0x20,
  /** @brief The page up key. */
  EVENT_CODE_KEY_PAGEUP = 0x21,
  /** @brief The page down key. */
  EVENT_CODE_KEY_PAGEDOWN = 0x22,
  /** @brief The end key. */
  EVENT_CODE_KEY_END = 0x23,
  /** @brief The home key. */
  EVENT_CODE_KEY_HOME = 0x24,
  /** @brief The left arrow key. */
  EVENT_CODE_KEY_LEFT = 0x25,
  /** @brief The up arrow key. */
  EVENT_CODE_KEY_UP = 0x26,
  /** @brief The right arrow key. */
  EVENT_CODE_KEY_RIGHT = 0x27,
  /** @brief The down arrow key. */
  EVENT_CODE_KEY_DOWN = 0x28,
  EVENT_CODE_KEY_SELECT = 0x29,
  EVENT_CODE_KEY_PRINT = 0x2A,
  EVENT_CODE_KEY_EXECUTE = 0x2B,
  /** @brief The Print Screen key. */
  EVENT_CODE_KEY_PRINTSCREEN = 0x2C,
  /** @brief The insert key. */
  EVENT_CODE_KEY_INSERT = 0x2D,
  /** @brief The delete key. */
  EVENT_CODE_KEY_DELETE = 0x2E,
  EVENT_CODE_KEY_HELP = 0x2F,

  /** @brief The 0 key */
  EVENT_CODE_KEY_0 = 0x30,
  /** @brief The 1 key */
  EVENT_CODE_KEY_1 = 0x31,
  /** @brief The 2 key */
  EVENT_CODE_KEY_2 = 0x32,
  /** @brief The 3 key */
  EVENT_CODE_KEY_3 = 0x33,
  /** @brief The 4 key */
  EVENT_CODE_KEY_4 = 0x34,
  /** @brief The 5 key */
  EVENT_CODE_KEY_5 = 0x35,
  /** @brief The 6 key */
  EVENT_CODE_KEY_6 = 0x36,
  /** @brief The 7 key */
  EVENT_CODE_KEY_7 = 0x37,
  /** @brief The 8 key */
  EVENT_CODE_KEY_8 = 0x38,
  /** @brief The 9 key */
  EVENT_CODE_KEY_9 = 0x39,

  /** @brief The A key. */
  EVENT_CODE_KEY_A = 0x41,
  /** @brief The B key. */
  EVENT_CODE_KEY_B = 0x42,
  /** @brief The C key. */
  EVENT_CODE_KEY_C = 0x43,
  /** @brief The D key. */
  EVENT_CODE_KEY_D = 0x44,
  /** @brief The E key. */
  EVENT_CODE_KEY_E = 0x45,
  /** @brief The F key. */
  EVENT_CODE_KEY_F = 0x46,
  /** @brief The G key. */
  EVENT_CODE_KEY_G = 0x47,
  /** @brief The H key. */
  EVENT_CODE_KEY_H = 0x48,
  /** @brief The I key. */
  EVENT_CODE_KEY_I = 0x49,
  /** @brief The J key. */
  EVENT_CODE_KEY_J = 0x4A,
  /** @brief The K key. */
  EVENT_CODE_KEY_K = 0x4B,
  /** @brief The L key. */
  EVENT_CODE_KEY_L = 0x4C,
  /** @brief The M key. */
  EVENT_CODE_KEY_M = 0x4D,
  /** @brief The N key. */
  EVENT_CODE_KEY_N = 0x4E,
  /** @brief The O key. */
  EVENT_CODE_KEY_O = 0x4F,
  /** @brief The P key. */
  EVENT_CODE_KEY_P = 0x50,
  /** @brief The Q key. */
  EVENT_CODE_KEY_Q = 0x51,
  /** @brief The R key. */
  EVENT_CODE_KEY_R = 0x52,
  /** @brief The S key. */
  EVENT_CODE_KEY_S = 0x53,
  /** @brief The T key. */
  EVENT_CODE_KEY_T = 0x54,
  /** @brief The U key. */
  EVENT_CODE_KEY_U = 0x55,
  /** @brief The V key. */
  EVENT_CODE_KEY_V = 0x56,
  /** @brief The W key. */
  EVENT_CODE_KEY_W = 0x57,
  /** @brief The X key. */
  EVENT_CODE_KEY_X = 0x58,
  /** @brief The Y key. */
  EVENT_CODE_KEY_Y = 0x59,
  /** @brief The Z key. */
  EVENT_CODE_KEY_Z = 0x5A,

  /** @brief The left Windows/Super key. */
  EVENT_CODE_KEY_LSUPER = 0x5B,
  /** @brief The right Windows/Super key. */
  EVENT_CODE_KEY_RSUPER = 0x5C,
  /** @brief The applicatons key. */
  EVENT_CODE_KEY_APPS = 0x5D,

  /** @brief The sleep key. */
  EVENT_CODE_KEY_SLEEP = 0x5F,

  /** @brief The numberpad 0 key. */
  EVENT_CODE_KEY_NUMPAD0 = 0x60,
  /** @brief The numberpad 1 key. */
  EVENT_CODE_KEY_NUMPAD1 = 0x61,
  /** @brief The numberpad 2 key. */
  EVENT_CODE_KEY_NUMPAD2 = 0x62,
  /** @brief The numberpad 3 key. */
  EVENT_CODE_KEY_NUMPAD3 = 0x63,
  /** @brief The numberpad 4 key. */
  EVENT_CODE_KEY_NUMPAD4 = 0x64,
  /** @brief The numberpad 5 key. */
  EVENT_CODE_KEY_NUMPAD5 = 0x65,
  /** @brief The numberpad 6 key. */
  EVENT_CODE_KEY_NUMPAD6 = 0x66,
  /** @brief The numberpad 7 key. */
  EVENT_CODE_KEY_NUMPAD7 = 0x67,
  /** @brief The numberpad 8 key. */
  EVENT_CODE_KEY_NUMPAD8 = 0x68,
  /** @brief The numberpad 9 key. */
  EVENT_CODE_KEY_NUMPAD9 = 0x69,
  /** @brief The numberpad multiply key. */
  EVENT_CODE_KEY_MULTIPLY = 0x6A,
  /** @brief The numberpad add key. */
  EVENT_CODE_KEY_ADD = 0x6B,
  /** @brief The numberpad separator key. */
  EVENT_CODE_KEY_SEPARATOR = 0x6C,
  /** @brief The numberpad subtract key. */
  EVENT_CODE_KEY_SUBTRACT = 0x6D,
  /** @brief The numberpad decimal key. */
  EVENT_CODE_KEY_DECIMAL = 0x6E,
  /** @brief The numberpad divide key. */
  EVENT_CODE_KEY_DIVIDE = 0x6F,

  /** @brief The F1 key. */
  EVENT_CODE_KEY_F1 = 0x70,
  /** @brief The F2 key. */
  EVENT_CODE_KEY_F2 = 0x71,
  /** @brief The F3 key. */
  EVENT_CODE_KEY_F3 = 0x72,
  /** @brief The F4 key. */
  EVENT_CODE_KEY_F4 = 0x73,
  /** @brief The F5 key. */
  EVENT_CODE_KEY_F5 = 0x74,
  /** @brief The F6 key. */
  EVENT_CODE_KEY_F6 = 0x75,
  /** @brief The F7 key. */
  EVENT_CODE_KEY_F7 = 0x76,
  /** @brief The F8 key. */
  EVENT_CODE_KEY_F8 = 0x77,
  /** @brief The F9 key. */
  EVENT_CODE_KEY_F9 = 0x78,
  /** @brief The F10 key. */
  EVENT_CODE_KEY_F10 = 0x79,
  /** @brief The F11 key. */
  EVENT_CODE_KEY_F11 = 0x7A,
  /** @brief The F12 key. */
  EVENT_CODE_KEY_F12 = 0x7B,
  /** @brief The F13 key. */
  EVENT_CODE_KEY_F13 = 0x7C,
  /** @brief The F14 key. */
  EVENT_CODE_KEY_F14 = 0x7D,
  /** @brief The F15 key. */
  EVENT_CODE_KEY_F15 = 0x7E,
  /** @brief The F16 key. */
  EVENT_CODE_KEY_F16 = 0x7F,
  /** @brief The F17 key. */
  EVENT_CODE_KEY_F17 = 0x80,
  /** @brief The F18 key. */
  EVENT_CODE_KEY_F18 = 0x81,
  /** @brief The F19 key. */
  EVENT_CODE_KEY_F19 = 0x82,
  /** @brief The F20 key. */
  EVENT_CODE_KEY_F20 = 0x83,
  /** @brief The F21 key. */
  EVENT_CODE_KEY_F21 = 0x84,
  /** @brief The F22 key. */
  EVENT_CODE_KEY_F22 = 0x85,
  /** @brief The F23 key. */
  EVENT_CODE_KEY_F23 = 0x86,
  /** @brief The F24 key. */
  EVENT_CODE_KEY_F24 = 0x87,

  /** @brief The number lock key. */
  EVENT_CODE_KEY_NUMLOCK = 0x90,

  /** @brief The scroll lock key. */
  EVENT_CODE_KEY_SCROLL = 0x91,

  /** @brief The numberpad equal key. */
  EVENT_CODE_KEY_NUMPAD_EQUAL = 0x92,

  /** @brief The left shift key. */
  EVENT_CODE_KEY_LSHIFT = 0xA0,
  /** @brief The right shift key. */
  EVENT_CODE_KEY_RSHIFT = 0xA1,
  /** @brief The left control key. */
  EVENT_CODE_KEY_LCONTROL = 0xA2,
  /** @brief The right control key. */
  EVENT_CODE_KEY_RCONTROL = 0xA3,
  /** @brief The left alt key. */
  EVENT_CODE_KEY_LALT = 0xA4,
  /** @brief The right alt key. */
  EVENT_CODE_KEY_RALT = 0xA5,

  /** @brief The semicolon key. */
  EVENT_CODE_KEY_SEMICOLON = 0x3B,

  /** @brief The apostrophe/single-quote key */
  EVENT_CODE_KEY_APOSTROPHE = 0xDE,
  /** @brief An alias for EVENT_CODE_KEY_APOSTROPHE, apostrophe/single-quote key
   */
  EVENT_CODE_KEY_QUOTE = EVENT_CODE_KEY_APOSTROPHE,
  /** @brief The equal/plus key. */
  EVENT_CODE_KEY_EQUAL = 0xBB,
  /** @brief The comma key. */
  EVENT_CODE_KEY_COMMA = 0xBC,
  /** @brief The minus key. */
  EVENT_CODE_KEY_MINUS = 0xBD,
  /** @brief The period key. */
  EVENT_CODE_KEY_PERIOD = 0xBE,
  /** @brief The slash key. */
  EVENT_CODE_KEY_SLASH = 0xBF,

  /** @brief The grave key. */
  EVENT_CODE_KEY_GRAVE = 0xC0,

  /** @brief The left (square) bracket key e.g. [{ */
  EVENT_CODE_KEY_LBRACKET = 0xDB,
  /** @brief The pipe/backslash key */
  EVENT_CODE_KEY_PIPE = 0xDC,
  /** @brief An alias for the pipe/backslash key */
  EVENT_CODE_KEY_BACKSLASH = EVENT_CODE_KEY_PIPE,
  /** @brief The right (square) bracket key e.g. ]} */
  EVENT_CODE_KEY_RBRACKET = 0xDD,

  // Event to shut down the application in the next frame
  EVENT_CODE_APPLICATION_QUIT = 0xDE,

  /**
   * @brief Key press event
   * @details context: u16 key_code = data.data.du16[0]
   *                   u16 is_repeated = data.data.du16[1]
   *                   u16 mouse_x = data.data.du16[2]
   *                   u16 mouse_y = data.data.du16[3]
   *                   u16 pressed = data.data.du16[4]
   */
  EVENT_CODE_KEY_PRESS = 0xDF,

  /**
   * @brief Key release event
   * @details context: u16 key_code = data.data.du16[0]
   *                   u16 is_repeated = data.data.du16[1]
   *                   u16 mouse_x = data.data.du16[2]
   *                   u16 mouse_y = data.data.du16[3]
   *                   u16 pressed = data.data.du16[4]
   */
  EVENT_CODE_KEY_RELEASE = 0xE0,

  /**
   * @brief Mouse button press event
   * @details context: u16 button_code = data.data.du16[0]
   *                   u16 mouse_x = data.data.du16[1]
   *                   u16 mouse_y = data.data.du16[2]
   */
  EVENT_CODE_MOUSE_BUTTON_PRESS = 0xE1,

  /**
   * @brief Mouse button release event
   * @details context: u16 button_code = data.data.du16[0]
   *                  u16 mouse_x = data.data.du16[1]
   *                  u16 mouse_y = data.data.du16[2]
   */
  EVENT_CODE_MOUSE_BUTTON_RELEASE = 0xE2,

  /**
   * @brief Mouse move event
   * @details context: i16 x = data.data.di16[0], f32 y = data.data.di16[1]
   */
  EVENT_CODE_MOUSE_MOVE = 0xE3,

  /**
   * @brief Mouse scroll event
   * @details context: z_delta = data.data.u8[0]
   *                  u16 mouse_x = data.data.du16[1]
   *                  u16 mouse_y = data.data.du16[2]
   */
  EVENT_CODE_MOUSE_SCROLL = 0xE5,

  /**
   * @brief Window resize event
   * @details context: u16 width = data.data.du16[0], u16 height =
   * data.data.du16[1]
   */
  EVENT_CODE_WINDOW_RESIZE = 0xE6,

  // Engine event signals
  EVENT_CODE_ENGINE_MAIN_LOOP_BEGIN = 0xE7,

  EVENT_CODE_ENGINE_MAIN_LOOP_END = 0xE8,

  EVENT_CODE_ENGINE_FRAME_BEGIN = 0xE9,

  EVENT_CODE_ENGINE_FRAME_END = 0xEA,

  EVENT_CODE_ENGINE_RENDER_BEGIN = 0xEB,

  EVENT_CODE_ENGINE_RENDER_END = 0xEC,

  EVENT_CODE_ENGINE_UPDATE_BEGIN = 0xED,

  EVENT_CODE_ENGINE_UPDATE_END = 0xEE,

  EVENT_CODE_ENGINE_PHYSICS_BEGIN = 0xEF,

  EVENT_CODE_ENGINE_PHYSICS_END = 0xF0,

  MAX_SYSTEM_EVENT_CODE = 0xFF
} system_event_code;
