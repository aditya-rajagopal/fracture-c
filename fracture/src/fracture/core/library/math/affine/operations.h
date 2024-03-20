/**
 * @file operations.h
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

FR_FORCE_INLINE void fr_affine_mul(const mat4* a, const mat4* b, mat4* dst) {
#if FR_SIMD == 1
    __m128 c = a->simd[0];
    
    __m128 c0_b = b->simd[0];
    __m128 c1_b = b->simd[1];
    __m128 c2_b = b->simd[2];
    __m128 c3_b = b->simd[3];

    __m128 c0_d = _mm_mul_ps(FR_SIMD_SPLAT_X(c0_b), c); // c0 * b00
    __m128 c1_d = _mm_mul_ps(FR_SIMD_SPLAT_X(c1_b), c); // c0 * b01
    __m128 c2_d = _mm_mul_ps(FR_SIMD_SPLAT_X(c2_b), c); // c0 * b02
    __m128 c3_d = _mm_mul_ps(FR_SIMD_SPLAT_X(c3_b), c); // c0 * b03

    c = a->simd[1];
    c0_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_Y(c0_b), c0_d); // c0 * b00 + c1 * b10
    c1_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_Y(c1_b), c1_d); // c0 * b01 + c1 * b11
    c2_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_Y(c2_b), c2_d); // c0 * b02 + c1 * b12
    c3_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_Y(c3_b), c3_d); // c0 * b03 + c1 * b13

    c = a->simd[2];
    c0_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_Z(c0_b), c0_d); // c0 * b00 + c1 * b10 + c2 * b20
    c1_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_Z(c1_b), c1_d); // c0 * b01 + c1 * b11 + c2 * b21
    c2_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_Z(c2_b), c2_d); // c0 * b02 + c1 * b12 + c2 * b22
    c3_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_Z(c3_b), c3_d); // c0 * b03 + c1 * b13 + c2 * b23

    c = a->simd[3];
    c3_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_W(c3_b), c3_d); // c0 * b03 + c1 * b13 + c2 * b23 + c3 * b33
    // We dont need the other components because in the last row the first three components are 0

    dst->simd[0] = c0_d;
    dst->simd[1] = c1_d;
    dst->simd[2] = c2_d;
    dst->simd[3] = c3_d;
#endif
}

FR_FORCE_INLINE void fr_affine_mul_rot(const mat4* a, const mat4* b, mat4* dst) {
#if FR_SIMD == 1
    __m128 c = a->simd[0];
    
    __m128 c0_b = b->simd[0];
    __m128 c1_b = b->simd[1];
    __m128 c2_b = b->simd[2];

    __m128 c0_d = _mm_mul_ps(FR_SIMD_SPLAT_X(c0_b), c); // c0 * b00
    __m128 c1_d = _mm_mul_ps(FR_SIMD_SPLAT_X(c1_b), c); // c0 * b01
    __m128 c2_d = _mm_mul_ps(FR_SIMD_SPLAT_X(c2_b), c); // c0 * b02

    c = a->simd[1];
    c0_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_Y(c0_b), c0_d); // c0 * b00 + c1 * b10
    c1_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_Y(c1_b), c1_d); // c0 * b01 + c1 * b11
    c2_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_Y(c2_b), c2_d); // c0 * b02 + c1 * b12

    c = a->simd[2];
    c0_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_Z(c0_b), c0_d); // c0 * b00 + c1 * b10 + c2 * b20
    c1_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_Z(c1_b), c1_d); // c0 * b01 + c1 * b11 + c2 * b21
    c2_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_Z(c2_b), c2_d); // c0 * b02 + c1 * b12 + c2 * b22
    // We dont need the other components because we are only doing rotation matrix

    dst->simd[0] = c0_d;
    dst->simd[1] = c1_d;
    dst->simd[2] = c2_d;
    dst->simd[3] = a->simd[3];
#endif
}

FR_FORCE_INLINE void fr_affine_inv(const mat4* transform, mat4* out) {
#if FR_SIMD == 1
    // | R T | has the inverse | R^T -R^T * T |
    // | 0 1 |                 | 0          1 |
    __m128 c0 = transform->simd[0];
    __m128 c1 = transform->simd[1];
    __m128 c2 = transform->simd[2];
    __m128 c3 = transform->simd[3];
    __m128 identity_column = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);

    _MM_TRANSPOSE4_PS(c0, c1, c2, identity_column);

    __m128 x = FR_SIMD_SPLAT_X(c3);
    __m128 y = FR_SIMD_SPLAT_Y(c3);
    __m128 z = FR_SIMD_SPLAT_Z(c3);
    __m128 neg_mask = FR_SIGNMASK_NEGf32x4;

    __m128 T = _mm_mul_ps(c0, x);
    T = fr_simd_fmadd(c1, y, T);
    T = fr_simd_fmadd(c2, z, T);
    T = _mm_xor_ps(T, neg_mask);
    T = _mm_add_ps(T, identity_column);

    out->simd[0] = c0;
    out->simd[1] = c1;
    out->simd[2] = c2;
    out->simd[3] = T;
#endif
}
