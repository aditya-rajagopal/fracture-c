/**
 * @file vec3.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-02-29
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include "fracture/core/defines.h"
#include "fracture/core/library/math/simd/sse.h"
typedef union vec3_u {
    f32 data[3];
    struct {
       union {
           f32 x;
           f32 r;
           f32 u;
       };
       union {
           f32 y;
           f32 g;
           f32 v;
       };
       union {
           f32 z;
           f32 b;
           f32 w;
       };
#if FR_VEC3_SIMD == 1
       union {
           f32 padding;
       };
    };
    f32 data[4];
    __m128 simd;
} __attribute__((aligned(16))) vec3;
#else
    };
}  vec3;
#endif

typedef union ivec3_u {
    struct {
       union {
           i32 x;
           i32 r;
           i32 u;
       };
       union {
           i32 y;
           i32 g;
           i32 v;
       };
       union {
           i32 z;
           i32 b;
           i32 w;
       };
    };
    i32 data[3];
} ivec3;

typedef union uvec3_u {
    struct {
       union {
           u32 x;
           u32 r;
           u32 u;
       };
       union {
           u32 y;
           u32 g;
           u32 v;
       };
       union {
           u32 z;
           u32 b;
           u32 w;
       };
    };
    u32 data[3];
} uvec3;

typedef union dvec3_u {
    struct {
       union {
           f64 x;
           f64 r;
           f64 u;
       };
       union {
           f64 y;
           f64 g;
           f64 v;
       };
       union {
           f64 z;
           f64 b;
           f64 w;
       };
    };
    f64 data[3];
} dvec3;

// TODO: Add support for 8, 16, and 64 bit integers

typedef union i8vec3_u {
    struct {
       union {
           i8 x;
           i8 r;
           i8 u;
       };
       union {
           i8 y;
           i8 g;
           i8 v;
       };
       union {
           i8 z;
           i8 b;
           i8 w;
       };
    };
    i8 data[3];
} i8vec3;

typedef union i16vec3_u {
    struct {
       union {
           i16 x;
           i16 r;
           i16 u;
       };
       union {
           i16 y;
           i16 g;
           i16 v;
       };
       union {
           i16 z;
           i16 b;
           i16 w;
       };
    };
    i16 data[3];
} i16vec3;

typedef ivec3 i32vec3;

typedef union i64vec3_u {
    struct {
       union {
           i64 x;
           i64 r;
           i64 u;
       };
       union {
           i64 y;
           i64 g;
           i64 v;
       };
       union {
           i64 z;
           i64 b;
           i64 w;
       };
    };
    i64 data[3];
} i64vec3;

typedef union u8vec3_u {
    struct {
       union {
           u8 x;
           u8 r;
           u8 u;
       };
       union {
           u8 y;
           u8 g;
           u8 v;
       };
       union {
           u8 z;
           u8 b;
           u8 w;
       };
    };
    u8 data[3];
} u8vec3;

typedef union u16vec3_u {
    struct {
       union {
           u16 x;
           u16 r;
           u16 u;
       };
       union {
           u16 y;
           u16 g;
           u16 v;
       };
       union {
           u16 z;
           u16 b;
           u16 w;
       };
    };
    u16 data[3];
} u16vec3;

typedef uvec3 u32vec3;

typedef union u64vec3_u {
    struct {
       union {
           u64 x;
           u64 r;
           u64 u;
       };
       union {
           u64 y;
           u64 g;
           u64 v;
       };
       union {
           u64 z;
           u64 b;
           u64 w;
       };
    };
    u64 data[3];
} u64vec3;
