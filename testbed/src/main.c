#include <core/logging.h>

int main() {
    logging_config config = {0};
    config.enable_console = TRUE;
    config.enable_file = FALSE;
    config.logging_flags = FR_LOG_LEVEL_ALL;
    config.filename = NULL_PTR;
    if (!initialize_logging(&config)) {
        return 1;
    }

    FR_CORE_FATAL("This is a fatal message: %f", PI);
    FR_CORE_ERROR("This is an error message: %f", PI);
    FR_CORE_WARN("This is a warning message: %f", PI);
    FR_CORE_INFO("This is an info message: %f", PI);
    FR_CORE_TRACE("This is a trace message: %f", PI);
    FR_CORE_ASSERT(1 == 1);
    
    logging_level_set(LOG_LEVEL_INFO, FALSE);
    FR_CORE_INFO("This is an info message: %f", PI);
    shutdown_logging();
    return 0;
}
