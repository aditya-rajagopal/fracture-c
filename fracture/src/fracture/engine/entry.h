#pragma once

#include "fracture/core/systems/fracture_memory.h"
#include "fracture/engine/application_types.h"
#include "fracture/engine/engine.h"
#include "fracture/fracture_core.h"

extern b8 create_client_application(application_handle* app_handle);

extern b8 destroy_client_application(application_handle* app_handle);

int main() {
    if (!fr_memory_initialize()) {
        return FR_EXIT_MEMORY_INIT_FAILURE;
    }

    application_handle app_handle;

    if (!create_client_application(&app_handle)) {
        return FR_EXIT_APPLICATION_CREATE_FAILURE;
    }

    if (!engine_initialize(&app_handle)) {
        return FR_EXIT_APPLICATION_INIT_FAILURE;
    }

    if (!engine_run(&app_handle)) {
        return FR_EXIT_APPLICATION_RUN_FAILURE;
    }

    if (!engine_shutdown(&app_handle)) {
        return FR_EXIT_APPLICATION_SHUTDOWN_FAILURE;
    }

    destroy_client_application(&app_handle);

    if (!fr_memory_shutdown()) {
        return FR_EXIT_MEMORY_SHUTDOWN_FAILURE;
    }
    return FR_EXIT_SUCCESS;
}
