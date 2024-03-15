/**
 * @file vec4.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-02-29
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include <stdalign.h>
#include "fracture/core/defines.h"
#include "fracture/core/library/math/simd/sse.h"

typedef union vec4_u {
    alignas(16) f32 data[4];
    struct {
        union {
            f32 x;
            f32 r;
            f32 s;
        };
        union {
            f32 y;
            f32 g;
            f32 t;
        };
        union {
            f32 z;
            f32 b;
            f32 p;
            f32 width;
        };
        union {
            f32 w;
            f32 a;
            f32 q;
            f32 height;
        };
    };
#if FR_SIMD == 1
    alignas(16) __m128 simd;
#endif
} __attribute__((aligned(16))) vec4;

typedef vec4 colour;
typedef vec4 rect_2d;

typedef union bvec4_u {
    struct {
        union {
            b32 x;
            b32 r;
            b32 s;
        };
        union {
            b32 y;
            b32 g;
            b32 t;
        };
        union {
            b32 z;
            b32 b;
            b32 p;
        };
        union {
            b32 w;
            b32 a;
            b32 q;
        };
    };
    alignas(16) b32 data[4];
#if FR_SIMD == 1
    alignas(16) __m128i simd;
#endif
} __attribute__((aligned(16))) bvec4;