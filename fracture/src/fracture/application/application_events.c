#include "application_events.h"

#include "fracture/core/systems/event.h"

void fr_application_process_window_close() {
    event_data data = {0};
    fr_event_dispatch(EVENT_CODE_APPLICATION_QUIT, 0, data);
}