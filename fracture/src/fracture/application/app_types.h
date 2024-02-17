/**
 * @file app_types.h
 * @author Aditya Rajagopal
 * @brief Contains types to handle application data and configuration
 * @version 0.0.1
 * @date 2024-02-16
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include "fracture/fracture_core.h"
#include "fracture/systems/logging.h"

/**
 * @brief Application configuration struct 
 * 
 */
typedef struct application_config {
    /** @brief Name of the application */
    const char* name;
    
    /** @brief Width of the application window */
    u32 width;

    /** @brief Height of the application window */
    u32 height;

    /** @brief X position of the application window */
    u32 x_pos;

    /** @brief Y position of the application window */
    u32 y_pos;

    /** @brief Logging module config */
    logging_config logging_config;
} application_config;

/**
 * @brief Application handle struct that contains all the necessary information for the application to run
 * 
 */
typedef struct application_handle {
    /** @brief Application configuration */
    application_config app_config;

    /** @brief Function pointer to initialize the application */
    b8 (*initialize)(struct application_handle* app_config);

    /** @brief Function pointer to update the application */
    b8 (*update)(struct application_handle* app_config, f32 delta_time);

    /** @brief Function pointer to applications render function */
    b8 (*render)(struct application_handle* app_config, f32 delta_time);

    /** @brief Application's on resize function */
    b8 (*on_resize)(struct application_handle* app_config, u32 width, u32 height);

    /** @brief Function pointer to the application shutdown function */
    b8 (*shutdown)(struct application_handle* app_config);

    /** @brief Pointer to application data that is managed by the application */
    void* application_data;
} application_handle;