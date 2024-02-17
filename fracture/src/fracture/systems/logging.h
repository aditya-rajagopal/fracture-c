/**
 * @file logging.h
 * @author Aditya Rajagopal
 * @brief Implements a simple logging system with support for different logging levels. Can be configured to log to a file and/or the console.
 * @version 0.0.1
 * @date 2024-02-13
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once
#include "fracture/fracture_core.h"

#include <core/systems/logging.h>

// Define logging macros

#define FR_CORE_FATAL(format, ...) log_message(LOG_LEVEL_FATAL, LOG_SOURCE_ENGINE, format, ##__VA_ARGS__)
#define FR_CORE_FATAL_DETAILED(format, ...) log_message_detailed(LOG_LEVEL_FATAL, LOG_SOURCE_ENGINE, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define FR_CORE_ERROR(format, ...) log_message(LOG_LEVEL_ERROR, LOG_SOURCE_ENGINE, format, ##__VA_ARGS__)
#define FR_CORE_ERROR_DETAILED(format, ...) log_message_detailed(LOG_LEVEL_ERROR, LOG_SOURCE_ENGINE, __FILE__, __LINE__, format, ##__VA_ARGS__)

#if ENABLE_WARN_LOGGING == 1
    #define FR_CORE_WARN(format, ...) log_message(LOG_LEVEL_WARN, LOG_SOURCE_ENGINE, format, ##__VA_ARGS__)
    #define FR_CORE_WARN_DETAILED(format, ...) log_message_detailed(LOG_LEVEL_WARN, LOG_SOURCE_ENGINE, __FILE__, __LINE__, format, ##__VA_ARGS__)
#else
    #define FR_CORE_WARN(format, ...)
    #define FR_CORE_WARN_DETAILED(format, ...)
#endif

#if ENABLE_INFO_LOGGING == 1
    #define FR_CORE_INFO(format, ...) log_message(LOG_LEVEL_INFO, LOG_SOURCE_ENGINE, format, ##__VA_ARGS__)
    #define FR_CORE_INFO_DETAILED(format, ...) log_message_detailed(LOG_LEVEL_INFO, LOG_SOURCE_ENGINE, __FILE__, __LINE__, format, ##__VA_ARGS__)
#else
    #define FR_CORE_INFO(format, ...)
    #define FR_CORE_INFO_DETAILED(format, ...)
#endif

#if ENABLE_TRACE_LOGGING == 1
    #define FR_CORE_TRACE(format, ...) log_message(LOG_LEVEL_TRACE, LOG_SOURCE_ENGINE, format, ##__VA_ARGS__)
    #define FR_CORE_TRACE_DETAILED(format, ...) log_message_detailed(LOG_LEVEL_TRACE, LOG_SOURCE_ENGINE, __FILE__, __LINE__, format, ##__VA_ARGS__)
#else
    #define FR_CORE_TRACE(format, ...)
    #define FR_CORE_TRACE_DETAILED(format, ...)
#endif

// Assert macros

#if FR_ENABLE_ASSERTS
    #define FR_CORE_ASSERT(expression) if (!(expression)) { log_message_detailed(LOG_LEVEL_ASSERT, LOG_SOURCE_ENGINE, __FILE__, __LINE__, #expression); DEBUG_BREAK(); }
#else
    #define FR_CORE_ASSERT(expression)
#endif


// Client logging macros

#define FR_FATAL(format, ...) log_message(LOG_LEVEL_FATAL, LOG_SOURCE_CLIENT, format, ##__VA_ARGS__)
#define FR_FATAL_DETAILED(format, ...) log_message_detailed(LOG_LEVEL_FATAL, LOG_SOURCE_CLIENT, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define FR_ERROR(format, ...) log_message(LOG_LEVEL_ERROR, LOG_SOURCE_CLIENT, format, ##__VA_ARGS__)
#define FR_ERROR_DETAILED(format, ...) log_message_detailed(LOG_LEVEL_ERROR, LOG_SOURCE_CLIENT, __FILE__, __LINE__, format, ##__VA_ARGS__)

#if ENABLE_WARN_LOGGING == 1
    #define FR_WARN(format, ...) log_message(LOG_LEVEL_WARN, LOG_SOURCE_CLIENT, format, ##__VA_ARGS__)
    #define FR_WARN_DETAILED(format, ...) log_message_detailed(LOG_LEVEL_WARN, LOG_SOURCE_CLIENT, __FILE__, __LINE__, format, ##__VA_ARGS__)
#else
    #define FR_WARN(format, ...)
    #define FR_WARN_DETAILED(format, ...)
#endif

#if ENABLE_INFO_LOGGING == 1
    #define FR_INFO(format, ...) log_message(LOG_LEVEL_INFO, LOG_SOURCE_CLIENT, format, ##__VA_ARGS__)
    #define FR_INFO_DETAILED(format, ...) log_message_detailed(LOG_LEVEL_INFO, LOG_SOURCE_CLIENT, __FILE__, __LINE__, format, ##__VA_ARGS__)
#else
    #define FR_INFO(format, ...)
    #define FR_INFO_DETAILED(format, ...)
#endif

#if ENABLE_TRACE_LOGGING == 1
    #define FR_TRACE(format, ...) log_message(LOG_LEVEL_TRACE, LOG_SOURCE_CLIENT, format, ##__VA_ARGS__)
    #define FR_TRACE_DETAILED(format, ...) log_message_detailed(LOG_LEVEL_TRACE, LOG_SOURCE_CLIENT, __FILE__, __LINE__, format, ##__VA_ARGS__)
#else
    #define FR_TRACE(format, ...)
    #define FR_TRACE_DETAILED(format, ...)
#endif

#if FR_ENABLE_ASSERTS
    #define FR_ASSERT(expression) if (!(expression)) { log_message_detailed(LOG_LEVEL_ASSERT, LOG_SOURCE_CLIENT, __FILE__, __LINE__, #expression); DEBUG_BREAK(); }
#else
    #define FR_ASSERT(expression)
#endif