#include "event.h"

#include "fracture/core/systems/fracture_memory.h"
#include "fracture/core/systems/logging.h"

#include "fracture/core/containers/darrays.h"

#define MAX_EVENT_CODES 16384
#define INITIAL_EVENT_CALLBACK_SIZE 4

typedef struct event_callback {
    void* listener;
    PFN_on_event_callback callback;
} event_callback;

typedef struct event_code_callbacks {
    event_callback* callbacks;
} event_code_callbacks;

typedef struct event_system_state {
    event_code_callbacks registered_callbacks[MAX_EVENT_CODES];
} event_system_state;

static event_system_state state;
static b8 is_initialized = FALSE;

b8 fr_event_initialize() {
    if (is_initialized) {
        FR_CORE_WARN("Event system already initialized");
        return FALSE;
    }
    fr_memory_zero(&state, sizeof(event_system_state));
    is_initialized = TRUE;
    return TRUE;
}

b8 fr_event_shutdown() {
    if (!is_initialized) {
        return FALSE;
    }
    for (u32 i = 0; i < MAX_EVENT_CODES; i++) {
        if (state.registered_callbacks[i].callbacks != NULL_PTR) {
          darray_destroy(state.registered_callbacks[i].callbacks);
        }
        state.registered_callbacks[i].callbacks = NULL_PTR;
    }
    fr_memory_zero(&state, sizeof(event_system_state));
    is_initialized = FALSE;
    return TRUE;
}

b8 fr_event_register_handler(u16 event_code, void* listener_instance, PFN_on_event_callback callback) {
    if (!is_initialized) {
        return FALSE;
    }
    if (event_code >= MAX_EVENT_CODES) {
        FR_CORE_ERROR("Event code: %d exceeds maximum event code: %d", event_code, MAX_EVENT_CODES);
        return FALSE;
    }
    event_callback new_callback = { listener_instance, callback };
    event_callback* callbacks = state.registered_callbacks[event_code].callbacks;
    if (callbacks == NULL_PTR) {
        callbacks = darray_reserve(INITIAL_EVENT_CALLBACK_SIZE, event_callback);
        state.registered_callbacks[event_code].callbacks = callbacks;
    }
    u64 length = darray_length(callbacks);

    // Check if the listener instance and callback are already registered
    for (u64 i = 0; i < length; i++) {
        event_callback* handler = &callbacks[i];
        if (handler->listener == listener_instance && handler->callback == callback) {
            FR_CORE_WARN(
                "Listener instance: %p with callback: %p already registered for event code: %d",
                listener_instance, handler->callback, event_code);
            return FALSE;
        }
    }
    darray_push(callbacks, new_callback);
    return TRUE;
}

b8 fr_event_deregister_handler(u16 event_code, void* listener_instance, PFN_on_event_callback callback) {
    if (!is_initialized) {
        return FALSE;
    }
    if (event_code >= MAX_EVENT_CODES) {
        FR_CORE_ERROR("Event code: %d exceeds maximum event code: %d", event_code, MAX_EVENT_CODES);
        return FALSE;
    }
    event_callback* handlers = state.registered_callbacks[event_code].callbacks;
    if (handlers == NULL_PTR) {
        FR_CORE_WARN("No handlers registered for event code: %d", event_code);
        return FALSE;
    }
    u32 length = darray_length(handlers);
    for (u32 i = 0; i < length; i++) {
        event_callback* handler = &handlers[i];
        if (handler->listener == listener_instance && handler->callback == callback) {
            event_callback* removed;
            darray_pop_at(handlers, &removed, i);
            return TRUE;
        }
    }
    FR_CORE_WARN(
        "Listener instance: %p with callback: %p not found for event code: %d",
        listener_instance, callback, event_code);
    return FALSE;
}

b8 fr_event_dispatch(u16 event_code, void* sender, event_data data) {
    if (!is_initialized) {
        return FALSE;
    }
    if (event_code >= MAX_EVENT_CODES) {
        FR_CORE_ERROR("Event code: %d exceeds maximum event code: %d", event_code, MAX_EVENT_CODES);
        return FALSE;
    }
    event_callback* handlers = state.registered_callbacks[event_code].callbacks;
    if (handlers == NULL_PTR) {
        return TRUE;
    }
    u32 length = darray_length(handlers);
    for (u32 i = 0; i < length; i++) {
        event_callback* handler = &handlers[i];
        if(handler->callback(event_code, sender, handler->listener, data)) {
            // If the callback returns TRUE, the event has been handled and we can stop dispatching
            return TRUE;
        }
    }
    return TRUE;
}
