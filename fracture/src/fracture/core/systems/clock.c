#include "clock.h"

#include <platform.h>

#include "fracture/core/systems/logging.h"

void fr_clock_start(clock* clock) {
    if (!clock) {
        FR_CORE_FATAL("Clock is NULL");
        return;
    }

    clock->start_time = platform_get_absolute_time();
    clock->elapsed_time = 0.0F;
}

void fr_clock_update(clock* clock) {
    if (!clock) {
        FR_CORE_FATAL("Clock is NULL");
        return;
    }
    if (clock->start_time == 0.0) {
        FR_CORE_WARN("Clock has not been initialized");
        return;
    }
    f64 current_time = platform_get_absolute_time();
    clock->elapsed_time = current_time - clock->start_time;
}

void fr_clock_stop(clock* clock) {
    if (!clock) {
        FR_CORE_FATAL("Clock is NULL");
        return;
    }

    clock->start_time = 0.0;
    clock->elapsed_time = 0.0F;
}

f64 fr_clock_get_elapsed_time_s(clock* c) {
    if (!c) {
        FR_CORE_FATAL("Clock is NULL");
        return 0.0F;
    }

    return c->elapsed_time;
}

FR_API f64 fr_clock_get_elapsed_time_ms(clock* c) {
    if (!c) {
        FR_CORE_FATAL("Clock is NULL");
        return 0.0F;
    }

    return c->elapsed_time * 1000.0F;
}

FR_API f64 fr_clock_get_elapsed_time_us(clock* c) {
    if (!c) {
        FR_CORE_FATAL("Clock is NULL");
        return 0.0F;
    }

    return c->elapsed_time * 1000.0F * 1000.0F;
}
