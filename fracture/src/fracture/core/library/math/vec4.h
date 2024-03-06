/**
 * @file vec4.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-03-04
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include <emmintrin.h>
#include <xmmintrin.h>
#include "detail/vec4.h"
#include "detail/vec3.h"
#include "detail/vec2.h"

#include "math_constants.h"
#include "utils.h"

// ------------------------------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------------------------------

FR_FORCE_INLINE void fr_vec4(f32 x, f32 y, f32 z, f32 w, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_set_ps(w, z, y, x);
#else
    dest->x = x;
    dest->y = y;
    dest->z = z;
    dest->w = w;
#endif
}

FR_FORCE_INLINE void fr_vec4_v3s(const vec3* xyz, f32 w, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_set_ps(w, xyz->z, xyz->y, xyz->x);
#else
    dest->x = xyz->x;
    dest->y = xyz->y;
    dest->z = xyz->z;
    dest->w = w;
#endif
}

FR_FORCE_INLINE void fr_vec4_sv3(f32 x, const vec3* yzw, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_set_ps(yzw->z, yzw->y, yzw->x, x);
#else
    dest->x = x;
    dest->y = yzw->x;
    dest->z = yzw->y;
    dest->w = yzw->z;
#endif
}

FR_FORCE_INLINE void fr_vec4_v2s(const vec2* xy, f32 z, f32 w, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_set_ps(w, z, xy->y, xy->x);
#else
    dest->x = xy->x;
    dest->y = xy->y;
    dest->z = z;
    dest->w = w;
#endif
}

FR_FORCE_INLINE void fr_vec4_v2(const vec2* xy, const vec2* zw, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_set_ps(zw->y, zw->x, xy->y, xy->x);
#else
    dest->x = xy->x;
    dest->y = xy->y;
    dest->z = zw->x;
    dest->w = zw->y;
#endif
}

FR_FORCE_INLINE void fr_vec4_fill(f32 s, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_set1_ps(s);
#else
    dest->x = s;
    dest->y = s;
    dest->z = s;
    dest->w = s;
#endif
}

FR_FORCE_INLINE void fr_vec4_zero(vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_setzero_ps();
#else
    dest->x = 0.0f;
    dest->y = 0.0f;
    dest->z = 0.0f;
    dest->w = 0.0f;
#endif
}

FR_FORCE_INLINE void fr_vec4_one(vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_set1_ps(1.0f);
#else
    dest->x = 1.0f;
    dest->y = 1.0f;
    dest->z = 1.0f;
    dest->w = 1.0f;
#endif
}

FR_FORCE_INLINE void fr_vec4_arr(const f32* arr, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_loadu_ps(arr);
#else
    dest->x = arr[0];
    dest->y = arr[1];
    dest->z = arr[2];
    dest->w = arr[3];
#endif
}

// ------------------------------------------------------------------------------------------------
// Utilities
// ------------------------------------------------------------------------------------------------

FR_FORCE_INLINE void fr_vec4_iveqv(const vec4* a, const vec4* b, bvec4* dest) {
#if FR_SIMD == 1
    __m128 thresh = _mm_set1_ps(FLOAT_EPSILON);
    __m128 diff = _mm_sub_ps(a->simd, b->simd);
    __m128 abs_diff = fr_simd_abs(diff);
    i32 mask = _mm_movemask_ps(_mm_cmplt_ps(abs_diff, thresh));
    dest->x = CHECK_BIT(mask, 0);
    dest->y = CHECK_BIT(mask, 1);
    dest->z = CHECK_BIT(mask, 2);
    dest->w = CHECK_BIT(mask, 3);
#else
    dest->x = fr_equal(a->x, b->x);
    dest->y = fr_equal(a->y, b->y);
    dest->z = fr_equal(a->z, b->z);
    dest->w = fr_equal(a->w, b->w);
#endif
}

FR_FORCE_INLINE void fr_vec4_veqv(const vec4* a, const vec4* b, vec4* dest) {
#if FR_SIMD == 1 
    __m128 thresh = _mm_set1_ps(FLOAT_EPSILON);
    __m128 diff = _mm_sub_ps(a->simd, b->simd);
    __m128 abs_diff = fr_simd_abs(diff);
    __m128 mask = _mm_cmplt_ps(abs_diff, thresh);
    dest->simd = _mm_and_ps(mask, _mm_set1_ps(1.0f));
#else
    dest->x = fr_equal(a->x, b->x);
    dest->y = fr_equal(a->y, b->y);
    dest->z = fr_equal(a->z, b->z);
    dest->w = fr_equal(a->w, b->w);
#endif
}


FR_FORCE_INLINE void fr_vec4_vneqv(const vec4* a, const vec4* b, vec4* dest) {
#if FR_SIMD == 1
    __m128 thresh = _mm_set1_ps(FLOAT_EPSILON);
    __m128 diff = _mm_sub_ps(a->simd, b->simd);
    __m128 abs_diff = fr_simd_abs(diff);
    __m128 mask = _mm_cmpgt_ps(abs_diff, thresh);
    dest->simd = _mm_and_ps(mask, _mm_set1_ps(1.0f));
#else
    dest->x = (f32)(a->x != b->x);
    dest->y = (f32)(a->y != b->y);
    dest->z = (f32)(a->z != b->z);
    dest->w = (f32)(a->w != b->w);
#endif
}
FR_FORCE_INLINE void fr_vec4_ineqv(const vec4* a, const vec4* b, bvec4* dest) {
#if FR_SIMD == 1
    __m128 thresh = _mm_set1_ps(FLOAT_EPSILON);
    __m128 diff = _mm_sub_ps(a->simd, b->simd);
    __m128 abs_diff = fr_simd_abs(diff);
    i32 mask = _mm_movemask_ps(_mm_cmpgt_ps(abs_diff, thresh));
    dest->x = CHECK_BIT(mask, 0);
    dest->y = CHECK_BIT(mask, 1);
    dest->z = CHECK_BIT(mask, 2);
    dest->w = CHECK_BIT(mask, 3);
#else
    dest->x = (f32)(a->x != b->x);
    dest->y = (f32)(a->y != b->y);
    dest->z = (f32)(a->z != b->z);
    dest->w = (f32)(a->w != b->w);
#endif
}

FR_FORCE_INLINE b8 fr_vec4_eqv(const vec4* a, const vec4* b) {
#if FR_SIMD == 1 // In O0 optimization non simd is faster than simd
    __m128 thresh = _mm_set1_ps(FLOAT_EPSILON);
    __m128 diff = _mm_sub_ps(a->simd, b->simd);
    __m128 abs_diff = fr_simd_abs(diff);
    __m128 mask = _mm_cmplt_ps(abs_diff, thresh);
    return _mm_movemask_ps(mask) == 0x0F;
#else
    return fr_equal(a->x, b->x) && fr_equal(a->y, b->y) &&
           fr_equal(a->z, b->z) && fr_equal(a->w, b->w);
#endif
}

FR_FORCE_INLINE b8 fr_vec4_eqv_exact(const vec4* a, const vec4* b) {
#if FR_SIMD == 1
    return _mm_movemask_ps(_mm_cmpeq_ps(a->simd, b->simd)) == 0x0F;
#else
    return a->x == b->x && a->y == b->y && a->z == b->z && a->w == b->w;
#endif
}

FR_FORCE_INLINE b8 fr_vec4_neqv(const vec4* a, const vec4* b) {
#if FR_SIMD == 1 // In O0 optimization non simd is faster than simd
    __m128 thresh = _mm_set1_ps(FLOAT_EPSILON);
    __m128 diff = _mm_sub_ps(a->simd, b->simd);
    __m128 abs_diff = fr_simd_abs(diff);
    __m128 mask = _mm_cmpgt_ps(abs_diff, thresh);
    return _mm_movemask_ps(mask) == 0x0F;
#else
    return !fr_equal(a->x, b->x) || !fr_equal(a->y, b->y) ||
           !fr_equal(a->z, b->z) || !fr_equal(a->w, b->w);
#endif
}

FR_FORCE_INLINE b8 fr_vec4_neqv_exact(const vec4* a, const vec4* b) {
#if FR_SIMD == 1
    return _mm_movemask_ps(_mm_cmpneq_ps(a->simd, b->simd)) != 0x0F;
#else
    return a->x != b->x || a->y != b->y || a->z != b->z || a->w != b->w;
#endif
}

FR_FORCE_INLINE void fr_vec4_iveqs(const vec4* a, f32 s, bvec4* dest) {
#if FR_SIMD == 1
    __m128 thresh = _mm_set1_ps(FLOAT_EPSILON);
    __m128 diff = _mm_sub_ps(a->simd, _mm_set1_ps(s));
    __m128 abs_diff = fr_simd_abs(diff);
    i32 mask = _mm_movemask_ps(_mm_cmplt_ps(abs_diff, thresh));
    dest->x = CHECK_BIT(mask, 0);
    dest->y = CHECK_BIT(mask, 1);
    dest->z = CHECK_BIT(mask, 2);
    dest->w = CHECK_BIT(mask, 3);
#else
    dest->x = fr_equal(a->x, s);
    dest->y = fr_equal(a->y, s);
    dest->z = fr_equal(a->z, s);
    dest->w = fr_equal(a->w, s);
#endif
}

FR_FORCE_INLINE void fr_vec4_veqs(const vec4* a, f32 s, vec4* dest) {
#if FR_SIMD == 1 // Simd is about the same speed as non simd
    __m128 thresh = _mm_set1_ps(FLOAT_EPSILON);
    __m128 diff = _mm_sub_ps(a->simd, _mm_set1_ps(s));
    __m128 abs_diff = fr_simd_abs(diff);
    __m128 mask = _mm_cmplt_ps(abs_diff, thresh);
    dest->simd = _mm_and_ps(mask, _mm_set1_ps(1.0f));
#else
    dest->x = fr_equal(a->x, s);
    dest->y = fr_equal(a->y, s);
    dest->z = fr_equal(a->z, s);
    dest->w = fr_equal(a->w, s);
#endif
}

FR_FORCE_INLINE void fr_vec4_vneqs(const vec4* a, f32 s, vec4* dest) {
#if FR_SIMD == 1 // Simd is about the same speed as non simd
    __m128 thresh = _mm_set1_ps(FLOAT_EPSILON);
    __m128 diff = _mm_sub_ps(a->simd, _mm_set1_ps(s));
    __m128 abs_diff = fr_simd_abs(diff);
    __m128 mask = _mm_cmpgt_ps(abs_diff, thresh);
    dest->simd = _mm_and_ps(mask, _mm_set1_ps(1.0f));
#else
    dest->x = (f32)(a->x != s);
    dest->y = (f32)(a->y != s);
    dest->z = (f32)(a->z != s);
    dest->w = (f32)(a->w != s);
#endif
}

FR_FORCE_INLINE b8 fr_vec4_eqs(const vec4* a, f32 s) {
#if FR_SIMD == 1 // Simd is about the same speed as non simd
    __m128 thresh = _mm_set1_ps(FLOAT_EPSILON);
    __m128 diff = _mm_sub_ps(a->simd, _mm_set1_ps(s));
    __m128 abs_diff = fr_simd_abs(diff);
    __m128 mask = _mm_cmplt_ps(abs_diff, thresh);
    return _mm_movemask_ps(mask) == 0x0F;
#else
    return fr_equal(a->x, s) && fr_equal(a->y, s) &&
           fr_equal(a->z, s) && fr_equal(a->w, s);
#endif
}

FR_FORCE_INLINE b8 fr_vec4_neqs(const vec4* a, f32 s) {
#if FR_SIMD == 1 // Simd is about the same speed as non simd
    __m128 thresh = _mm_set1_ps(FLOAT_EPSILON);
    __m128 diff = _mm_sub_ps(a->simd, _mm_set1_ps(s));
    __m128 abs_diff = fr_simd_abs(diff);
    __m128 mask = _mm_cmpgt_ps(abs_diff, thresh);
    return _mm_movemask_ps(mask) == 0x0F;
#else
    return !fr_equal(a->x, s) || !fr_equal(a->y, s) ||
           !fr_equal(a->z, s) || !fr_equal(a->w, s);
#endif
}

FR_FORCE_INLINE b8 fr_vec4_eqs_exact(const vec4* a, f32 s) {
#if FR_SIMD == 1
    return _mm_movemask_ps(_mm_cmpeq_ps(a->simd, _mm_set1_ps(s))) == 0x0F;
#else
    return a->x == s && a->y == s && a->z == s && a->w == s;
#endif
}

FR_FORCE_INLINE b8 fr_vec4_neqs_exact(const vec4* a, f32 s) {
#if FR_SIMD == 1
    return _mm_movemask_ps(_mm_cmpneq_ps(a->simd, _mm_set1_ps(s))) != 0x0F;
#else
    return a->x != s || a->y != s || a->z != s || a->w != s;
#endif
}

FR_FORCE_INLINE void fr_vec4_vgtv(const vec4* a, const vec4* b, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_and_ps(_mm_cmpgt_ps(a->simd, b->simd), _mm_set1_ps(1.0f));
#else
    dest->x = (f32)(a->x > b->x);
    dest->y = (f32)(a->y > b->y);
    dest->z = (f32)(a->z > b->z);
    dest->w = (f32)(a->w > b->w);
#endif
}

FR_FORCE_INLINE void fr_vec4_vltv(const vec4* a, const vec4* b, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_and_ps(_mm_cmplt_ps(a->simd, b->simd), _mm_set1_ps(1.0f));
#else
    dest->x = (f32)(a->x < b->x);
    dest->y = (f32)(a->y < b->y);
    dest->z = (f32)(a->z < b->z);
    dest->w = (f32)(a->w < b->w);
#endif
}

FR_FORCE_INLINE void fr_vec4_vgev(const vec4* a, const vec4* b, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_and_ps(_mm_cmpge_ps(a->simd, b->simd), _mm_set1_ps(1.0f));
#else
    dest->x = (f32)(a->x >= b->x);
    dest->y = (f32)(a->y >= b->y);
    dest->z = (f32)(a->z >= b->z);
    dest->w = (f32)(a->w >= b->w);
#endif
}

FR_FORCE_INLINE void fr_vec4_vlev(const vec4* a, const vec4* b, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_and_ps(_mm_cmple_ps(a->simd, b->simd), _mm_set1_ps(1.0f));
#else
    dest->x = (f32)(a->x <= b->x);
    dest->y = (f32)(a->y <= b->y);
    dest->z = (f32)(a->z <= b->z);
    dest->w = (f32)(a->w <= b->w);
#endif
}

FR_FORCE_INLINE void fr_vec4_vgts(const vec4* a, f32 s, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_and_ps(_mm_cmpgt_ps(a->simd, _mm_set1_ps(s)), _mm_set1_ps(1.0f));
#else
    dest->x = (f32)(a->x > s);
    dest->y = (f32)(a->y > s);
    dest->z = (f32)(a->z > s);
    dest->w = (f32)(a->w > s);
#endif
}

FR_FORCE_INLINE void fr_vec4_vlts(const vec4* a, f32 s, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_and_ps(_mm_cmplt_ps(a->simd, _mm_set1_ps(s)), _mm_set1_ps(1.0f));
#else
    dest->x = (f32)(a->x < s);
    dest->y = (f32)(a->y < s);
    dest->z = (f32)(a->z < s);
    dest->w = (f32)(a->w < s);
#endif
}

FR_FORCE_INLINE void fr_vec4_vges(const vec4* a, f32 s, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_and_ps(_mm_cmpge_ps(a->simd, _mm_set1_ps(s)), _mm_set1_ps(1.0f));
#else
    dest->x = (f32)(a->x >= s);
    dest->y = (f32)(a->y >= s);
    dest->z = (f32)(a->z >= s);
    dest->w = (f32)(a->w >= s);
#endif
}

FR_FORCE_INLINE void fr_vec4_vles(const vec4* a, f32 s, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_and_ps(_mm_cmple_ps(a->simd, _mm_set1_ps(s)), _mm_set1_ps(1.0f));
#else
    dest->x = (f32)(a->x <= s);
    dest->y = (f32)(a->y <= s);
    dest->z = (f32)(a->z <= s);
    dest->w = (f32)(a->w <= s);
#endif
}

FR_FORCE_INLINE b8 fr_vec4_iszero(const vec4* a) {
#if FR_SIMD == 1
    return _mm_movemask_ps(_mm_cmpeq_ps(a->simd, _mm_setzero_ps())) == 0x0F;
#else
    return fr_equal(a->x, 0.0f) && fr_equal(a->y, 0.0f) &&
           fr_equal(a->z, 0.0f) && fr_equal(a->w, 0.0f);
#endif
}

// ------------------------------------------------------------------------------------------------
// Arithmetic
// ------------------------------------------------------------------------------------------------

FR_FORCE_INLINE f32 fr_vec4_dot(const vec4* a, const vec4* b) {
#if FR_SIMD == 1 // In O0 optimization non simd is faster than simd
   return _mm_cvtss_f32(fr_simd_vdot(a->simd, b->simd));
#else
    return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
#endif
}

FR_FORCE_INLINE f32 fr_vec4_norm(const vec4* a) {
#if 1 // non simd is faster if you dont have O2 optimization
    return _mm_cvtss_f32(fr_simd_vnorm(a->simd));
#else
    return fr_sqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
#endif
}

FR_FORCE_INLINE f32 fr_vec4_invnorm(const vec4* a) {
#if 1 // non simd is faster if you dont have O2 optimization
    return _mm_cvtss_f32(fr_simd_vinvnorm(a->simd));
#else
    return fr_sqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
#endif
}

FR_FORCE_INLINE f32 fr_vec4_norm2(const vec4* a) {
#if 1 // non simd is faster if you dont have O2 optimization
    return _mm_cvtss_f32(fr_simd_vnorm2(a->simd));
#else
    return a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w;
#endif
}

FR_FORCE_INLINE f32 fr_vec4_norm1(const vec4* a) {
#if 1 // non simd is faster if you dont have O2 optimization
    return _mm_cvtss_f32(fr_simd_vnorm1(a->simd));
#else
    return fr_abs(a->x) + fr_abs(a->y) + fr_abs(a->z) + fr_abs(a->w);
#endif
}

FR_FORCE_INLINE f32 fr_vec4_norm_inf(const vec4* a) {
#if 1 // non simd is faster if you dont have O2 optimization
    return _mm_cvtss_f32(fr_simd_vnorm_inf(a->simd));
#else
    return fr_max(fr_max(fr_abs(a->x), fr_abs(a->y)), fr_max(fr_abs(a->z), fr_abs(a->w));
#endif
}

FR_FORCE_INLINE void fr_vec4_normalize(const vec4* a, vec4* dest) {
#if FR_SIMD == 1
    if (fr_vec4_iszero(a)) {
        dest->simd = _mm_setzero_ps();
        return;
    }
    __m128 inv_norm = fr_simd_vinvnorm(a->simd);
    dest->simd = _mm_mul_ps(a->simd, inv_norm);
#else
    f32 norm = fr_vec4_norm(a);
    if (norm == 0.0f) {
        dest->x = 0.0f;
        dest->y = 0.0f;
        dest->z = 0.0f;
        dest->w = 0.0f;
        return;
    }
    f32 inv_norm = 1.0f / norm;
    dest->x = a->x * inv_norm;
    dest->y = a->y * inv_norm;
    dest->z = a->z * inv_norm;
    dest->w = a->w * inv_norm;
#endif
}

FR_FORCE_INLINE void fr_vec4_normalize_unsafe(const vec4* a, vec4* dest) {
#if FR_SIMD == 1
    __m128 inv_norm = fr_simd_vinvnorm(a->simd);
    dest->simd = _mm_mul_ps(a->simd, inv_norm);
#else
    f32 inv_norm = 1.0f / fr_vec4_norm(a);
    dest->x = a->x * inv_norm;
    dest->y = a->y * inv_norm;
    dest->z = a->z * inv_norm;
    dest->w = a->w * inv_norm;
#endif
}

FR_FORCE_INLINE void fr_vec4_abs(const vec4* a, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = fr_simd_abs(a->simd);
#else
    dest->x = fr_abs(a->x);
    dest->y = fr_abs(a->y);
    dest->z = fr_abs(a->z);
    dest->w = fr_abs(a->w);
#endif
}

FR_FORCE_INLINE void fr_vec4_negate(const vec4* a, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_xor_ps(a->simd, FR_SIGN_BITf32x4);
#else
    dest->x = -a->x;
    dest->y = -a->y;
    dest->z = -a->z;
    dest->w = -a->w;
#endif
}

FR_FORCE_INLINE void fr_vec4_add(const vec4* a, const vec4* b, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_add_ps(a->simd, b->simd);
#else
    dest->x = a->x + b->x;
    dest->y = a->y + b->y;
    dest->z = a->z + b->z;
    dest->w = a->w + b->w;
#endif
}

FR_FORCE_INLINE void fr_vec4_adds(const vec4* a, f32 s, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_add_ps(a->simd, _mm_set1_ps(s));
#else
    dest->x = a->x + s;
    dest->y = a->y + s;
    dest->z = a->z + s;
    dest->w = a->w + s;
#endif
}

FR_FORCE_INLINE void fr_vec4_sub(const vec4* a, const vec4* b, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_sub_ps(a->simd, b->simd);
#else
    dest->x = a->x - b->x;
    dest->y = a->y - b->y;
    dest->z = a->z - b->z;
    dest->w = a->w - b->w;
#endif
}

FR_FORCE_INLINE void fr_vec4_subs(const vec4* a, f32 s, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_sub_ps(a->simd, _mm_set1_ps(s));
#else
    dest->x = a->x - s;
    dest->y = a->y - s;
    dest->z = a->z - s;
    dest->w = a->w - s;
#endif
}

FR_FORCE_INLINE void fr_vec4_mul(const vec4* a, const vec4* b, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_mul_ps(a->simd, b->simd);
#else
    dest->x = a->x * b->x;
    dest->y = a->y * b->y;
    dest->z = a->z * b->z;
    dest->w = a->w * b->w;
#endif
}

