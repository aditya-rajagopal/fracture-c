/**
 * @file application.h
 * @author Aditya Rajagopal
 * @brief Module that contains the functions to create and handle the application's lifecycle. 
 * @version 0.0.1
 * @date 2024-02-16
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include <fracture/fracture_core.h>
#include <fracture/engine/application_types.h>

/**
 * @brief Initializes the application with the given configuration
 * 
 * @param app_handle The handle for the application
 * @return b8 True if the application was initialized successfully, false otherwise
 */
FR_API b8 engine_initialize(application_handle* app_handle);

/**
 * @brief Shuts down the application and cleans up all the resources
 * 
 * @param app_handle The handle for the application 
 * @return b8 True if the application was shut down successfully, false otherwise
 */
FR_API b8 engine_shutdown(application_handle* aapp_handle);

/**
 * @brief Starts the application's main loop
 * 
 * @param app_handle The handle for the application
 * @return b8 True if the application ran successfully, false otherwise
 */
FR_API b8 engine_run(application_handle* app_handle);
