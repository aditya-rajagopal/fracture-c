/**
 * @file application_events.h
 * @author Aditya Rajagopal
 * @brief Contains the implementation of the application events to be handles.
 * @version 0.0.1
 * @date 2024-02-18
 *
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 *
 */
#pragma once

#include "fracture/core/defines.h"

/**
 * @brief Function called by the platform layer to process window close events.
 */
void fr_engine_process_window_close();

void fr_engine_process_window_resize(u32 width, u32 height);
