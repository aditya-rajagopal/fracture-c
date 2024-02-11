#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define LOG_BUFFER_SIZE 1024
#define MAX_LOG_MESSAGES 100

// ANSI color codes
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[37m"

// Debug levels
typedef enum {
    DEBUG_INFO,
    DEBUG_WARNING,
    DEBUG_ERROR
} DebugLevel;

// Log message structure
typedef struct {
    DebugLevel level;
    char message[LOG_BUFFER_SIZE];
} LogMessage;

// Mutex for thread safety
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

// Condition variable for signaling new log messages
static pthread_cond_t log_cond = PTHREAD_COND_INITIALIZER;

// Buffer for storing log messages
static LogMessage log_buffer[MAX_LOG_MESSAGES];
static int log_buffer_count = 0;

// Function to handle logging asynchronously
static void* log_async(void* args) {
    while(1) {
        pthread_mutex_lock(&log_mutex);
        // Wait until there's a log message in the buffer
        while (log_buffer_count == 0) {
            pthread_cond_wait(&log_cond, &log_mutex);
        }
        // Get the log message from the buffer
        LogMessage msg = log_buffer[0];
        // Shift remaining log messages in the buffer
        for (int i = 0; i < log_buffer_count - 1; ++i) {
            log_buffer[i] = log_buffer[i + 1];
        }
        log_buffer_count--;
        pthread_mutex_unlock(&log_mutex);
        
        // Print the log message
        pthread_mutex_lock(&log_mutex);
        switch (msg.level) {
            case DEBUG_INFO:
                printf(ANSI_COLOR_GREEN "[INFO] ");
                break;
            case DEBUG_WARNING:
                printf(ANSI_COLOR_YELLOW "[WARNING] ");
                break;
            case DEBUG_ERROR:
                printf(ANSI_COLOR_RED "[ERROR] ");
                break;
            default:
                printf("[UNKNOWN] ");
                break;
        }
        printf(ANSI_COLOR_RESET "%s\n", msg.message);
        pthread_mutex_unlock(&log_mutex);
    }
}

// Initialize logging system
void init_logging() {
    pthread_t tid;
    pthread_create(&tid, NULL, log_async, NULL);
    pthread_detach(tid);
}

// Log message
#define LOG(level, format, ...) \
    do { \
        pthread_mutex_lock(&log_mutex); \
        if (log_buffer_count < MAX_LOG_MESSAGES) { \
            LogMessage msg; \
            msg.level = level; \
            snprintf(msg.message, LOG_BUFFER_SIZE, format, ##__VA_ARGS__); \
            log_buffer[log_buffer_count++] = msg; \
            pthread_cond_signal(&log_cond); \
        } \
        pthread_mutex_unlock(&log_mutex); \
    } while(0)

#endif /* LOGGING_H */