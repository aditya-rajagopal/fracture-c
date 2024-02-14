#include "logging.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static const char* log_level_strings[] = {
    "FATAL",
    "ERROR",
    "WARN",
    "INFO",
    "TRACE",
    "ASSERTION FAILED"
};

static const char* log_level_colors[] = {
    COLOR_FATAL,
    COLOR_ERROR,
    COLOR_WARN,
    COLOR_INFO,
    COLOR_TRACE,
    COLOR_ASSERT
};

static logging_config* state = NULL_PTR;

b8 initialize_logging(logging_config* config) {
    if (state != NULL_PTR) {
        return TRUE;
    }

    state = (logging_config*)malloc(sizeof(logging_config));
    state->enable_console = config->enable_console;
    state->enable_file = config->enable_file;
    state->logging_flags = config->logging_flags;
    state->filename = config->filename;

    // TODO: Setup file logging

    return TRUE;
}

void shutdown_logging() {
    if (state == NULL_PTR) {
        return;
    }

    free(state);
}
 
void log_message(log_level level, const char* message, ...) {
    if (state == NULL_PTR) {
        return;
    }

    if (CHECK_BIT(state->logging_flags, level) == 0) {
        return;
    }

    char buffer[32000];
    __builtin_va_list vargs;
    va_start(vargs, message);
    vsnprintf(buffer, 32000, message, vargs);
    va_end(vargs);

    char log_message[32000];
    snprintf(log_message, 32000, "%s[%s] %s%s\n", log_level_colors[level], log_level_strings[level], buffer, COLOR_RESET);    

    if (state->enable_console) {
        printf("%s", log_message);
    }

    // TODO: Write to file
}

void log_message_detailed(log_level level, const char *file, int line, const char *format, ...) {
    if (state == NULL_PTR) {
        return;
    }

    if (CHECK_BIT(state->logging_flags, level) == 0) {
        return;
    }

    char buffer[32000];
    __builtin_va_list vargs;
    va_start(vargs, format);
    vsnprintf(buffer, 32000, format, vargs);
    va_end(vargs);

    char log_message[32000];
    snprintf(log_message, 32000, "%s[%s] %s:%d %s%s\n", log_level_colors[level], log_level_strings[level], file, line, buffer, COLOR_RESET);

    if (state->enable_console) {
        printf("%s", log_message);
    }

    // TODO: Write to file
}

b8 logging_file_status() {
    if (state == NULL_PTR) {
        return FALSE;
    }

    return state->enable_file;
}

b8 logging_console_status() {
    if (state == NULL_PTR) {
        return FALSE;
    }

    return state->enable_console;
}

void logging_file_set(b8 enabled) {
    if (state == NULL_PTR) {
        return;
    }

    state->enable_file = enabled;
}

void logging_console_set(b8 enabled) {
    if (state == NULL_PTR) {
        return;
    }

    state->enable_console = enabled;
}

b8 logging_level_get(log_level level) {
    if (state == NULL_PTR) {
        return FALSE;
    }

    return CHECK_BIT(state->logging_flags, level);
}

void logging_level_set(log_level level, b8 enabled) {
    if (state == NULL_PTR) {
        return;
    }

    if (enabled) {
        SET_BIT(state->logging_flags, level);
    } else {
        CLEAR_BIT(state->logging_flags, level);
    }
}