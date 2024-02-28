#include "platform.h"
#include <excpt.h>
#include <string.h>

#if PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h> // For GET_X_LPARAM and GET_Y_LPARAM
#include <stdlib.h>

/**
 * @brief Internal state for windows platform layer 
 * 
 */
typedef struct internal_state {
    /** @brief handle to the instance of the application */
    HINSTANCE hInstance;

    /** @brief handle to the window */
    HWND hWnd;
} internal_state;

static f64 clock_frequency;
static LARGE_INTEGER start_time;

static platform_state* plat_state;
static internal_state* state_ptr;

LRESULT CALLBACK _win32_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param);

b8 platform_startup(platform_state *platform_state, const char *window_title, u32 width, u32 height, u32 x_pos, u32 y_pos) {
    platform_state->internal_state = malloc(sizeof(internal_state));
    plat_state = platform_state;
    internal_state* state = (internal_state*)platform_state->internal_state;
    state_ptr = state;

    if (platform_state->on_key_event == NULL_PTR ||
        platform_state->on_mouse_move == NULL_PTR ||
        platform_state->on_mouse_button_event == NULL_PTR ||
        platform_state->on_mouse_scroll == NULL_PTR ||
        platform_state->on_window_resize == NULL_PTR) {
      return FALSE;
    }

    // Get the handle of a module that is cuurently running. 0 here indicates that you are asking for the handle of the current application that is running.
    state->hInstance = GetModuleHandleA(0);
    
    HICON icon = LoadIcon(state->hInstance, IDI_APPLICATION);
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(WNDCLASSA));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = _win32_process_message; // pointer to function that handles windows events
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = state->hInstance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "fracture_window_class"; // class name we are registering

    if(!RegisterClassA(&wc)) {
        MessageBoxA(0, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
    }

    // A window has 2 sizes technically. The entire window which includes the header and options and the client window which is the area which is usually where
    // we draw to.

    // Client window sizes
    u32 client_x_pos = x_pos;
    u32 client_y_pos = y_pos;
    u32 client_width = width;
    u32 client_height = height;
    
    // Final window sizes
    u32 window_x_pos = x_pos;
    u32 window_y_pos = y_pos;
    u32 window_width = client_width;
    u32 window_height = client_height;

    // Describes how the window looks
    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_THICKFRAME;

    u32 window_ex_style = WS_EX_APPWINDOW;
    
    // Obtain the size of the borders based on the styles chosen
    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

    window_x_pos += border_rect.left;
    window_y_pos += border_rect.top;

    window_width += border_rect.right - border_rect.left;
    window_height += border_rect.bottom - border_rect.top;

    HWND handle = CreateWindowExA(
        window_ex_style, "fracture_window_class", window_title,
        window_style, window_x_pos, window_y_pos, window_width,
        window_height, 0, 0, state->hInstance, 0
    );

    if (handle == NULL_PTR) {
        MessageBoxA(NULL, "Window Creation Failed", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
    } else {
        state->hWnd = handle;
    }

    // Time to show the window
    b32 should_activate = 1; // TODO: If the window does nto need to accept inputs this should be false;
    i32 show_window_commad_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
    // If initially minimized use SW_MINIMIZE : SW_SHOWMINNOACTIVE;
    // If initially maximized use SW_SHOWMAXIMIZED : SW_MAXIMIZE;
    ShowWindow(state->hWnd, show_window_commad_flags);

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clock_frequency = 1.0 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&start_time);

    return TRUE;
}

void platform_shutdown(platform_state *platform_state) {
    internal_state* state = (internal_state*)platform_state->internal_state;

    if (state->hWnd) {
        DestroyWindow(state->hWnd);
        state->hWnd = 0;
    }

    plat_state = NULL_PTR;
}

b8 platform_pump_messages(platform_state *platform_state) {
    MSG message;
    
    // We will be calling this function once every application loop. Windows has a stack of messages we need to handle.
    // We need to handle every message otherwise we will hand so we pass the PM_REMOVE flag.
    while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message); // calls the lpfnWndPrc which for us is the _win32_process_message.
    }

    return TRUE;
}

void* platform_allocate(u64 size, b8 aligned) {
    return malloc(size); // TODO: Temporary
}

void platform_free(void *block, b8 aligned) {
    free(block);
}

void* platform_zero_memory(void* block, u64 size) {
    return memset(block, 0, size);
}


void* platform_set_memory(void* block, i32 value, u64 size) {
    return memset(block, value, size);
}

void* platform_copy_memory(void* dest, const void* source, u64 size) {
    return memcpy(dest, source, size);
}

void platform_console_write(const char *message, u8 color) {
    OutputDebugStringA(message); // Lets us output to the debug console in addition to the standard console.

    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    // FATAL, ERROR, WARN, INFO, TRACE, ASSERTION
    static const u8 colors[] = {
        FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_RED | FOREGROUND_INTENSITY, // FATAL - White text on red background
        FOREGROUND_RED, // ERROR - Red
        FOREGROUND_RED | FOREGROUND_GREEN, // WARN - Yellow
        FOREGROUND_GREEN | FOREGROUND_BLUE, // INFO - Green
        FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED, // TRACE - Grey
        FOREGROUND_RED | FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED // ASSERTION - Red text on white background
    };
    SetConsoleTextAttribute(console_handle, colors[color]);

    u64 length = strlen(message);
    LPDWORD num_written = 0;
    // Windows has a more powerful console API than the standard C library. We can use the WriteConsoleA function to write to the console.
    WriteConsoleA(console_handle, message, (DWORD)length, num_written, 0);


}

void platform_console_write_error(const char *message, u8 color) {
    OutputDebugStringA(message); // Lets us output to the debug console in addition to the standard console.

    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    // FATAL, ERROR, WARN, INFO, TRACE, ASSERTION
    static const u8 colors[] = {
        FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_RED, // FATAL - White text on red background
        FOREGROUND_RED, // ERROR - Red
        FOREGROUND_RED | FOREGROUND_GREEN, // WARN - Yellow
        FOREGROUND_GREEN | FOREGROUND_BLUE, // INFO - Green
        FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED, // TRACE - Grey
        FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED // ASSERTION - Red text on white background
    };
    SetConsoleTextAttribute(console_handle, colors[color]);

    u64 length = strlen(message);
    LPDWORD num_written = 0;
    // Windows has a more powerful console API than the standard C library. We can use the WriteConsoleA function to write to the console.
    WriteConsoleA(console_handle, message, (DWORD)length, num_written, 0);
}

f64 platform_get_absolute_time() {
    LARGE_INTEGER current_time;
    QueryPerformanceCounter(&current_time);
    return (f64)current_time.QuadPart * clock_frequency;
}

void platform_sleep(u64 milliseconds) {
    Sleep(milliseconds);
}

void platform_get_handle_info(u64* out_size, void *memory) {
    *out_size = sizeof(internal_state);
    if (!memory) {
        return;
    }

    memcpy(memory, state_ptr, *out_size);
}

void platform_get_framebuffer_size(u32 *width, u32 *height) {
    RECT rect;
    GetClientRect(state_ptr->hWnd, &rect);
    *width = rect.right - rect.left;
    *height = rect.bottom - rect.top;
}

//*********************************************************************************************************************
//***********************************************PRIVATE FUNCTIONS*****************************************************
//*********************************************************************************************************************

LRESULT CALLBACK _win32_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
        case WM_ERASEBKGND:
            // This message is sent when the window background must be erased (for example, when a window is resized).
            // Return 1 tells the OS that we will handle this message and it should not do anything.
            return 1;
        case WM_CLOSE: {
            plat_state->on_window_close();
            return 0;
        } break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE: {
            RECT r;
            GetClientRect(hwnd, &r);
            u32 width = r.right - r.left;
            u32 height = r.bottom - r.top;
            plat_state->on_window_resize(width, height);
        } break;
        case WM_MOUSEMOVE: {
            // Mouse move event
            i32 x = GET_X_LPARAM(l_param);
            i32 y = GET_Y_LPARAM(l_param);
            plat_state->on_mouse_move(x, y);
        } break;
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP: {
            // Check if key pressed or released
            b8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            keys key = (u16)w_param;
            b8 is_extended = (HIWORD(l_param) & KF_EXTENDED) != KF_EXTENDED;

            if (w_param == VK_MENU) {
                key = is_extended ? KEY_RALT : KEY_LALT;
            } else if (w_param == VK_SHIFT) {
                u32 left_shift = MapVirtualKeyA(VK_LSHIFT, MAPVK_VK_TO_VSC);
                u32 scan_code = ((l_param & (0xFF << 16)) >> 16);
                key = (scan_code == left_shift) ? KEY_RSHIFT : KEY_LSHIFT;
            } else if (w_param == VK_CONTROL) {
                key = is_extended ? KEY_RCONTROL : KEY_LCONTROL;
            }
    
            if (key == VK_OEM_1) {
                key = KEY_SEMICOLON;
            }

            // Process the event
            plat_state->on_key_event(key, pressed);

            // Return 0 to indicate that we have handled the message and it should not be processed further
            return 0;
        }break;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP: {
            // Check if button pressed or released
            b8 pressed = (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN);
            switch (msg) {
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                    plat_state->on_mouse_button_event(MOUSE_BUTTON_LEFT, pressed);
                    break;
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                    plat_state->on_mouse_button_event(MOUSE_BUTTON_RIGHT, pressed);
                    break;
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                    plat_state->on_mouse_button_event(MOUSE_BUTTON_MIDDLE, pressed);
                    break;
            }
        } break;
        case WM_MOUSEWHEEL: {
            i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
            if (z_delta != 0) {
                // Normalize the delta to 1 or -1
                z_delta = z_delta / abs(z_delta);
            }
            plat_state->on_mouse_scroll(z_delta);
        } break;
    }

    // if we do not handle the message we pass it to the default window procedure
    return DefWindowProcA(hwnd, msg, w_param, l_param);
}

#endif // PLATFORM_WINDOWS
