/**
 * @file mat2.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-03-09
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include <stdio.h>
#include <string.h>
#include <xmmintrin.h>
#include "fracture/core/defines.h"
#include "detail/matrix.h"
#include "simd/sse.h"
#include "vec2.h"

//-----------------------------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------------------------

FR_FORCE_INLINE void fr_mat2_identity(mat2* m) {
#if FR_SIMD == 1
    m->simd = _mm_setr_ps(1.0f, 0.0f, 0.0f, 1.0f);
#else
    m->m00 = 1.0f;
    m->m01 = 0.0f;
    m->m10 = 0.0f;
    m->m11 = 1.0f;
#endif
}

FR_FORCE_INLINE void fr_mat2_zero(mat2* m) {
#if FR_SIMD == 1
    m->simd = _mm_setzero_ps();
#else
    m->m00 = 0.0f;
    m->m01 = 0.0f;
    m->m10 = 0.0f;
    m->m11 = 0.0f;
#endif
}

FR_FORCE_INLINE void fr_mat2_fill(mat2* m, f32 val) {
#if FR_SIMD == 1
    m->simd = _mm_set1_ps(val);
#else
    m->m00 = val;
    m->m01 = val;
    m->m10 = val;
    m->m11 = val;
#endif
}

FR_FORCE_INLINE void fr_mat2_diags(f32 val, mat2* m) {
#if FR_SIMD == 1
    m->simd = _mm_setr_ps(val, 0.0f, 0.0f, val);
#else
    m->m00 = val;
    m->m01 = 0.0f;
    m->m10 = 0.0f;
    m->m11 = val;
#endif
}

FR_FORCE_INLINE void fr_mat2_diag(const vec2* v, mat2* m) {
#if FR_SIMD == 1
    m->simd = _mm_setr_ps(v->x, 0.0f, 0.0f, v->y);
#else
    m->m00 = v->x;
    m->m01 = 0.0f;
    m->m10 = 0.0f;
    m->m11 = v->y;
#endif
}

FR_FORCE_INLINE void fr_mat2(f32 m00, f32 m01, f32 m10, f32 m11, mat2* m) {
#if FR_SIMD == 1
    m->simd = _mm_setr_ps(m00, m10, m01, m11);
#else
    m->m00 = m00;
    m->m01 = m01;
    m->m10 = m10;
    m->m11 = m11;
#endif
}

FR_FORCE_INLINE void fr_mat2_from_vec2(vec2* col1, vec2* col2, mat2* m) {
#if FR_SIMD == 1
    m->simd = _mm_setr_ps(col1->x, col1->y, col2->x, col2->y);
#else
    m->columns[0] = *col1;
    m->columns[1] = *col2;
#endif
}

FR_FORCE_INLINE void fr_mat2_copy(const mat2* m, mat2* out) {
#if FR_SIMD == 1
    out->simd = m->simd;
#else
    out->m00 = m->m00;
    out->m01 = m->m01;
    out->m10 = m->m10;
    out->m11 = m->m11;
#endif
}

//-----------------------------------------------------------------------------------------------
// Operations
//-----------------------------------------------------------------------------------------------

FR_FORCE_INLINE void fr_mat2_transpose(const mat2* m, mat2* out) {
#if FR_SIMD == 1
    out->simd = FR_SIMD_SHUFFLE1(m->simd, 3, 1, 2, 0);
#else
    out->m00 = m->m00;
    out->m01 = m->m10;
    out->m10 = m->m01;
    out->m11 = m->m11;
#endif
}

FR_FORCE_INLINE void fr_mat2_mul(const mat2* left, const mat2* right, mat2* out) {
#if FR_SIMD == 1
    __m128 l = left->simd;
    __m128 r = right->simd;
    __m128 x0 = FR_SIMD_SHUFFLE1(r, 3, 3, 0, 0);
    __m128 x1 = FR_SIMD_SHUFFLE1(r, 2, 2, 1, 1);
    out->simd = _mm_add_ps(_mm_mul_ps(l, x0),
                           _mm_mul_ps(FR_SIMD_SHUFFLE1(l, 1, 0, 3, 2), x1));
#else
    out->m00 = left->m00 * right->m00 + left->m01 * right->m10;
    out->m01 = left->m00 * right->m01 + left->m01 * right->m11;
    out->m10 = left->m10 * right->m00 + left->m11 * right->m10;
    out->m11 = left->m10 * right->m01 + left->m11 * right->m11;
#endif
}

FR_FORCE_INLINE void fr_mat2_mulv(const mat2* m, const vec2* v, vec2* out) {
    out->x = m->m00 * v->x + m->m01 * v->y;
    out->y = m->m10 * v->x + m->m11 * v->y;
}

FR_FORCE_INLINE void fr_mat2_vmul(const vec2* v, const mat2* m, vec2* out) {
    out->x = m->m00 * v->x + m->m10 * v->y;
    out->y = m->m01 * v->x + m->m11 * v->y;
}

FR_FORCE_INLINE f32 fr_mat2_vmulv(const vec2* v1, const mat2* m, const vec2* v2) {
    vec2 temp;
    fr_mat2_vmul(v1, m, &temp);
    return fr_vec2_dot(&temp, v2);
}

FR_FORCE_INLINE f32 fr_mat2_determinant(const mat2* m) {
    return m->m00 * m->m11 - m->m01 * m->m10;
}

FR_FORCE_INLINE void fr_mat2_inverse(const mat2* m, mat2* out) {
    f32 det = fr_mat2_determinant(m);
    if (det == 0.0f) {
        fr_mat2_zero(out);
        return;
    }
    f32 inv_det = 1.0f / det;
    out->m00 = m->m11 * inv_det;
    out->m01 = -m->m01 * inv_det;
    out->m10 = -m->m10 * inv_det;
    out->m11 = m->m00 * inv_det;
}

FR_FORCE_INLINE void fr_mat2_scale(const mat2* m, f32 scale, mat2* out) {
    out->m00 = m->m00 * scale;
    out->m01 = m->m01 * scale;
    out->m10 = m->m10 * scale;
    out->m11 = m->m11 * scale;
}

FR_FORCE_INLINE void fr_mat2_add(const mat2* left, const mat2* right, mat2* out) {
#if FR_SIMD == 1
    out->simd = _mm_add_ps(left->simd, right->simd);
#else
    out->m00 = left->m00 + right->m00;
    out->m01 = left->m01 + right->m01;
    out->m10 = left->m10 + right->m10;
    out->m11 = left->m11 + right->m11;
#endif
}

FR_FORCE_INLINE void fr_mat2_sub(const mat2* left, const mat2* right, mat2* out) {
#if FR_SIMD == 1
    out->simd = _mm_sub_ps(left->simd, right->simd);
#else
    out->m00 = left->m00 - right->m00;
    out->m01 = left->m01 - right->m01;
    out->m10 = left->m10 - right->m10;
    out->m11 = left->m11 - right->m11;
#endif
}

FR_FORCE_INLINE f32 fr_mat2_trace(const mat2* m) {
    return m->m00 + m->m11;
}

FR_FORCE_INLINE f32 fr_mat2_frobenius_norm(const mat2* m) {
#if FR_SIMD == 1
    return _mm_cvtss_f32(fr_simd_vnorm(m->simd));
#else
    return fr_sqrt(m->m00 * m->m00 + m->m01 * m->m01 + m->m10 * m->m10 + m->m11 * m->m11);
#endif
}

FR_FORCE_INLINE void fr_mat2_print(char* name, int* len, const mat2* m, char* out_string) {
    char buffer[1024];
    u32 offset = 0;
    offset = snprintf(buffer, 1024, "mat2(float:%s) = \n", name);
    offset += snprintf(buffer + offset, 1024 - offset, "| %f, %f |\n", m->m00, m->m01);
    offset += snprintf(buffer + offset, 1024 - offset, "| %f, %f |\n", m->m10, m->m11);
    if (out_string == NULL_PTR) {
        *len = offset;
        return;
    }
    snprintf(out_string, *len, "%s", buffer);
}

//-----------------------------------------------------------------------------------------------
// Eigenvalues and Eigenvectors
//-----------------------------------------------------------------------------------------------

FR_FORCE_INLINE void fr_mat2_eigenvalues(const mat2* m, vec2* out) {
    f32 trace = fr_mat2_trace(m);
    f32 trace_squared = trace * trace;
    f32 det_term = fr_mat2_determinant(m) * 4.0f;
    if (trace_squared < det_term) {
        out[0].x = trace * 0.5f;
        out[0].y = fr_sqrt(det_term - trace_squared) * 0.5f;
        out[1].x = out[0].x;
        out[1].y = -out[0].y;
        return;
    }
    f32 sqrt_term = fr_sqrt(trace * trace - det_term);
    out[0].x = (trace + sqrt_term) * 0.5f;
    out[0].y = 0.0f;
    out[1].x = (trace - sqrt_term) * 0.5f;
    out[1].y = 0.0f;
}

FR_FORCE_INLINE void fr_mat2_eigenvectors(const mat2* m, const vec2* eigenvalues, mat2* out) {
    vec2 diff;
    fr_vec2_sub_to(&eigenvalues[0], &eigenvalues[1], &diff);
    vec2 zero = fr_vec2_zero();
    if (fr_vec2_eq(&diff, &zero)) {
        fr_mat2_identity(out);
        return;
    }
    vec2 temp;
    temp.x = 1.0f;
    temp.y = (eigenvalues[0].x - m->m00) / m->m01;
    fr_vec2_normalize(&temp);
    out->m00 = temp.x;
    out->m10 = temp.y;
    temp.x = 1.0f;
    temp.y = (eigenvalues[1].x - m->m00) / m->m01;
    fr_vec2_normalize(&temp);
    out->m01 = temp.x;
    out->m11 = temp.y;
}

//-----------------------------------------------------------------------------------------------
// Comparison
//-----------------------------------------------------------------------------------------------

FR_FORCE_INLINE b8 fr_mat2_eq_exact(const mat2* left, const mat2* right) {
#if FR_SIMD == 1
    return _mm_movemask_ps(_mm_cmpeq_ps(left->simd, right->simd)) == 0x0F;
#else
    return left->m00 == right->m00 && left->m01 == right->m01 && left->m10 == right->m10 && left->m11 == right->m11;
#endif
}

FR_FORCE_INLINE b8 fr_mat2_eq(const mat2* left, const mat2* right) {
#if FR_SIMD == 1
    return fr_simd_eq(left->simd, right->simd);
#else
    return fr_equal(left->m00, right->m00) && fr_equal(left->m01, right->m01) &&
           fr_equal(left->m10, right->m10) && fr_equal(left->m11, right->m11);
#endif
}

FR_FORCE_INLINE b8 fr_mat2_neq(const mat2* left, const mat2* right) {
#if FR_SIMD == 1
    return _mm_movemask_ps(fr_simd_vneq(left->simd, right->simd)) != 0x00;
#else
    return !fr_mat2_eq(left, right);
#endif
}

FR_FORCE_INLINE b8 fr_mat2_neq_exact(const mat2* left, const mat2* right) {
#if FR_SIMD == 1
    return _mm_movemask_ps(_mm_cmpneq_ps(left->simd, right->simd)) != 0x00;
#else
    return !fr_mat2_eq_exact(left, right);
#endif
}