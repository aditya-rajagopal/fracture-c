/**
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

#include "fracture/fracture_core.h"

typedef struct clock {
    f64 start_time;
    f64 elapsed_time;
} clock;

/**
 * @brief Initializes a clock. 
 * 
 * @param c The clock to initialize. 
 */
void fr_clock_start(clock* c);

/**
 * @brief Updates the clock.
 * @details This function should be called every frame to update the clock. It
 * will update the elapsed time.
 * @param c The clock to update.
 */
void fr_clock_update(clock* c);

/**
 * @brief Stops the clock.
 * @details This function should be called when the clock is no longer needed.
 * It will stop the clock and free any resources associated with it.
 * @param c The clock to stop.
 */
void fr_clock_stop(clock* c);
