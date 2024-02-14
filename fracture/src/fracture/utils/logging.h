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

/**
 * @brief Flags to enable or disable logging levels at bit position corresponding to the log level.
 * 
 */
typedef enum log_level_flag {
    FATAL_FLAG = BIT(0),
    ERROR_FLAG = BIT(1),
    WARN_FLAG = BIT(2),
    INFO_FLAG = BIT(3),
    TRACE_FLAG = BIT(4),
    ASSERT_FLAG = BIT(5)
} log_level_flag;

// Define some useful logging levels
#define FR_LOG_LEVEL_ALL (FATAL_FLAG | ERROR_FLAG | WARN_FLAG | INFO_FLAG | TRACE_FLAG | ASSERT_FLAG)
#define FR_LOG_LEVEL_NONE 0
#define FR_LOG_LEVEL_DEFAULT (FATAL_FLAG | ERROR_FLAG | WARN_FLAG | INFO_FLAG)
#define FR_LOG_LEVEL_ERROR (FATAL_FLAG | ERROR_FLAG)

typedef enum log_level {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_TRACE = 4,
    LOG_LEVEL_ASSERT = 5,
    TOTAL_LOG_LEVELS
} log_level;

#define ENABLE_WARN_LOGGING 1

#if FR_RELEASE == 1
    #define ENABLE_INFO_LOGGING 0
    #define ENABLE_TRACE_LOGGING 0
#else
    #define ENABLE_INFO_LOGGING 1
    #define ENABLE_TRACE_LOGGING 1
#endif

/**
 * @brief Configuration for the logging system to be defined and used by the application. 
 * 
 */
typedef struct logging_config {
    /** @brief The filename to write the log to. */
    const char *filename;

    /** @brief Enable the console output. */
    b8 enable_console;

    /** @brief Enable the file output. */
    b8 enable_file;

    /** @brief Flags to enable or disable logging levels at bit position corresponding to the log level. */
    b8 logging_flags;
} logging_config;

// HACK: This is just to test platform layer
FR_API void application_loop();

/**
 * @brief Initialize the logging system with the given configuration. The amount of memory required for the logging system is returned in the memory_required parameter if the config pointer passed is null.
 *        It is the responsibility of the caller to allocate the required memory and pass it to the function.
 * 
 * @param config The configuration to use for the logging system.
 * @param memory_required The amount of memory required for the logging system.
 * @return b8 True if the logging system was successfully initialized, false otherwise.
 */
FR_API b8 initialize_logging(logging_config* config);

/**
 * @brief Shutdown the logging system, closing any open files and freeing any allocated memory. 
 * 
 */
FR_API void shutdown_logging();

/**
 * @brief Log a message with the given level, file, line, and format string. 
 * 
 * @param level The level of the log message.
 * @param file The file where the log message was called.
 * @param line The line where the log message was called.
 * @param format The format string for the log message.
 * @param ... The arguments to the format string.
 */
FR_API void log_message_detailed(log_level level, const char* file, int line, const char* format, ...);

/**
 * @brief Log a message with the given level and format string. 
 * 
 * @param level The level of the log message.
 * @param format The format string for the log message.
 * @param ... The arguments to the format string.
 */
FR_API void log_message(log_level level, const char* format, ...);

/**
 * @brief Check if the given logging level is enabled. 
 * 
 * @param level The logging level to check.
 * @return b9 True if the logging level is enabled, false otherwise.
 */
b8 logging_level_get(log_level level);

/**
 * @brief Set the logging level to the given value. 
 * 
 * @param level The logging level to set.
 * @param enabled True to enable the logging level, false to disable it.
 */
FR_API void logging_level_set(log_level level, b8 enabled);

/**
 * @brief Check if console logging is enabled. 
 * 
 * @return b8 True if console logging is enabled, false otherwise.
 */
FR_API b8 logging_console_status();

/**
 * @brief Check if file logging is enabled. 
 * 
 * @return b8 True if file logging is enabled, false otherwise.
 */
FR_API b8 logging_file_status();

/**
 * @brief Set the console logging to the given value. 
 * 
 * @param enabled True to enable console logging, false to disable it.
 */
FR_API void logging_console_set(b8 enabled);

/**
 * @brief Set the file logging to the given value. 
 * 
 * @param enabled True to enable file logging, false to disable it.
 */
FR_API void logging_file_set(b8 enabled);

/**
 * @brief Set the filename to write the log to closes the current file if it is open.
 * 
 * @param filename The filename to write the log to.
 */
void logging_filename_set(const char* filename);

// Define logging macros

#define FR_CORE_FATAL(format, ...) log_message(LOG_LEVEL_FATAL, format, ##__VA_ARGS__)
#define FR_CORE_FATAL_DETAILED(format, ...) log_message_detailed(LOG_LEVEL_FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define FR_CORE_ERROR(format, ...) log_message(LOG_LEVEL_ERROR, format, ##__VA_ARGS__)
#define FR_CORE_ERROR_DETAILED(format, ...) log_message_detailed(LOG_LEVEL_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)

#if ENABLE_WARN_LOGGING == 1
    #define FR_CORE_WARN(format, ...) log_message(LOG_LEVEL_WARN, format, ##__VA_ARGS__)
    #define FR_CORE_WARN_DETAILED(format, ...) log_message_detailed(LOG_LEVEL_WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)
#else
    #define FR_CORE_WARN(format, ...)
    #define FR_CORE_WARN_DETAILED(format, ...)
#endif

#if ENABLE_INFO_LOGGING == 1
    #define FR_CORE_INFO(format, ...) log_message(LOG_LEVEL_INFO, format, ##__VA_ARGS__)
    #define FR_CORE_INFO_DETAILED(format, ...) log_message_detailed(LOG_LEVEL_INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)
#else
    #define FR_CORE_INFO(format, ...)
    #define FR_CORE_INFO_DETAILED(format, ...)
#endif

#if ENABLE_TRACE_LOGGING == 1
    #define FR_CORE_TRACE(format, ...) log_message(LOG_LEVEL_TRACE, format, ##__VA_ARGS__)
    #define FR_CORE_TRACE_DETAILED(format, ...) log_message_detailed(LOG_LEVEL_TRACE, __FILE__, __LINE__, format, ##__VA_ARGS__)
#else
    #define FR_CORE_TRACE(format, ...)
    #define FR_CORE_TRACE_DETAILED(format, ...)
#endif

// Assert macros

#if FR_ENABLE_ASSERTS
    #define FR_CORE_ASSERT(expression) if (!(expression)) { log_message_detailed(LOG_LEVEL_ASSERT, __FILE__, __LINE__, #expression); DEBUG_BREAK(); }
#else
    #define FR_CORE_ASSERT(expression)
#endif
