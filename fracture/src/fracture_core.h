/**
 * @file defines.h
 * @author Aditya Rajagopal
 * @brief Defines all the types and macros used in the Fracture Game Engine
 * @version 0.0.1
 * @date 2024-02-10
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include <defines.h>

#if defined (WIN32) || defined (_WIN32) || defined (__WIN32__)
    #define FR_PLATFORM_WINDOWS  1
    #ifndef _WIN64
        #error "64-bit Windows is required"
    #endif
#elif defined (__linux__) || defined (__gnu_linux__)
    #define FR_PLATFORM_LINUX
    #ifndef __x86_64__
        #error "64-bit Linux is required"
    #endif
#else
    #error "Unsupported platform"
#endif

// Define export macro
#ifdef FR_EXPORT
    #ifdef _MSC_VER
        #define FR_API __declspec(dllexport)
    #else
        #define FR_API __attribute__((visibility("default")))
    #endif
#elif FR_IMPORT
    #ifdef _MSC_VER
        #define FR_API __declspec(dllimport)
    #else
        #define FR_API
    #endif
#else
    #error "FR_EXPORT or FR_IMPORT must be defined"
#endif

// Define debug macro
#ifdef _DEBUG
    #define FR_DEBUG 1
#else
    #define FR_DEBUG 0
#endif

#ifdef _RELEASE
    #define FR_RELEASE 1
#else
    #define FR_RELEASE 0
#endif

// Define debug break
#ifdef _ENABLE_ASSERTS
    #define DEBUG_BREAK() __debugbreak()
    #define FR_ENABLE_ASSERTS 1
#else
    #define DEBUG_BREAK()
#endif
