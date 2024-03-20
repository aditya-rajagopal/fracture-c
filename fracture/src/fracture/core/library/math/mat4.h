/**
 * @file mat4.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-03-11
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include "fracture/core/defines.h"
#include "vec4.h"
#include "mat3.h"
#include "utils.h"
#include "simd/sse.h"
#include "detail/matrix.h"

#include "fracture/core/library/random/fr_random.h"

//--------------------------------------------------------------------------------------------
// Constructors
//--------------------------------------------------------------------------------------------

FR_FORCE_INLINE void fr_mat4_identity(mat4* m) {
#if FR_SIMD == 1
    m->simd[0] = _mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f);
    m->simd[1] = _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f);
    m->simd[2] = _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f);
    m->simd[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
#else
    m->m00 = 1.0f; m->m01 = 0.0f; m->m02 = 0.0f; m->m03 = 0.0f;
    m->m10 = 0.0f; m->m11 = 1.0f;  m->m12 = 0.0f; m->m13 = 0.0f;
    m->m20 = 0.0f; m->m21 = 0.0f; m->m22 = 1.0f; m->m23 = 0.0f;
    m->m30 = 0.0f; m->m31 = 0.0f; m->m32 = 0.0f; m->m33 = 1.0f;
#endif
}

FR_FORCE_INLINE void fr_mat4_zero(mat4* m) {
#if FR_SIMD == 1
    m->simd[0] = _mm_setzero_ps();
    m->simd[1] = _mm_setzero_ps();
    m->simd[2] = _mm_setzero_ps();
    m->simd[3] = _mm_setzero_ps();
#else
    m->m00 = 0.0f; m->m01 = 0.0f; m->m02 = 0.0f; m->m03 = 0.0f;
    m->m10 = 0.0f; m->m11 = 0.0f;  m->m12 = 0.0f; m->m13 = 0.0f;
    m->m20 = 0.0f; m->m21 = 0.0f; m->m22 = 0.0f; m->m23 = 0.0f;
    m->m30 = 0.0f; m->m31 = 0.0f; m->m32 = 0.0f; m->m33 = 0.0f;
#endif
}

FR_FORCE_INLINE void fr_mat4_diags(mat4* m, f32 diag) {
#if FR_SIMD == 1
    m->simd[0] = _mm_setr_ps(diag, 0.0f, 0.0f, 0.0f);
    m->simd[1] = _mm_setr_ps(0.0f, diag, 0.0f, 0.0f);
    m->simd[2] = _mm_setr_ps(0.0f, 0.0f, diag, 0.0f);
    m->simd[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, diag);
#else
    m->m00 = diag; m->m01 = 0.0f; m->m02 = 0.0f; m->m03 = 0.0f;
    m->m10 = 0.0f; m->m11 = diag;  m->m12 = 0.0f; m->m13 = 0.0f;
    m->m20 = 0.0f; m->m21 = 0.0f; m->m22 = diag; m->m23 = 0.0f;
    m->m30 = 0.0f; m->m31 = 0.0f; m->m32 = 0.0f; m->m33 = diag;
#endif
}

FR_FORCE_INLINE void fr_mat4_diagsv(mat4* m, vec4* diag) {
#if FR_SIMD == 1
    m->simd[0] = _mm_setr_ps(diag->x, 0.0f, 0.0f, 0.0f);
    m->simd[1] = _mm_setr_ps(0.0f, diag->y, 0.0f, 0.0f);
    m->simd[2] = _mm_setr_ps(0.0f, 0.0f, diag->z, 0.0f);
    m->simd[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, diag->w);
#else
    m->m00 = diag->x; m->m01 = 0.0f; m->m02 = 0.0f; m->m03 = 0.0f;
    m->m10 = 0.0f; m->m11 = diag->y;  m->m12 = 0.0f; m->m13 = 0.0f;
    m->m20 = 0.0f; m->m21 = 0.0f; m->m22 = diag->z; m->m23 = 0.0f;
    m->m30 = 0.0f; m->m31 = 0.0f; m->m32 = 0.0f; m->m33 = diag->w;
#endif
}

FR_FORCE_INLINE void fr_mat4_create(const f32* data, mat4* m) {
#if FR_SIMD == 1
    m->simd[0] = _mm_load_ps(data);
    m->simd[1] = _mm_load_ps(data + 4);
    m->simd[2] = _mm_load_ps(data + 8);
    m->simd[3] = _mm_load_ps(data + 12);
#else
    m->m00 = data[0]; m->m10 = data[1]; m->m20 = data[2]; m->m30 = data[3];
    m->m01 = data[4]; m->m11 = data[5]; m->m21 = data[6]; m->m31 = data[7];
    m->m02 = data[8]; m->m12 = data[9]; m->m22 = data[10]; m->m32 = data[11];
    m->m03 = data[12]; m->m13 = data[13]; m->m23 = data[14]; m->m33 = data[15];
#endif
}

FR_FORCE_INLINE void fr_mat4v(const vec4* c0, const vec4* c1, const vec4* c2,
                              const vec4* c3, mat4* m) {
#if FR_SIMD == 1
    m->simd[0] = c0->simd;
    m->simd[1] = c1->simd;
    m->simd[2] = c2->simd;
    m->simd[3] = c3->simd;
#else
    m->m00= c0->x; m->m10 = c0->y; m->m20 = c0->z; m->m30 = c0->w;
    m->m01 = c1->x; m->m11 = c1->y; m->m21 = c1->z; m->m31 = c1->w;
    m->m02 = c2->x; m->m12 = c2->y; m->m22 = c2->z; m->m32 = c2->w;
    m->m03 = c3->x; m->m13 = c3->y; m->m23 = c3->z; m->m33 = c3->w;
#endif
}

FR_FORCE_INLINE void fr_mat4_copy(const mat4* src, mat4* dst) {
#if FR_SIMD == 1
    dst->simd[0] = src->simd[0];
    dst->simd[1] = src->simd[1];
    dst->simd[2] = src->simd[2];
    dst->simd[3] = src->simd[3];
#else
    dst->m00 = src->m00; dst->m01 = src->m01; dst->m02 = src->m02; dst->m03 = src->m03;
    dst->m10 = src->m10; dst->m11 = src->m11; dst->m12 = src->m12; dst->m13 = src->m13;
    dst->m20 = src->m20; dst->m21 = src->m21; dst->m22 = src->m22; dst->m23 = src->m23;
    dst->m30 = src->m30; dst->m31 = src->m31; dst->m32 = src->m32; dst->m33 = src->m33;
#endif
}

FR_FORCE_INLINE void fr_mat4_copy_transpose(const mat4* src, mat4* dst)
{
#if FR_SIMD == 1
    dst->simd[0] = _mm_setr_ps(src->m00, src->m01, src->m02, src->m03);
    dst->simd[1] = _mm_setr_ps(src->m10, src->m11, src->m12, src->m13);
    dst->simd[2] = _mm_setr_ps(src->m20, src->m21, src->m22, src->m23);
    dst->simd[3] = _mm_setr_ps(src->m30, src->m31, src->m32, src->m33);
#else
    dst->m00 = src->m00; dst->m01 = src->m10; dst->m02 = src->m20; dst->m03 = src->m30;
    dst->m10 = src->m01; dst->m11 = src->m11; dst->m12 = src->m21; dst->m13 = src->m31;
    dst->m20 = src->m02; dst->m21 = src->m12; dst->m22 = src->m22; dst->m23 = src->m32;
    dst->m30 = src->m03; dst->m31 = src->m13; dst->m32 = src->m23; dst->m33 = src->m33;
#endif
}

FR_FORCE_INLINE void fr_mat4_write_mat3(const mat3* src, mat4* dst) {
    dst->m00 = src->m00; dst->m10 = src->m10; dst->m20 = src->m20;
    dst->m01 = src->m01; dst->m11 = src->m11; dst->m21 = src->m21;
    dst->m02 = src->m02; dst->m12 = src->m12; dst->m22 = src->m22;
}

FR_FORCE_INLINE void fr_mat4_to_mat3(const mat4* src, mat3* dst) {
    dst->m00 = src->m00; dst->m10 = src->m10; dst->m20 = src->m20;
    dst->m01 = src->m01; dst->m11 = src->m11; dst->m21 = src->m21;
    dst->m02 = src->m02; dst->m12 = src->m12; dst->m22 = src->m22;
}

FR_FORCE_INLINE void fr_mat4_to_mat3t(const mat4* src, mat3* dst) {
    dst->m00 = src->m00; dst->m10 = src->m01; dst->m20 = src->m02;
    dst->m01 = src->m10; dst->m11 = src->m11; dst->m21 = src->m12;
    dst->m02 = src->m20; dst->m12 = src->m21; dst->m22 = src->m22;
}

FR_FORCE_INLINE void fr_mat4_random_uniform_custom(mat4* m, f32 (*PFN_randf_range)(void* state, f32 min, f32 max), void* state, f32 min, f32 max) {
    FR_ALIGN(16) f32 data[16] = {0.0f};
    for (u32 i = 0; i < 16; i++) {
        data[i] = PFN_randf_range(state, min, max);
    }
    fr_mat4_create(data, m);
}

FR_FORCE_INLINE void fr_mat4_random_uniform(mat4* m, void* state) {
    FR_ALIGN(16) f32 data[16] = {0.0f};
    for (u32 i = 0; i < 16; i++) {
        data[i] = fr_random_uniform(state);
    }
    fr_mat4_create(data, m);
}
FR_FORCE_INLINE void fr_mat4_random_uniform_range(mat4* m, void* state, f32 min, f32 max) {
    FR_ALIGN(16) f32 data[16] = {0.0f};
    for (u32 i = 0; i < 16; i++) {
        data[i] = fr_random_uniform_range(state, min, max);
    }
    fr_mat4_create(data, m);
}

//--------------------------------------------------------------------------------------------
// Utility
//--------------------------------------------------------------------------------------------

FR_FORCE_INLINE void fr_mat4_transpose_inplace(mat4* m) {
#if FR_SIMD == 1
    _MM_TRANSPOSE4_PS(m->simd[0], m->simd[1], m->simd[2], m->simd[3]);
#else
    fr_swap(&m->m01, &m->m10);
    fr_swap(&m->m02, &m->m20);
    fr_swap(&m->m03, &m->m30);
    fr_swap(&m->m12, &m->m21);
    fr_swap(&m->m13, &m->m31);
    fr_swap(&m->m23, &m->m32);
#endif
}

FR_FORCE_INLINE void fr_mat4_transpose(const mat4* src, mat4* dst) {
#if FR_SIMD == 1
    __m128 c0 = _mm_unpacklo_ps(src->simd[0], src->simd[1]);
    __m128 c1 = _mm_unpacklo_ps(src->simd[2], src->simd[3]);
    __m128 c2 = _mm_unpackhi_ps(src->simd[0], src->simd[1]);
    __m128 c3 = _mm_unpackhi_ps(src->simd[2], src->simd[3]);
    dst->simd[0] = _mm_movelh_ps(c0, c1);
    dst->simd[1] = _mm_movehl_ps(c1, c0);
    dst->simd[2] = _mm_movelh_ps(c2, c3);
    dst->simd[3] = _mm_movehl_ps(c3, c2);
#else
    dst->m00 = src->m00; dst->m01 = src->m10; dst->m02 = src->m20; dst->m03 = src->m30;
    dst->m10 = src->m01; dst->m11 = src->m11; dst->m12 = src->m21; dst->m13 = src->m31;
    dst->m20 = src->m02; dst->m21 = src->m12; dst->m22 = src->m22; dst->m23 = src->m32;
    dst->m30 = src->m03; dst->m31 = src->m13; dst->m32 = src->m23; dst->m33 = src->m33;
#endif
}

FR_FORCE_INLINE void fr_mat4_swapc(mat4* src, u32 c1, u32 c2) {
#if FR_SIMD == 1
    __m128 c1_d = src->simd[c2];
    __m128 c2_d = src->simd[c1];
    src->simd[c1] = c1_d;
    src->simd[c2] = c2_d;
#else
#endif
}

FR_FORCE_INLINE void fr_mat4_swapr(mat4* src, u32 r1, u32 r2) {
    __attribute__((aligned(16))) f32 temp[4];
    temp[0] = src->data[4 * r1];
    temp[1] = src->data[4 * r1 + 1];
    temp[2] = src->data[4 * r1 + 2];
    temp[3] = src->data[4 * r1 + 3];

    src->data[4 * r1] = src->data[4 * r2];
    src->data[4 * r1 + 1] = src->data[4 * r2 + 1];
    src->data[4 * r1 + 2] = src->data[4 * r2 + 2];
    src->data[4 * r1 + 3] = src->data[4 * r2 + 3];

    src->data[4 * r2] = temp[0];
    src->data[4 * r2 + 1] = temp[1];
    src->data[4 * r2 + 2] = temp[2];
    src->data[4 * r2 + 3] = temp[3];
}

FR_FORCE_INLINE b8 fr_mat4_eq(const mat4* a, const mat4* b) {
#if FR_SIMD == 1
    __m128 threshold = _mm_set1_ps(FLOAT_EPSILON);
    __m128 c0 = _mm_cmplt_ps(fr_simd_abs(_mm_sub_ps(a->simd[0], b->simd[0])), threshold);
    __m128 c1 = _mm_cmplt_ps(fr_simd_abs(_mm_sub_ps(a->simd[1], b->simd[1])), threshold);
    __m128 c2 = _mm_cmplt_ps(fr_simd_abs(_mm_sub_ps(a->simd[2], b->simd[2])), threshold);
    __m128 c3 = _mm_cmplt_ps(fr_simd_abs(_mm_sub_ps(a->simd[3], b->simd[3])), threshold);
    return _mm_movemask_ps(_mm_and_ps(_mm_and_ps(c0, c1), _mm_and_ps(c2, c3))) == 0xf;
#else
    return fr_equal(a->m00, b->m00) && fr_equal(a->m01, b->m01) && fr_equal(a->m02, b->m02) && fr_equal(a->m03, b->m03) &&
           fr_equal(a->m10, b->m10) && fr_equal(a->m11, b->m11) && fr_equal(a->m12, b->m12) && fr_equal(a->m13, b->m13) &&
           fr_equal(a->m20, b->m20) && fr_equal(a->m21, b->m21) && fr_equal(a->m22, b->m22) && fr_equal(a->m23, b->m23) &&
           fr_equal(a->m30, b->m30) && fr_equal(a->m31, b->m31) && fr_equal(a->m32, b->m32) && fr_equal(a->m33, b->m33);
#endif
}

FR_FORCE_INLINE void fr_mat4_eqm(const mat4* a, const mat4* b, mat4* dst) {
#if FR_SIMD == 1
    __m128 threshold = _mm_set1_ps(FLOAT_EPSILON);
    __m128 one = _mm_set1_ps(1.0f);
    __m128 c0 = _mm_cmplt_ps(fr_simd_abs(_mm_sub_ps(a->simd[0], b->simd[0])), threshold);
    __m128 c1 = _mm_cmplt_ps(fr_simd_abs(_mm_sub_ps(a->simd[1], b->simd[1])), threshold);
    __m128 c2 = _mm_cmplt_ps(fr_simd_abs(_mm_sub_ps(a->simd[2], b->simd[2])), threshold);
    __m128 c3 = _mm_cmplt_ps(fr_simd_abs(_mm_sub_ps(a->simd[3], b->simd[3])), threshold);
    c0 = _mm_and_ps(c0, one);
    c1 = _mm_and_ps(c1, one);
    c2 = _mm_and_ps(c2, one);
    c3 = _mm_and_ps(c3, one);
    dst->simd[0] = c0;
    dst->simd[1] = c1;
    dst->simd[2] = c2;
    dst->simd[3] = c3;
#else
    dst->m00 = fr_equal(a->m00, b->m00);
    dst->m01 = fr_equal(a->m01, b->m01);
    dst->m02 = fr_equal(a->m02, b->m02);
    dst->m03 = fr_equal(a->m03, b->m03);
    dst->m10 = fr_equal(a->m10, b->m10);
    dst->m11 = fr_equal(a->m11, b->m11);
    dst->m12 = fr_equal(a->m12, b->m12);
    dst->m13 = fr_equal(a->m13, b->m13);
    dst->m20 = fr_equal(a->m20, b->m20);
    dst->m21 = fr_equal(a->m21, b->m21);
    dst->m22 = fr_equal(a->m22, b->m22);
    dst->m23 = fr_equal(a->m23, b->m23);
    dst->m30 = fr_equal(a->m30, b->m30);
    dst->m31 = fr_equal(a->m31, b->m31);
    dst->m32 = fr_equal(a->m32, b->m32);
    dst->m33 = fr_equal(a->m33, b->m33);
#endif
}

//--------------------------------------------------------------------------------------------
// Operations
//--------------------------------------------------------------------------------------------

FR_FORCE_INLINE void fr_mat4_mul(const mat4* a, const mat4* b, mat4* dst) {
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
    c0_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_W(c0_b), c0_d); // c0 * b00 + c1 * b10 + c2 * b20 + c3 * b30
    c1_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_W(c1_b), c1_d); // c0 * b01 + c1 * b11 + c2 * b21 + c3 * b31
    c2_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_W(c2_b), c2_d); // c0 * b02 + c1 * b12 + c2 * b22 + c3 * b32
    c3_d = fr_simd_fmadd(c, FR_SIMD_SPLAT_W(c3_b), c3_d); // c0 * b03 + c1 * b13 + c2 * b23 + c3 * b33

    dst->simd[0] = c0_d;
    dst->simd[1] = c1_d;
    dst->simd[2] = c2_d;
    dst->simd[3] = c3_d;
#else
    dst->m00 = a->m00 * b->m00 + a->m01 * b->m10 + a->m02 * b->m20 + a->m03 * b->m30;
    dst->m01 = a->m00 * b->m01 + a->m01 * b->m11 + a->m02 * b->m21 + a->m03 * b->m31;
    dst->m02 = a->m00 * b->m02 + a->m01 * b->m12 + a->m02 * b->m22 + a->m03 * b->m32;
    dst->m03 = a->m00 * b->m03 + a->m01 * b->m13 + a->m02 * b->m23 + a->m03 * b->m33;
    dst->m10 = a->m10 * b->m00 + a->m11 * b->m10 + a->m12 * b->m20 + a->m13 * b->m30;
    dst->m11 = a->m10 * b->m01 + a->m11 * b->m11 + a->m12 * b->m21 + a->m13 * b->m31;
    dst->m12 = a->m10 * b->m02 + a->m11 * b->m12 + a->m12 * b->m22 + a->m13 * b->m32;
    dst->m13 = a->m10 * b->m03 + a->m11 * b->m13 + a->m12 * b->m23 + a->m13 * b->m33;
    dst->m20 = a->m20 * b->m00 + a->m21 * b->m10 + a->m22 * b->m20 + a->m23 * b->m30;
    dst->m21 = a->m20 * b->m01 + a->m21 * b->m11 + a->m22 * b->m21 + a->m23 * b->m31;
    dst->m22 = a->m20 * b->m02 + a->m21 * b->m12 + a->m22 * b->m22 + a->m23 * b->m32;
    dst->m23 = a->m20 * b->m03 + a->m21 * b->m13 + a->m22 * b->m23 + a->m23 * b->m33;
    dst->m30 = a->m30 * b->m00 + a->m31 * b->m10 + a->m32 * b->m20 + a->m33 * b->m30;
    dst->m31 = a->m30 * b->m01 + a->m31 * b->m11 + a->m32 * b->m21 + a->m33 * b->m31;
    dst->m32 = a->m30 * b->m02 + a->m31 * b->m12 + a->m32 * b->m22 + a->m33 * b->m32;
    dst->m33 = a->m30 * b->m03 + a->m31 * b->m13 + a->m32 * b->m23 + a->m33 * b->m33;
#endif
}

FR_FORCE_INLINE void fr_mat4_mulv(const mat4* m, const vec4* v, vec4* dst) {
#if FR_SIMD == 1
    __m128 c = _mm_mul_ps(m->simd[0], FR_SIMD_SPLAT_X(v->simd));
    c = fr_simd_fmadd(m->simd[1], FR_SIMD_SPLAT_Y(v->simd), c);
    c = fr_simd_fmadd(m->simd[2], FR_SIMD_SPLAT_Z(v->simd), c);
    c = fr_simd_fmadd(m->simd[3], FR_SIMD_SPLAT_W(v->simd), c);
    dst->simd = c;
#else
    dst->x = m->m00 * v->x + m->m01 * v->y + m->m02 * v->z + m->m03 * v->w;
    dst->y = m->m10 * v->x + m->m11 * v->y + m->m12 * v->z + m->m13 * v->w;
    dst->z = m->m20 * v->x + m->m21 * v->y + m->m22 * v->z + m->m23 * v->w;
    dst->w = m->m30 * v->x + m->m31 * v->y + m->m32 * v->z + m->m33 * v->w;
#endif
}

// Multiply a vector by the transpose of a matrix
FR_FORCE_INLINE void fr_mat4_vmul(const vec4* v, const mat4* m, vec4* dst) {
#if FR_SIMD == 1
    mat4 mt;
    fr_mat4_transpose(m, &mt);
    fr_mat4_mulv(&mt, v, dst);
#else
    dst->x = m->m00 * v->x + m->m10 * v->y + m->m20 * v->z + m->m30 * v->w;
    dst->y = m->m01 * v->x + m->m11 * v->y + m->m21 * v->z + m->m31 * v->w;
    dst->z = m->m02 * v->x + m->m12 * v->y + m->m22 * v->z + m->m32 * v->w;
    dst->w = m->m03 * v->x + m->m13 * v->y + m->m23 * v->z + m->m33 * v->w;
#endif
}

FR_FORCE_INLINE void fr_mat4_mulv3(const mat4* m, const vec3* v, f32 w, vec3* dst) {
#if FR_SIMD == 1
    __m128 c = _mm_mul_ps(m->simd[0], _mm_set1_ps(v->x));
    c = fr_simd_fmadd(m->simd[1], _mm_set1_ps(v->y), c);
    c = fr_simd_fmadd(m->simd[2], _mm_set1_ps(v->z), c);
    c = fr_simd_fmadd(m->simd[3], _mm_set1_ps(w), c);
    FR_ALIGN(16) f32 f[4];
    _mm_store_ps(f, c);
    dst->x = f[0];
    dst->y = f[1];
    dst->z = f[2];
#else
#endif
}

FR_FORCE_INLINE void fr_mat4_scale(const mat4* m, f32 s, mat4* dst) {
#if FR_SIMD == 1
    __m128 c = _mm_set1_ps(s);
    dst->simd[0] = _mm_mul_ps(m->simd[0], c);
    dst->simd[1] = _mm_mul_ps(m->simd[1], c);
    dst->simd[2] = _mm_mul_ps(m->simd[2], c);
    dst->simd[3] = _mm_mul_ps(m->simd[3], c);
#else
    dst->m00 = m->m00 * s; dst->m01 = m->m01 * s; dst->m02 = m->m02 * s; dst->m03 = m->m03 * s;
    dst->m10 = m->m10 * s; dst->m11 = m->m11 * s; dst->m12 = m->m12 * s; dst->m13 = m->m13 * s;
    dst->m20 = m->m20 * s; dst->m21 = m->m21 * s; dst->m22 = m->m22 * s; dst->m23 = m->m23 * s;
    dst->m30 = m->m30 * s; dst->m31 = m->m31 * s; dst->m32 = m->m32 * s; dst->m33 = m->m33 * s;
#endif
}

FR_FORCE_INLINE void fr_mat4_add(const mat4* a, const mat4* b, mat4* dst) {
#if FR_SIMD == 1
    dst->simd[0] = _mm_add_ps(a->simd[0], b->simd[0]);
    dst->simd[1] = _mm_add_ps(a->simd[1], b->simd[1]);
    dst->simd[2] = _mm_add_ps(a->simd[2], b->simd[2]);
    dst->simd[3] = _mm_add_ps(a->simd[3], b->simd[3]);
#else
    dst->m00 = a->m00 + b->m00; dst->m01 = a->m01 + b->m01; dst->m02 = a->m02 + b->m02; dst->m03 = a->m03 + b->m03;
    dst->m10 = a->m10 + b->m10; dst->m11 = a->m11 + b->m11; dst->m12 = a->m12 + b->m12; dst->m13 = a->m13 + b->m13;
    dst->m20 = a->m20 + b->m20; dst->m21 = a->m21 + b->m21; dst->m22 = a->m22 + b->m22; dst->m23 = a->m23 + b->m23;
    dst->m30 = a->m30 + b->m30; dst->m31 = a->m31 + b->m31; dst->m32 = a->m32 + b->m32; dst->m33 = a->m33 + b->m33;
#endif
}

FR_FORCE_INLINE void fr_mat4_sub(const mat4* a, const mat4* b, mat4* dst) {
#if FR_SIMD == 1
    dst->simd[0] = _mm_sub_ps(a->simd[0], b->simd[0]);
    dst->simd[1] = _mm_sub_ps(a->simd[1], b->simd[1]);
    dst->simd[2] = _mm_sub_ps(a->simd[2], b->simd[2]);
    dst->simd[3] = _mm_sub_ps(a->simd[3], b->simd[3]);
#else
    dst->m00 = a->m00 - b->m00; dst->m01 = a->m01 - b->m01; dst->m02 = a->m02 - b->m02; dst->m03 = a->m03 - b->m03;
    dst->m10 = a->m10 - b->m10; dst->m11 = a->m11 - b->m11; dst->m12 = a->m12 - b->m12; dst->m13 = a->m13 - b->m13;
    dst->m20 = a->m20 - b->m20; dst->m21 = a->m21 - b->m21; dst->m22 = a->m22 - b->m22; dst->m23 = a->m23 - b->m23;
    dst->m30 = a->m30 - b->m30; dst->m31 = a->m31 - b->m31; dst->m32 = a->m32 - b->m32; dst->m33 = a->m33 - b->m33;
#endif
}

FR_FORCE_INLINE f32 fr_mat4_det(const mat4* m) {
    __m128 c0 = m->simd[0];
    __m128 c1 = m->simd[1];
    __m128 c2 = m->simd[2];
    __m128 c3 = m->simd[3];

    // We can calculate 2x2 determinants and store them in a temporary matrix
    __m128 d0 = _mm_mul_ps(FR_SIMD_SHUFFLE1(c2, 0, 0, 1, 1), FR_SIMD_SHUFFLE1(c3, 2, 3, 2, 3));
    d0 = fr_simd_fnmadd(FR_SIMD_SHUFFLE1(c2, 2, 3, 2, 3), FR_SIMD_SHUFFLE1(c3, 0, 0, 1, 1), d0);
    // Of the remaining 2x2s put 1 in the hi bits and 1 in the lo bits. This will be useful later.
    __m128 d1 = _mm_mul_ps(FR_SIMD_SHUFFLE1(c2, 0, 0, 2, 2), FR_SIMD_SHUFFLE1(c3, 1, 1, 3, 3));
    d1 = fr_simd_fnmadd(FR_SIMD_SHUFFLE1(c2, 1, 1, 3, 3), FR_SIMD_SHUFFLE1(c3, 0, 0, 2, 2), d1);

    // Calculate the 3x3 determinants now
    __m128 det = _mm_mul_ps(FR_SIMD_SHUFFLE1(c1, 0, 0, 0, 1), _mm_shuffle_ps(d1, d0, _MM_SHUFFLE(1, 0, 0, 0)));
    det = fr_simd_fnmadd(FR_SIMD_SHUFFLE1(c1, 1, 1, 2, 2), FR_SIMD_SHUFFLE1(d0, 3, 2, 2, 0), det);
    det = fr_simd_fmadd(FR_SIMD_SHUFFLE1(c1, 2, 3, 3, 3), _mm_shuffle_ps(d0, d1, _MM_SHUFFLE(2, 2, 3, 1)), det);
    det = _mm_mul_ps(c0, det);
    // now we need to add with the alternating signs
    det = _mm_xor_ps(det, FR_SIGNMASK_NPNPf32x4);
    return _mm_cvtss_f32(fr_simd_vhadd(det));
}

FR_FORCE_INLINE void fr_mat4_inv(const mat4* m, mat4* dst) {
    __m128 mask = FR_SIGNMASK_NPNPf32x4;
    __m128 inv_mask = FR_SIGNMASK_PNPNf32x4;
    __m128 c0 = m->simd[0];
    __m128 c1 = m->simd[1];
    __m128 c2 = m->simd[2];
    __m128 c3 = m->simd[3];

    // We need to calculate all the 2x2 determinants and store them in the
    // correct order to access them later
    __m128 e0 = _mm_movehl_ps(c3, c2);                // m33 m23 m32 m22
    __m128 e1 = _mm_movelh_ps(c2, c3);                // m13 m03 m12 m02
    __m128 e2 = FR_SIMD_SHUFFLE1(e0, 1, 3, 3, 3);     // m32 m33 m33 m33
    __m128 e3 = FR_SIMD_SHUFFLE1(e0, 0, 2, 2, 2);     // m22 m23 m23 m23
    __m128 e4 = FR_SIMD_SHUFFLE1(e1, 1, 3, 3, 3);     // m12 m13 m13 m13
    __m128 e5 = FR_SIMD_SHUFFLE1(e1, 0, 2, 2, 2);     // m02 m03 m03 m03
    e0 = FR_SIMD_SHUFFLE(c2, c1, 0, 0, 0, 0);         // m01 m01 m02 m02
    e1 = FR_SIMD_SHUFFLE(c2, c1, 1, 1, 1, 1);         // m11 m11 m12 m12
    __m128 e6 = FR_SIMD_SHUFFLE(c2, c1, 2, 2, 2, 2);  // m21 m21 m22 m22
    __m128 e7 = FR_SIMD_SHUFFLE(c2, c1, 3, 3, 3, 3);  // m31 m31 m32 m32

    __m128 d0 = _mm_mul_ps(e6, e2); // m21*m32 | m21*m33 | m22*m33 | m22*m33
    __m128 d1 = _mm_mul_ps(e1, e2); // m11*m32 | m11*m33 | m12*m33 | m12*m33
    __m128 d2 = _mm_mul_ps(e1, e3); // m11*m22 | m11*m23 | m12*m23 | m12*m23
    __m128 d3 = _mm_mul_ps(e0, e2); // m01*m32 | m01*m33 | m02*m33 | m02*m33
    __m128 d4 = _mm_mul_ps(e0, e3); // m01*m22 | m01*m23 | m02*m23 | m02*m23
    __m128 d5 = _mm_mul_ps(e0, e4); // m01*m12 | m01*m13 | m02*m13 | m02*m13

    // d0 = m21*m32 - m22*m31 | 3
    //      m21*m33 - m23*m31 | 2
    //      m22*m33 - m23*m32 | 1
    //      m22*m33 - m23*m32 | 0
    d0 = fr_simd_fnmadd(e3, e7, d0);

    // d1 = m11*m32 - m12*m31 | 3
    //      m11*m33 - m13*m31 | 2
    //      m12*m33 - m13*m32 | 1
    //      m12*m33 - m13*m32 | 0
    d1 = fr_simd_fnmadd(e4, e7, d1);

    // d2 = m11*m22 - m12*m21 | 3
    //      m11*m23 - m13*m21 | 2
    //      m12*m23 - m13*m22 | 1
    //      m12*m23 - m13*m22 | 0
    d2 = fr_simd_fnmadd(e4, e6, d2);

    // d3 = m01*m32 - m02*m31 | 3
    //      m01*m33 - m03*m31 | 2
    //      m02*m33 - m03*m32 | 1
    //      m02*m33 - m03*m32 | 0
    d3 = fr_simd_fnmadd(e5, e7, d3);

    // d4 = m01*m22 - m02*m21 | 3
    //      m01*m23 - m03*m21 | 2
    //      m02*m23 - m03*m22 | 1
    //      m02*m23 - m03*m22 | 0
    d4 = fr_simd_fnmadd(e5, e6, d4);

    // d5 = m01*m12 - m02*m11 | 3
    //      m01*m13 - m03*m11 | 2
    //      m02*m13 - m03*m12 | 1
    //      m02*m13 - m03*m12 | 0
    d5 = fr_simd_fnmadd(e5, e1, d5);

    // Calculate the 3x3 determinants now
    e0 = _mm_movehl_ps(c1, c0); // m31 m21 m30 m20
    e1 = _mm_movelh_ps(c0, c1); // m11 m01 m10 m00

    e2 = FR_SIMD_SHUFFLE1(e1, 0, 0, 0, 2); // m00 m00 m00 m01
    e3 = FR_SIMD_SHUFFLE1(e1, 1, 1, 1, 3); // m10 m10 m10 m11
    e4 = FR_SIMD_SHUFFLE1(e0, 0, 0, 0, 2); // m20 m20 m20 m21
    e5 = FR_SIMD_SHUFFLE1(e0, 1, 1, 1, 3); // m30 m30 m30 m31

    __m128 inv0 = _mm_mul_ps(e3, d0); // m10*d0 | m10*d0 | m10*d0 | m11*d0
    __m128 inv1 = _mm_mul_ps(e2, d0); // m00*d0 | m00*d0 | m00*d0 | m01*d0
    __m128 inv2 = _mm_mul_ps(e2, d1); // m00*d1 | m00*d1 | m00*d1 | m01*d1
    __m128 inv3 = _mm_mul_ps(e2, d2); // m00*d2 | m00*d2 | m00*d2 | m01*d2

    inv0 = fr_simd_fnmadd(e4, d1, inv0); // m10*d0 - m20*d1 | m10*d0 - m20*d1 | m10*d0 - m20*d1 | m11*d0 - m21*d1
    inv1 = fr_simd_fnmadd(e4, d3, inv1); // m00*d0 - m20*d3 | m00*d0 - m20*d3 | m00*d0 - m20*d3 | m01*d0 - m21*d3
    inv2 = fr_simd_fnmadd(e3, d3, inv2); // m00*d1 - m10*d3 | m00*d1 - m10*d3 | m00*d1 - m10*d3 | m01*d1 - m11*d3
    inv3 = fr_simd_fnmadd(e3, d4, inv3); // m00*d2 - m10*d4 | m00*d2 - m10*d4 | m00*d2 - m10*d4 | m01*d2 - m11*d4

    // inv0 = m10*d0 - m20*d1 + m30*d2 | 3 
    //        m10*d0 - m20*d1 + m30*d2 | 2
    //        m10*d0 - m20*d1 + m30*d2 | 1
    //        m11*d0 - m21*d1 + m31*d2 | 0
    inv0 = fr_simd_fmadd(e5, d2, inv0);
    
    // inv1 = m00*d0 - m20*d3 + m30*d4 | 3
    //        m00*d0 - m20*d3 + m30*d4 | 2
    //        m00*d0 - m20*d3 + m30*d4 | 1
    //        m01*d0 - m21*d3 + m31*d4 | 0
    inv1 = fr_simd_fmadd(e5, d4, inv1);

    // inv2 = m00*d1 - m10*d3 + m30*d5 | 3
    //        m00*d1 - m10*d3 + m30*d5 | 2
    //        m00*d1 - m10*d3 + m30*d5 | 1
    //        m01*d1 - m11*d3 + m31*d5 | 0
    inv2 = fr_simd_fmadd(e5, d5, inv2);

    // inv3 = m00*d2 - m10*d4 + m20*d5 | 3
    //        m00*d2 - m10*d4 + m20*d5 | 2
    //        m00*d2 - m10*d4 + m20*d5 | 1
    //        m01*d2 - m11*d4 + m21*d5 | 0
    inv3 = fr_simd_fmadd(e4, d5, inv3);

    // now we need the right signs for the 3x3 determinants to create the adjugate matrix
    // the signs are alternating so we can use the mask to flip the signs (-1)^(i+j)
    inv0 = _mm_xor_ps(inv0, mask);     // inv0[3] | -inv0[2] | inv0[1] | -inv0[0]
    inv1 = _mm_xor_ps(inv1, inv_mask); // -inv1[3] | inv1[2] | -inv1[1] | inv1[0]
    inv2 = _mm_xor_ps(inv2, mask);     // inv2[3] | -inv2[2] | inv2[1] | -inv2[0]
    inv3 = _mm_xor_ps(inv3, inv_mask); // -inv3[3] | inv3[2] | -inv3[1] | inv3[0]

    // now we can calculate the determinant of the 4x4 matrix
    // take the first elements of all the adjugate matrix columns
    e0 = FR_SIMD_SHUFFLE(inv0, inv1, 0, 0, 0, 0);
    e1 = FR_SIMD_SHUFFLE(inv2, inv3, 0, 0, 0, 0);
    e0 = FR_SIMD_SHUFFLE(e0, e1, 2, 0, 2, 0); // -inv0[0] | inv1[0] | -inv2[0] | inv3[0]

    // e0 = _mm_rcp_ps(fr_simd_vhadd(_mm_mul_ps(e0, c0)));
    e0 = _mm_div_ps(_mm_set1_ps(1.0f), fr_simd_vhadd(_mm_mul_ps(e0, c0)));

    dst->simd[0] = _mm_mul_ps(inv0, e0);
    dst->simd[1] = _mm_mul_ps(inv1, e0);
    dst->simd[2] = _mm_mul_ps(inv2, e0);
    dst->simd[3] = _mm_mul_ps(inv3, e0);
}
