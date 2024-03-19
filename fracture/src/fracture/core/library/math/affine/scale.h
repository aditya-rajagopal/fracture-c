/**
 * @file scale.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-03-13
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include "fracture/core/defines.h"
#include "../mat4.h"
#include "../simd/sse.h"

FR_FORCE_INLINE void fr_affine_scale(vec3* scale, mat4* transform) {
    transform->simd[0] = _mm_mul_ps(transform->simd[0], _mm_set1_ps(scale->x));
    transform->simd[1] = _mm_mul_ps(transform->simd[1], _mm_set1_ps(scale->y));
    transform->simd[2] = _mm_mul_ps(transform->simd[2], _mm_set1_ps(scale->z));
}

FR_FORCE_INLINE void fr_affine_scale_to(vec3* scale, mat4* transform, mat4* out) {
    out->simd[0] = _mm_mul_ps(transform->simd[0], _mm_set1_ps(scale->x));
    out->simd[1] = _mm_mul_ps(transform->simd[1], _mm_set1_ps(scale->y));
    out->simd[2] = _mm_mul_ps(transform->simd[2], _mm_set1_ps(scale->z));
    out->simd[3] = transform->simd[3];
}

FR_FORCE_INLINE void fr_affine_scales(f32 scale, mat4* transform) {
    __m128 s = _mm_set1_ps(scale);
    transform->simd[0] = _mm_mul_ps(transform->simd[0], s);
    transform->simd[1] = _mm_mul_ps(transform->simd[1], s);
    transform->simd[2] = _mm_mul_ps(transform->simd[2], s);
}

FR_FORCE_INLINE void fr_affine_scales_to(f32 scale, mat4* transform, mat4* out) {
    __m128 s = _mm_set1_ps(scale);
    out->simd[0] = _mm_mul_ps(transform->simd[0], s);
    out->simd[1] = _mm_mul_ps(transform->simd[1], s);
    out->simd[2] = _mm_mul_ps(transform->simd[2], s);
    out->simd[3] = transform->simd[3];
}

FR_FORCE_INLINE void fr_affine_scale_xyz(f32 x, f32 y, f32 z, mat4* transform) {
    transform->simd[0] = _mm_mul_ps(transform->simd[0], _mm_set1_ps(x));
    transform->simd[1] = _mm_mul_ps(transform->simd[1], _mm_set1_ps(y));
    transform->simd[2] = _mm_mul_ps(transform->simd[2], _mm_set1_ps(z));
}

FR_FORCE_INLINE void fr_affine_scale_xyz_to(f32 x, f32 y, f32 z, mat4* transform, mat4* out) {
    out->simd[0] = _mm_mul_ps(transform->simd[0], _mm_set1_ps(x));
    out->simd[1] = _mm_mul_ps(transform->simd[1], _mm_set1_ps(y));
    out->simd[2] = _mm_mul_ps(transform->simd[2], _mm_set1_ps(z));
    out->simd[3] = transform->simd[3];
}

FR_FORCE_INLINE void fr_affine_scale_x(f32 x, mat4* transform) {
    transform->simd[0] = _mm_mul_ps(transform->simd[0], _mm_set1_ps(x));
}

FR_FORCE_INLINE void fr_affine_scale_y(f32 y, mat4* transform) {
    transform->simd[1] = _mm_mul_ps(transform->simd[1], _mm_set1_ps(y));
}

FR_FORCE_INLINE void fr_affine_scale_z(f32 z, mat4* transform) {
    transform->simd[2] = _mm_mul_ps(transform->simd[2], _mm_set1_ps(z));
}

FR_FORCE_INLINE void fr_affine_left_scale(mat4* transform, vec3* scale) {
    transform->simd[0] = _mm_mul_ps(transform->simd[0], _mm_set1_ps(scale->x));
    transform->simd[1] = _mm_mul_ps(transform->simd[1], _mm_set1_ps(scale->y));
    transform->simd[2] = _mm_mul_ps(transform->simd[2], _mm_set1_ps(scale->z));
}
