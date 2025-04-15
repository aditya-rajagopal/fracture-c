/* *
 * @file clock.h
 * @author Aditya Rajagopal
 * @brief A simple clock system for measuring time in the engine.
 * @version 0.0.1
 * @date 2024-02-19
 *
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 *
 */
#pragma once

#include "fracture/core/defines.h"

/**
 * @brief Structure to hold information about a clock
 *
 */
typedef struct clock_t {
    /** @brief The clock start time in seconds */
    f64 start_time;
    /** @brief The time elapsed since the start of the clock to when the last update was called */
    f64 elapsed_time;
} clock;

/**
 * @brief Initializes a clock.
 *
 * @param c The clock to initialize.
 */
FR_API void fr_clock_start(clock* c);

/**
 * @brief Updates the clock.
 * @details This function should be called every frame to update the clock. It
 * will update the elapsed time.
 * @param c The clock to update.
 */
FR_API void fr_clock_update(clock* c);

/**
 * @brief Stops the clock.
 * @details This function should be called when the clock is no longer needed.
 * It will stop the clock and free any resources associated with it.
 * @param c The clock to stop.
 */
FR_API void fr_clock_stop(clock* c);

/**
 * @brief Get the elapsed time of provided clock
 *
 * @param c The clock
 * @return f64 the elapsed time in seconds
 */
FR_API f64 fr_clock_get_elapsed_time_s(const clock* c);

/**
 * @brief Get the elapsed time of provided clock
 *
 * @param c The clock
 * @return f64 the elapsed time in milli-seconds
 */
FR_API f64 fr_clock_get_elapsed_time_ms(const clock* c);

/**
 * @brief Get the elapsed time of provided clock
 *
 * @param c The clock
 * @return f64 the elapsed time in micro-seconds
 */
FR_API f64 fr_clock_get_elapsed_time_us(const clock* c);

/**
 * @brief Get the elapsed time of provided clock
 *
 * @return f64 the current platform time
 */
FR_API f64 fr_clock_get_absolute_time_s();
