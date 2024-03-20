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

#include "fracture/core/defines.h"
#include "fracture/core/library/math/simd/sse.h"

typedef FR_ALIGN(16) union vec4_u {
    f32 data[4];
#if FR_SIMD == 1
    __m128 simd;
#endif
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
}  vec4;

typedef vec4 colour;
typedef vec4 rect_2d;
typedef vec4 quat;

typedef FR_ALIGN(16) union bvec4_u {
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
     b32 data[4];
#if FR_SIMD == 1
    __m128i simd;
#endif
} bvec4;