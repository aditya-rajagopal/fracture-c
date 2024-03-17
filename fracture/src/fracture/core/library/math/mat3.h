/**
 * @file mat3.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-03-10
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include "fracture/core/library/math/version.h"
#include "fracture/core/library/math/vec3.h"
#include "utils.h"
#include "simd/sse.h"

FR_FORCE_INLINE void fr_mat3_zero(mat3* m) {
    m->data[0] = 0.0f;
    m->data[1] = 0.0f;
    m->data[2] = 0.0f;
    m->data[3] = 0.0f;
    m->data[4] = 0.0f;
    m->data[5] = 0.0f;
    m->data[6] = 0.0f;
    m->data[7] = 0.0f;
    m->data[8] = 0.0f;
}

FR_FORCE_INLINE void fr_mat3_identity(mat3* m) {
    m->m00 = 1.0f;
    m->m11 = 1.0f;
    m->m22 = 1.0f;
    m->m01 = 0.0f;
    m->m02 = 0.0f;
    m->m10 = 0.0f;
    m->m12 = 0.0f;
    m->m20 = 0.0f;
    m->m21 = 0.0f;
}

FR_FORCE_INLINE void fr_mat3_random_uniform(mat3* m, f32 (*PFN_randf)(void* state, f32 min, f32 max), void* state, f32 min, f32 max) {
    m->m00 = PFN_randf(state, min, max);
    m->m01 = PFN_randf(state, min, max);
    m->m02 = PFN_randf(state, min, max);
    m->m10 = PFN_randf(state, min, max);
    m->m11 = PFN_randf(state, min, max);
    m->m12 = PFN_randf(state, min, max);
    m->m20 = PFN_randf(state, min, max);
    m->m21 = PFN_randf(state, min, max);
    m->m22 = PFN_randf(state, min, max);
}

FR_FORCE_INLINE void fr_mat3_fill(mat3* m, f32 val) {
    m->m00 = val;
    m->m01 = val;
    m->m02 = val;
    m->m10 = val;
    m->m11 = val;
    m->m12 = val;
    m->m20 = val;
    m->m21 = val;
    m->m22 = val;
}

FR_FORCE_INLINE void fr_mat3_diags(f32 val, mat3* m) {
    m->m00 = val;
    m->m11 = val;
    m->m22 = val;
    m->m01 = 0.0f;
    m->m02 = 0.0f;
    m->m10 = 0.0f;
    m->m12 = 0.0f;
    m->m20 = 0.0f;
    m->m21 = 0.0f;
}

FR_FORCE_INLINE void fr_mat3_diag(f32 m00, f32 m11, f32 m22, mat3* m) {
    m->m00 = m00;
    m->m11 = m11;
    m->m22 = m22;
    m->m01 = 0.0f;
    m->m02 = 0.0f;
    m->m10 = 0.0f;
    m->m12 = 0.0f;
    m->m20 = 0.0f;
    m->m21 = 0.0f;
}

FR_FORCE_INLINE void fr_mat3_diagv(vec3* v, mat3* m) {
    m->m00 = v->x;
    m->m11 = v->y;
    m->m22 = v->z;
    m->m01 = 0.0f;
    m->m02 = 0.0f;
    m->m10 = 0.0f;
    m->m12 = 0.0f;
    m->m20 = 0.0f;
    m->m21 = 0.0f;
}

FR_FORCE_INLINE void fr_mat3(const vec3* col0, const vec3* col1, const vec3* col2, mat3* m) {
    m->columns[0] = *col0;
    m->columns[1] = *col1;
    m->columns[2] = *col2;
}

FR_FORCE_INLINE void fr_mat3_copy(const mat3* restrict src, mat3* dst) {
    dst->m00 = src->m00;
    dst->m01 = src->m01;
    dst->m02 = src->m02;
    dst->m10 = src->m10;
    dst->m11 = src->m11;
    dst->m12 = src->m21;
    dst->m20 = src->m02;
    dst->m21 = src->m12;
    dst->m22 = src->m22;
}

FR_FORCE_INLINE void fr_mat3_write_column(const vec3* v, u32 column, mat3* m) {
    m->columns[column] = *v;
}


FR_FORCE_INLINE void fr_mat3_create(const f32* restrict src, mat3* dst) {
    dst->m00 = src[0];
    dst->m01 = src[1];
    dst->m02 = src[2];
    dst->m10 = src[3];
    dst->m11 = src[4];
    dst->m12 = src[5];
    dst->m20 = src[6];
    dst->m21 = src[7];
    dst->m22 = src[8];
}

//----------------------------------------------------------------------
// Utility functions
//----------------------------------------------------------------------

FR_FORCE_INLINE void fr_mat3_transpose(const mat3* restrict m, mat3* out) {
    out->m00 = m->m00;
    out->m01 = m->m10;
    out->m02 = m->m20;
    out->m10 = m->m01;
    out->m11 = m->m11;
    out->m12 = m->m21;
    out->m20 = m->m02;
    out->m21 = m->m12;
    out->m22 = m->m22;
}

FR_FORCE_INLINE void fr_mat3_transpose_inplace(mat3* m) {
    mat3 temp;
    fr_mat3_copy(m, &temp);
    m->m01 = temp.m10;
    m->m02 = temp.m20;
    m->m10 = temp.m01;
    m->m12 = temp.m21;
    m->m20 = temp.m02;
    m->m21 = temp.m12;
}

FR_FORCE_INLINE void fr_mat3_swapc(mat3* m, u32 c1, u32 c2) {
    if (c1 > 2 || c2 > 2) return;
    vec3 temp = m->columns[c1];
    m->columns[c1] = m->columns[c2];
    m->columns[c2] = temp;
}

FR_FORCE_INLINE void fr_mat3_swapr(mat3* m, u32 r1, u32 r2) {
    if (r1 > 2 || r2 > 2) return;
    vec3 temp = {.x = m->data[r1], .y = m->data[r1 + 3], .z = m->data[r1 + 6]};
    m->data[r1] = m->data[r2];
    m->data[r1 + 3] = m->data[r2 + 3];
    m->data[r1 + 6] = m->data[r2 + 6];
    m->data[r2] = temp.x;
    m->data[r2 + 3] = temp.y;
    m->data[r2 + 6] = temp.z;
}

FR_FORCE_INLINE b8 fr_mat3_eq(const mat3* a, const mat3* b) {
    return fr_vec3_eq(&a->columns[0], &b->columns[0]) &&
           fr_vec3_eq(&a->columns[1], &b->columns[1]) &&
           fr_vec3_eq(&a->columns[2], &b->columns[2]);
}

FR_FORCE_INLINE b8 fr_mat3_eqs(const mat3* m, f32 val) {
    return fr_vec3_eq_scalar(&m->columns[0], val) && fr_vec3_eq_scalar(&m->columns[1], val) &&
           fr_vec3_eq_scalar(&m->columns[2], val);
}

FR_FORCE_INLINE void fr_mat3_meq(const mat3* a, const mat3* b, mat3* out) {
    fr_vec3_veq(&a->columns[0], &b->columns[0], &out->columns[0]);
    fr_vec3_veq(&a->columns[1], &b->columns[1], &out->columns[1]);
    fr_vec3_veq(&a->columns[2], &b->columns[2], &out->columns[2]);
}

FR_FORCE_INLINE void fr_mat3_meqs(const mat3* m, f32 val, mat3* out) {
    fr_vec3_veqs(&m->columns[0], val, &out->columns[0]);
    fr_vec3_veqs(&m->columns[1], val, &out->columns[1]);
    fr_vec3_veqs(&m->columns[2], val, &out->columns[2]);
}

//----------------------------------------------------------------------
// Operations
//----------------------------------------------------------------------

FR_FORCE_INLINE void fr_mat3_add(const mat3* a, const mat3* b, mat3* out) {
    out->m00 = a->m00 + b->m00;
    out->m01 = a->m01 + b->m01;
    out->m02 = a->m02 + b->m02;
    out->m10 = a->m10 + b->m10;
    out->m11 = a->m11 + b->m11;
    out->m12 = a->m12 + b->m12;
    out->m20 = a->m20 + b->m20;
    out->m21 = a->m21 + b->m21;
    out->m22 = a->m22 + b->m22;
}

FR_FORCE_INLINE void fr_mat3_sub(const mat3* a, const mat3* b, mat3* out) {
    out->m00 = a->m00 - b->m00;
    out->m01 = a->m01 - b->m01;
    out->m02 = a->m02 - b->m02;
    out->m10 = a->m10 - b->m10;
    out->m11 = a->m11 - b->m11;
    out->m12 = a->m12 - b->m12;
    out->m20 = a->m20 - b->m20;
    out->m21 = a->m21 - b->m21;
    out->m22 = a->m22 - b->m22;
}

FR_FORCE_INLINE void fr_mat3_mul(const mat3* m1, const mat3* m2,
                                      mat3* out) {
#if FR_SIMD == 1
    // Taken from GLM library. This is a SIMD optimized version of matrix multiplication
    // I have not written this code, but I have modified it to fit the Fracture API
    // Still do not fully underand why this is faster than the non-SIMD version
    __m128 l0, l1, l2, r0, r1, r2, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9;

    l0 = _mm_loadu_ps(m1->data);
    l1 = _mm_loadu_ps(m1->data + 4);

    r0 = _mm_loadu_ps(m2->data);
    r1 = _mm_loadu_ps(m2->data + 4);

    x8 = FR_SIMD_SHUFFLE1(l0, 0, 2, 1, 0);                /* a00 a02 a01 a00 */
    x1 = FR_SIMD_SHUFFLE1(r0, 3, 0, 0, 0);                /* b10 b00 b00 b00 */
    x2 = _mm_shuffle_ps(l0, l1, _MM_SHUFFLE(1, 0, 3, 3)); /* a12 a11 a10 a10 */
    x3 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(2, 0, 3, 1)); /* b20 b11 b10 b01 */
    x0 = _mm_mul_ps(x8, x1);

    x6 = FR_SIMD_SHUFFLE1(l0, 1, 0, 2, 1); /* a01 a00 a02 a01 */
    x7 = FR_SIMD_SHUFFLE1(x3, 3, 3, 1, 1); /* b20 b20 b10 b10 */
    l2 = _mm_load_ss(m1->data + 8);
    r2 = _mm_load_ss(m2->data + 8);
    x1 = _mm_mul_ps(x6, x7);
    l2 = FR_SIMD_SHUFFLE1(l2, 0, 0, 1, 0); /* a22 a22 0.f a22 */
    r2 = FR_SIMD_SHUFFLE1(r2, 0, 0, 1, 0); /* b22 b22 0.f b22 */

    x4 = FR_SIMD_SHUFFLE1(x2, 0, 3, 2, 0); /* a10 a12 a11 a10 */
    x5 = FR_SIMD_SHUFFLE1(x2, 2, 0, 3, 2); /* a11 a10 a12 a11 */
    x6 = FR_SIMD_SHUFFLE1(x3, 2, 0, 0, 0); /* b11 b01 b01 b01 */
    x2 = FR_SIMD_SHUFFLE1(r1, 3, 3, 0, 0); /* b21 b21 b11 b11 */

    x8 = _mm_unpackhi_ps(x8, x4); /* a10 a00 a12 a02 */
    x9 = _mm_unpackhi_ps(x7, x2); /* b21 b20 b21 b20 */

    x0 = fr_simd_fmadd(x4, x6, x0);
    x1 = fr_simd_fmadd(x5, x2, x1);

    x2 = _mm_movehl_ps(l2, l1);                           /* a22 a22 a21 a20 */
    x3 = FR_SIMD_SHUFFLE1(x2, 0, 2, 1, 0);                /* a20 a22 a21 a20 */
    x2 = FR_SIMD_SHUFFLE1(x2, 1, 0, 2, 1);                /* a21 a20 a22 a21 */
    x4 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1, 1, 2, 2)); /* b12 b12 b02 b02 */

    x5 = FR_SIMD_SHUFFLE1(x4, 3, 0, 0, 0); /* b12 b02 b02 b02 */
    x4 = _mm_movehl_ps(r2, x4);            /* b22 b22 b12 b12 */
    x0 = fr_simd_fmadd(x3, x5, x0);
    x1 = fr_simd_fmadd(x2, x4, x1);

    /*
     Dot Product : dest[2][2] =  a02 * b20 +
                                 a12 * b21 +
                                 a22 * b22 +
                                 0   * 00                                    */
    x2 = _mm_movelh_ps(x8, l2); /* 0.f a22 a12 a02 */
    x3 = _mm_movelh_ps(x9, r2); /* 0.f b22 b21 b20 */
    x2 = fr_simd_vdot(x2, x3);

    _mm_storeu_ps(out->data, x0);
    _mm_storeu_ps(out->data + 4, x1);
    _mm_store_ss(out->data + 8, x2);
#else
    out->m00 = a->m00 * b->m00 + a->m01 * b->m10 + a->m02 * b->m20;
    out->m01 = a->m00 * b->m01 + a->m01 * b->m11 + a->m02 * b->m21;
    out->m02 = a->m00 * b->m02 + a->m01 * b->m12 + a->m02 * b->m22;
    out->m10 = a->m10 * b->m00 + a->m11 * b->m10 + a->m12 * b->m20;
    out->m11 = a->m10 * b->m01 + a->m11 * b->m11 + a->m12 * b->m21;
    out->m12 = a->m10 * b->m02 + a->m11 * b->m12 + a->m12 * b->m22;
    out->m20 = a->m20 * b->m00 + a->m21 * b->m10 + a->m22 * b->m20;
    out->m21 = a->m20 * b->m01 + a->m21 * b->m11 + a->m22 * b->m21;
    out->m22 = a->m20 * b->m02 + a->m21 * b->m12 + a->m22 * b->m22;
#endif
}

FR_FORCE_INLINE void fr_mat3_scale(const mat3* m, f32 s, mat3* out) {
    out->m00 = m->m00 * s;
    out->m01 = m->m01 * s;
    out->m02 = m->m02 * s;
    out->m10 = m->m10 * s;
    out->m11 = m->m11 * s;
    out->m12 = m->m12 * s;
    out->m20 = m->m20 * s;
    out->m21 = m->m21 * s;
    out->m22 = m->m22 * s;
}

FR_FORCE_INLINE f32 fr_mat3_trace(const mat3* m) {
    return m->m00 + m->m11 + m->m22;
}

FR_FORCE_INLINE void fr_mat3_tracev(const mat3* m, vec3* out) {
    out->x = m->m00;
    out->y = m->m11;
    out->z = m->m22;
}

FR_FORCE_INLINE f32 fr_mat3_det(const mat3* m) {
    f32 a = m->m00 * (m->m11 * m->m22 - m->m12 * m->m21);
    f32 b = m->m01 * (m->m10 * m->m22 - m->m12 * m->m20);
    f32 c = m->m02 * (m->m10 * m->m21 - m->m11 * m->m20);
    return a - b + c;
}

FR_FORCE_INLINE f32 fr_mat3_inv(const mat3* m, mat3* out) {
    f32 det = fr_mat3_det(m);
    if (det == 0.0f) {
        fr_mat3_zero(out);
        return 0.0f;
    }
    f32 inv_det = 1.0f / det;
    out->m00 = (m->m11 * m->m22 - m->m12 * m->m21) * inv_det;
    out->m01 = (m->m02 * m->m21 - m->m01 * m->m22) * inv_det;
    out->m02 = (m->m01 * m->m12 - m->m02 * m->m11) * inv_det;
    out->m10 = (m->m12 * m->m20 - m->m10 * m->m22) * inv_det;
    out->m11 = (m->m00 * m->m22 - m->m02 * m->m20) * inv_det;
    out->m12 = (m->m10 * m->m02 - m->m00 * m->m12) * inv_det;
    out->m20 = (m->m10 * m->m21 - m->m11 * m->m20) * inv_det;
    out->m21 = (m->m01 * m->m20 - m->m00 * m->m21) * inv_det;
    out->m22 = (m->m00 * m->m11 - m->m01 * m->m10) * inv_det;
    return det;
}

FR_FORCE_INLINE void fr_mat3_mulv(const mat3* m, const vec3* v, vec3* out) {
    out->x = m->m00 * v->x + m->m01 * v->y + m->m02 * v->z;
    out->y = m->m10 * v->x + m->m11 * v->y + m->m12 * v->z;
    out->z = m->m20 * v->x + m->m21 * v->y + m->m22 * v->z;
}

FR_FORCE_INLINE void fr_mat3_vmul(const vec3* v, const mat3* m, vec3* out_t) {
    out_t->x = m->m00 * v->x + m->m10 * v->y + m->m20 * v->z;
    out_t->y = m->m01 * v->x + m->m11 * v->y + m->m21 * v->z;
    out_t->z = m->m02 * v->x + m->m12 * v->y + m->m22 * v->z;
}

FR_FORCE_INLINE f32 fr_mat3_vmulv(const vec3* v1, const mat3* m, const vec3* v2) {
    vec3 temp;
    fr_mat3_vmul(v1, m, &temp);
    return fr_vec3_dot(&temp, v2);
}
