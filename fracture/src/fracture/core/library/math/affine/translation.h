/**
 * @file translation.h
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

FR_FORCE_INLINE void fr_affine_left_tanslate(vec3* translation, mat4* transform) {
    transform->simd[3] = _mm_add_ps(
        transform->simd[0],
        _mm_setr_ps(translation->x, translation->y, translation->z, 1.0f));
}

FR_FORCE_INLINE void fr_affine_left_tanslate_to(vec3* translation, mat4* transform, mat4* out) {
    __m128 t = _mm_add_ps(
        transform->simd[0],
        _mm_setr_ps(translation->x, translation->y, translation->z, 1.0f));
    out->simd[0] = transform->simd[0];
    out->simd[1] = transform->simd[1];
    out->simd[2] = transform->simd[2];
    out->simd[3] = t;
}

FR_FORCE_INLINE void fr_affine_translate(mat4* transform, vec3* translation) {
    __m128 c0 = transform->simd[0];
    __m128 c1 = transform->simd[1];
    __m128 c2 = transform->simd[2];
    __m128 c3 = transform->simd[3];

    __m128 t = _mm_mul_ps(c0, _mm_set1_ps(translation->x));
    t = fr_simd_fmadd(c1, _mm_set1_ps(translation->y), t);
    t = fr_simd_fmadd(c2, _mm_set1_ps(translation->z), t);
    t = _mm_add_ps(c3, t);
    transform->simd[3] = t;
}

FR_FORCE_INLINE void fr_affine_translate_to(mat4* transform, vec3* translation, mat4* out) {
    __m128 c0 = transform->simd[0];
    __m128 c1 = transform->simd[1];
    __m128 c2 = transform->simd[2];
    __m128 c3 = transform->simd[3];

    __m128 t = _mm_mul_ps(c0, _mm_set1_ps(translation->x));
    t = fr_simd_fmadd(c1, _mm_set1_ps(translation->y), t);
    t = fr_simd_fmadd(c2, _mm_set1_ps(translation->z), t);
    t = _mm_add_ps(c3, t);

    out->simd[0] = c0;
    out->simd[1] = c1;
    out->simd[2] = c2;
    out->simd[3] = t;
}

FR_FORCE_INLINE void fr_affine_left_tanslate_x(f32 x, mat4* transform) {
    transform->m03 += x;
}

FR_FORCE_INLINE void fr_affine_left_tanslate_y(f32 y, mat4* transform) {
    transform->m13 += y;
}

FR_FORCE_INLINE void fr_affine_left_tanslate_z(f32 z, mat4* transform) {
    transform->m23 += z;
}

FR_FORCE_INLINE void fr_affine_translate_x(mat4* transform, f32 x) {
    __m128 c0 = transform->simd[0];
    __m128 c3 = transform->simd[3];
    c3 = _mm_add_ps(c3, _mm_mul_ps(c0, _mm_set1_ps(x)));
    transform->simd[3] = c3;
}

FR_FORCE_INLINE void fr_affine_translate_y(mat4* transform, f32 y) {
    __m128 c1 = transform->simd[1];
    __m128 c3 = transform->simd[3];
    c3 = _mm_add_ps(c3, _mm_mul_ps(c1, _mm_set1_ps(y)));
    transform->simd[3] = c3;
}

FR_FORCE_INLINE void fr_affine_translate_z(mat4* transform, f32 z) {
    __m128 c2 = transform->simd[2];
    __m128 c3 = transform->simd[3];
    c3 = _mm_add_ps(c3, _mm_mul_ps(c2, _mm_set1_ps(z)));
    transform->simd[3] = c3;
}
