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

// Unsigned int types 
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

// Signed integer types 
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

// Float types
typedef float f32;
typedef double f64;

// Boolean type
typedef int b32;
typedef char b8;

// Null pointer
#define NULL_PTR 0

// Boolean values
#define TRUE 1
#define FALSE 0

// Pi
#define PI 3.14159265359f

// Define static assert
#if defined (__clang__) || defined (__gcc__)
    #define STATIC_ASSERT _Static_assert
#else
    #define STATIC_ASSERT static_assert
#endif


// Ensure all types are of the correct size
STATIC_ASSERT(sizeof(u8) == 1, "u8 is not 1 byte");
STATIC_ASSERT(sizeof(u16) == 2, "u16 is not 2 bytes");
STATIC_ASSERT(sizeof(u32) == 4, "u32 is not 4 bytes");
STATIC_ASSERT(sizeof(u64) == 8, "u64 is not 8 bytes");

STATIC_ASSERT(sizeof(i8) == 1, "i8 is not 1 byte");
STATIC_ASSERT(sizeof(i16) == 2, "i16 is not 2 bytes");
STATIC_ASSERT(sizeof(i32) == 4, "i32 is not 4 bytes");
STATIC_ASSERT(sizeof(i64) == 8, "i64 is not 8 bytes");

STATIC_ASSERT(sizeof(f32) == 4, "f32 is not 4 bytes");
STATIC_ASSERT(sizeof(f64) == 8, "f64 is not 8 bytes");

STATIC_ASSERT(sizeof(b32) == 4, "b32 is not 4 bytes");
STATIC_ASSERT(sizeof(b8) == 1, "b8 is not 1 byte");

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
#else
    #ifdef _MSC_VER
        #define FR_API __declspec(dllimport)
    #else
        #define FR_API
    #endif
#endif

// Define debug macro
#ifdef _DEBUG
    #define FR_DEBUG
#endif

// Define debug break
#ifdef FR_DEBUG
    #define DEBUG_BREAK __debugbreak()
#else
    #define DEBUG_BREAK
#endif
