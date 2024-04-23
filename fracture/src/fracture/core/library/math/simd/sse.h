/**
 * @file sse.h
 * @author Aditya Rajagopal
 * @brief
 * @version 0.0.1
 * @date 2024-03-03
 *
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 *
 */
#pragma once

#include "fracture/core/defines.h"

#if FR_SIMD == 1
#include <emmintrin.h>
#include <intrin.h>
#include <xmmintrin.h>

#include "fracture/core/library/math/math_constants.h"

#define FR_SIMD_ALIGNMENT 16
#define FR_SIGN_BIT_MASK (int)0x80000000      // 1000 0000 0000 0000 0000 0000 0000 0000
#define FR_SIGN_BIT_INV_MASK (int)0x7FFFFFFF  // 0111 1111 1111 1111 1111 1111 1111 1111

#define FR_SIGN_BITf32x4 _mm_set1_ps(-0.0f)
#define FR_INV_SIGN_BITf32x4 _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF))

// signbit 0 means positive, 1 means negative. We can xor the sign bit with 0x80000000 to flip the sign bit.
// xor with 0 to keep the sign bit the same.
#define FR_SIGNMASK_NPNPf32x4 _mm_castsi128_ps(_mm_set_epi32((int)0x80000000, 0, (int)0x80000000, 0))
#define FR_SIGNMASK_PNPNf32x4 _mm_castsi128_ps(_mm_set_epi32(0, (int)0x80000000, 0, (int)0x80000000))
#define FR_SIGNMASK_NEGf32x4 _mm_castsi128_ps(_mm_set1_epi32((int)0x80000000))

// Some useful macros for SIMD operations.
#define FR_SIMD_SHUFFLE(a, b, z, y, x, w) _mm_shuffle_ps(a, b, _MM_SHUFFLE(z, y, x, w))
#define FR_SIMD_SHUFFLE1(a, z, y, x, w) _mm_shuffle_ps(a, a, _MM_SHUFFLE(z, y, x, w))
#define FR_SIMD_SPLAT(mm, i) _mm_shuffle_ps(mm, mm, _MM_SHUFFLE(i, i, i, i))
#define FR_SIMD_SPLAT_X(mm) FR_SIMD_SPLAT(mm, 0)
#define FR_SIMD_SPLAT_Y(mm) FR_SIMD_SPLAT(mm, 1)
#define FR_SIMD_SPLAT_Z(mm) FR_SIMD_SPLAT(mm, 2)
#define FR_SIMD_SPLAT_W(mm) FR_SIMD_SPLAT(mm, 3)

#define FR_SIMD_LOAD(ptr) _mm_load_ps(ptr)
#define FR_SIMD_STORE(ptr, mm) _mm_store_ps(ptr, mm)
#define FR_SIMD_LOADU(ptr) _mm_loadu_ps(ptr)
#define FR_SIMD_STOREU(ptr, mm) _mm_storeu_ps(ptr, mm)

FR_FORCE_INLINE __m128 fr_simd_vhadd(__m128 vec) {
    __m128 x0;
    x0 = _mm_add_ps(vec, FR_SIMD_SHUFFLE1(vec, 0, 1, 2, 3));
    x0 = _mm_add_ps(x0, FR_SIMD_SHUFFLE1(x0, 1, 0, 0, 1));
    return x0;
    // __m128 shuf, sums;
    // shuf = FR_SIMD_SHUFFLE1(vec, 2, 3, 0, 1);
    // sums = _mm_add_ps(vec, shuf);
    // shuf = _mm_movehl_ps(shuf, sums);
    // sums = _mm_add_ss(sums, shuf);
    // return sums;
}

FR_FORCE_INLINE f32 fr_simd_hadd(__m128 vec) { return _mm_cvtss_f32(fr_simd_vhadd(vec)); }

FR_FORCE_INLINE __m128 fr_simd_abs(__m128 vec) { return _mm_and_ps(vec, FR_INV_SIGN_BITf32x4); }

FR_FORCE_INLINE __m128 fr_simd_vhmax(__m128 a) {
    __m128 x0;
    x0 = _mm_max_ps(a, FR_SIMD_SHUFFLE1(a, 1, 0, 3, 2));
    x0 = _mm_max_ps(x0, FR_SIMD_SHUFFLE1(x0, 2, 3, 0, 1));
    return x0;
}

FR_FORCE_INLINE f32 fr_simd_hmax(__m128 a) { return _mm_cvtss_f32(fr_simd_vhmax(a)); }

// FR_FORCE_INLINE __m128 fr_simd_vhmax_glm(__m128 a) {
//     __m128 x0, x1, x2;
//     x0 = _mm_movehl_ps(a, a); // x0 = {z, w, z, w}
//     x1 = _mm_max_ps(a, x0); // x1 = {max(x, z), max(y, w), max(z, z), max(w, w)
//     x2 = FR_SIMD_SPLAT(x1, 1); // puts the max(y, w) in all 4 components
//     return _mm_max_ps(x1, x2); // returns the max of all 4 components in the first element
// }

FR_FORCE_INLINE __m128 fr_simd_vhmin(__m128 a) {
    __m128 x0;
    x0 = _mm_min_ps(a, FR_SIMD_SHUFFLE1(a, 1, 0, 3, 2));
    x0 = _mm_min_ps(x0, FR_SIMD_SHUFFLE1(x0, 2, 3, 0, 1));
    return x0;
}

FR_FORCE_INLINE f32 fr_simd_hmin(__m128 a) { return _mm_cvtss_f32(fr_simd_vhmin(a)); }

FR_FORCE_INLINE __m128 fr_simd_clampv(__m128 val, __m128 min, __m128 max) {
    return _mm_min_ps(_mm_max_ps(val, min), max);
}

FR_FORCE_INLINE __m128 fr_simd_clamp(__m128 val, f32 min, f32 max) {
    __m128 minv = _mm_set1_ps(min);
    __m128 maxv = _mm_set1_ps(max);
    return fr_simd_clampv(val, minv, maxv);
}

FR_FORCE_INLINE __m128 fr_simd_step(__m128 edge, __m128 val) {
    return _mm_and_ps(_mm_cmpge_ps(val, edge), _mm_set1_ps(1.0f));
}

FR_FORCE_INLINE __m128 fr_simd_lerp(__m128 a, __m128 b, f32 t) {
    __m128 t0 = _mm_set1_ps(t);
    __m128 t1 = _mm_set1_ps(1.0f - t);
    return _mm_add_ps(_mm_mul_ps(a, t1), _mm_mul_ps(b, t0));
}

FR_FORCE_INLINE __m128 fr_simd_vsmoothstep(__m128 edge0, __m128 edge1, __m128 x) {
    // t = fr_clamp((t - a) / (b - a), 0.0f, 1.0f);
    __m128 t = _mm_div_ps(_mm_sub_ps(x, edge0), _mm_sub_ps(edge1, edge0));
    t = fr_simd_clamp(t, 0.0f, 1.0f);
    //  t * t * (3.0f - 2.0f * t);
    return _mm_mul_ps(_mm_mul_ps(t, t), _mm_sub_ps(_mm_set1_ps(3.0f), _mm_mul_ps(_mm_set1_ps(2.0f), t)));
}

FR_FORCE_INLINE __m128 fr_simd_vdot(__m128 a, __m128 b) {
    __m128 x0 = _mm_mul_ps(a, b);
    return fr_simd_vhadd(x0);
}

FR_FORCE_INLINE __m128 fr_simd_vnorm2(__m128 a) { return fr_simd_vdot(a, a); }

FR_FORCE_INLINE __m128 fr_simd_vnorm(__m128 a) { return _mm_sqrt_ps(fr_simd_vdot(a, a)); }

FR_FORCE_INLINE __m128 fr_simd_vinvnorm_fast(__m128 a) { return _mm_rsqrt_ps(fr_simd_vdot(a, a)); }

FR_FORCE_INLINE __m128 fr_simd_vinvnorm(__m128 a) { return _mm_div_ps(_mm_set1_ps(1.0), fr_simd_vdot(a, a)); }

FR_FORCE_INLINE __m128 fr_simd_vnorm1(__m128 a) { return fr_simd_vhadd(fr_simd_abs(a)); }

FR_FORCE_INLINE __m128 fr_simd_vnorm_inf(__m128 a) { return fr_simd_vhmax(fr_simd_abs(a)); }

FR_FORCE_INLINE __m128 fr_simd_unit_vector_unsafe(__m128 a) {
    __m128 inv_norm = fr_simd_vinvnorm(a);
    return _mm_mul_ps(a, inv_norm);
}

FR_FORCE_INLINE __m128 fr_simd_unit_vector(__m128 a) {
    __m128 zero = _mm_setzero_ps();
    if (_mm_movemask_ps(_mm_cmpeq_ps(a, zero)) == 0x0F) {
        return zero;
    }
    return fr_simd_unit_vector_unsafe(a);
}

FR_FORCE_INLINE __m128 fr_simd_reflect(__m128 unit_normal, __m128 incident) {
    // r = i - 2 * dot(n, i) * n
    __m128 dot = fr_simd_vdot(unit_normal, incident);
    __m128 x0 = _mm_mul_ps(unit_normal, _mm_mul_ps(dot, _mm_set1_ps(2.0f)));
    return _mm_sub_ps(incident, x0);
}

FR_FORCE_INLINE __m128 fr_simd_veq(__m128 a, __m128 b) {
    __m128 threshold = _mm_set1_ps(FLOAT_EPSILON);
    __m128 abs_diff = fr_simd_abs(_mm_sub_ps(a, b));
    return _mm_cmplt_ps(abs_diff, threshold);
}

FR_FORCE_INLINE b8 fr_simd_eq(__m128 a, __m128 b) { return _mm_movemask_ps(fr_simd_veq(a, b)) == 0x0F; }

FR_FORCE_INLINE __m128 fr_simd_vneq(__m128 a, __m128 b) {
    __m128 threshold = _mm_set1_ps(FLOAT_EPSILON);
    __m128 abs_diff = fr_simd_abs(_mm_sub_ps(a, b));
    return _mm_cmpge_ps(abs_diff, threshold);
}

FR_FORCE_INLINE __m128 fr_simd_fmadd(__m128 a, __m128 b, __m128 c) { return _mm_add_ps(_mm_mul_ps(a, b), c); }

FR_FORCE_INLINE __m128 fr_simd_fnmadd(__m128 a, __m128 b, __m128 c) { return _mm_sub_ps(c, _mm_mul_ps(a, b)); }

FR_FORCE_INLINE __m128 fr_simd_sign01(__m128 a) {
    __m128 sign = _mm_and_ps(FR_SIGN_BITf32x4, a);
    return _mm_cmpeq_ps(sign, _mm_setzero_ps());
}

FR_FORCE_INLINE __m128 fr_simd_sign(__m128 a) {
    __m128 x0, x1, x2, x3;
    x0 = _mm_set_ps(0.0f, 0.0f, 1.0f, -1.0f);
    x1 = FR_SIMD_SPLAT_Z(x0);

    x2 = _mm_and_ps(_mm_cmpgt_ps(a, x1), FR_SIMD_SPLAT_Y(x0));
    x3 = _mm_and_ps(_mm_cmplt_ps(a, x1), FR_SIMD_SPLAT_X(x0));
    return _mm_or_ps(x2, x3);
}

#endif
