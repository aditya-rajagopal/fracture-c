#include "engine_events.h"

#include "fracture/core/systems/event.h"

void fr_engine_process_window_close() {
    event_data data = {0};
    fr_event_dispatch(EVENT_CODE_APPLICATION_QUIT, 0, data);
}

void fr_engine_process_window_resize(u32 width, u32 height) {
    event_data data = {0};
    data.data.du32[0] = width;
    data.data.du32[1] = height;
    fr_event_dispatch(EVENT_CODE_WINDOW_RESIZE, 0, data);
}