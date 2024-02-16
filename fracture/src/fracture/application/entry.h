#pragma once

#include "fracture/fracture_core.h"
#include "fracture/application/app_types.h"
#include "fracture/application/application.h"

extern b8 create_client_application(application_handle* app_handle);

extern b8 destroy_client_application(application_handle* app_handle);

int main() {

    application_handle app_handle;

    if(!create_client_application(&app_handle)) {
        return FR_EXIT_APPLICATION_CREATE_FAILURE;
    }

    if(!application_initialize(&app_handle)) {
        return FR_EXIT_APPLICATION_INIT_FAILURE;
    }

    if(!application_run(&app_handle)) {
        return FR_EXIT_APPLICATION_RUN_FAILURE;
    }

    if(!application_shutdown(&app_handle)) {
        return FR_EXIT_APPLICATION_SHUTDOWN_FAILURE;
    }

    destroy_client_application(&app_handle);
    return FR_EXIT_SUCCESS;
}
