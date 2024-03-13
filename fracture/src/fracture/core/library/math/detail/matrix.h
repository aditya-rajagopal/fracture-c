/**
 * @file matrix.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-03-09
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include "fracture/core/defines.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "../simd/sse.h"

typedef union mat2s
{
    alignas(16) struct
    {
        f32 m00, m10;
        f32 m01, m11;
    };
    alignas(16) f32 data[4];
    alignas(16) vec2 columns[2];
#if FR_SIMD == 1
    alignas(16) __m128 simd;
#endif
} mat2;

typedef union mat3s
{
    struct
    {
        f32 m00, m10, m20;
        f32 m01, m11, m21;
        f32 m02, m12, m22;
    };
    f32 data[9];
    vec3 columns[3];
}  mat3;

typedef union mat4s
{
    alignas(16) struct
    {
        f32 m00, m10, m20, m30;
        f32 m01, m11, m21, m31;
        f32 m02, m12, m22, m32;
        f32 m03, m13, m23, m33;
    };
    alignas(16) f32 data[16];
    alignas(16) vec4 columns[4];
#if FR_SIMD == 1
    alignas(16) __m128 simd[4];
#endif
} mat4;

typedef mat4 mat4x4;
typedef mat3 mat3x3;
typedef mat2 mat2x2;

typedef union mat4x3s
{
    alignas(16) struct
    {
        f32 m00, m01, m02;
        f32 m10, m11, m12;
        f32 m20, m21, m22;
        f32 m30, m31, m32;
    };
    alignas(16) f32 data[12];
    vec4 columns[3];
#if FR_SIMD == 1
    alignas(16) __m128 simd[3];
#endif
} mat4x3;

typedef union mat4x2s
{
    alignas(16) struct
    {
        f32 m00, m01;
        f32 m10, m11;
        f32 m20, m21;
        f32 m30, m31;
    };
    alignas(16) f32 data[8];
    vec4 columns[2];
#if FR_SIMD == 1
    alignas(16) __m128 simd[2];
#endif
} mat4x2;

typedef union mat3x4s
{
    alignas(16) struct
    {
        f32 m00, m01, m02, m03;
        f32 m10, m11, m12, m13;
        f32 m20, m21, m22, m23;
    };
    alignas(16) f32 data[12];
    vec3 columns[4];
} mat3x4;

typedef union mat2x4s {
    alignas(16) struct {
        f32 m00, m01, m02, m03;
        f32 m10, m11, m12, m13;
    };
    alignas(16) f32 data[8];
    vec2 columns[4];
} mat2x4;
