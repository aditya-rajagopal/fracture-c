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

// Bit shifting
#define BIT(x) (1 << x)

// Bit manipulation
#define SET_BIT(x, y) (x |= BIT(y))
#define CLEAR_BIT(x, y) (x &= ~BIT(y))
#define TOGGLE_BIT(x, y) (x ^= BIT(y))
#define CHECK_BIT(x, y) (x & BIT(y))

// kibibytes, mebibytes, gibibytes
#define KiB(x) (x * 1024ULL)
#define MiB(x) (x * 1024ULL * 1024ULL)
#define GiB(x) (x * 1024ULL * 1024ULL * 1024ULL)

// kilobytes, megabytes, gigabytes
#define KB(x) (x * 1000ULL)
#define MB(x) (x * 1000ULL * 1000ULL)
#define GB(x) (x * 1000ULL * 1000ULL * 1000ULL)

// Min and Max and Clamp
#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)
#define CLAMP(x, min, max) (x < min ? min : (x > max ? max : x))

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

#ifdef _DEBUG
    #define CORE_DEBUG 1
#else
    #define CORE_DEBUG 0
#endif

// Taken from cglm library
#if defined(_MSC_VER)
/* do not use alignment for older visual studio versions */
#  if _MSC_VER < 1913 /*  Visual Studio 2017 version 15.6  */
#    define FR_ALL_UNALIGNED
#    define FR_ALIGN(X) /* no alignment */
#  else
#    define FR_ALIGN(X) __declspec(align(X))
#  endif
#else
#  define FR_ALIGN(X) __attribute((aligned(X)))
#endif

#if defined(FR_MATH_FORCE_INLINE)
#if defined(_MSC_VER)
#  define FR_FORCE_INLINE static __forceinline
#else
#  define FR_FORCE_INLINE static inline __attribute((always_inline))
#endif
#else
#define FR_FORCE_INLINE static inline
#endif

#if defined(_SIMD) && (defined(__SSE2__) || defined(__SSE__))
#define FR_SIMD 1
#else
#define FR_SIMD 0
#endif

#if FR_SIMD == 1 && defined(_VEC3_SIMD)
#define FR_VEC3_SIMD 1
#else
#define FR_VEC3_SIMD 0
#endif

// Define debug break
#ifdef _ENABLE_ASSERTS
    #define DEBUG_BREAK() __debugbreak()
    #define FR_ENABLE_ASSERTS 1
    #define FR_ASSERT(expression) \
        if (expression) {         \
        } else {                  \
            DEBUG_BREAK();        \
        }
#else
    #define DEBUG_BREAK()
    #define FR_ENABLE_ASSERTS 0
    #define FR_ASSERT(expression)
#endif

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
    #define FR_API
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