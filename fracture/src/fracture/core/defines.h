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
#define KiB(x) (x * 1024)
#define MiB(x) (x * 1024 * 1024)
#define GiB(x) (x * 1024 * 1024 * 1024)

// kilobytes, megabytes, gigabytes
#define KB(x) (x * 1000)
#define MB(x) (x * 1000 * 1000)
#define GB(x) (x * 1000 * 1000 * 1000)

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

#ifdef _DEBUG
    #define CORE_DEBUG 1
#else
    #define CORE_DEBUG 0
#endif