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

#include <xmmintrin.h>
#include "fracture/core/defines.h"

#if FR_SIMD == 1
#include <intrin.h>

#define FR_SIMD_ALIGNMENT 16
#define FR_SIGN_BIT_MASK (int)0x80000000 // 1000 0000 0000 0000 0000 0000 0000 0000
#define FR_SIGN_BIT_INV_MASK (int)0x7FFFFFFF // 0111 1111 1111 1111 1111 1111 1111 1111

#define FR_SIGN_BITf32x4 _mm_set1_ps(-0.0f)
#define FR_INV_SIGN_BITf32x4 _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF))

// Some useful macros for SIMD operations.
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

FR_FORCE_INLINE __m128 fr_simd_vhadds(__m128 vec) {
    __m128 x0;
    x0 = _mm_add_ps(vec, FR_SIMD_SHUFFLE1(vec, 0, 1, 2,
                                          3));  // x1 = sum(x0, {z, y, x, 0.0f})
    x0 = _mm_add_ps(x0, FR_SIMD_SHUFFLE1(x0, 1, 0, 0, 1));
    return x0;
    // __m128 shuf, sums;
    // shuf = FR_SIMD_SHUFFLE1(vec, 2, 3, 0, 1);
    // sums = _mm_add_ps(vec, shuf);
    // shuf = _mm_movehl_ps(shuf, sums);
    // sums = _mm_add_ss(sums, shuf);
    // return sums;
}

FR_FORCE_INLINE __m128 fr_simd_abs(__m128 vec) {
    return _mm_and_ps(vec, FR_INV_SIGN_BITf32x4);
}

FR_FORCE_INLINE __m128 fr_simd_vhmax(__m128 a) {
    __m128 x0;
    x0 = _mm_max_ps(a, FR_SIMD_SHUFFLE1(a, 1, 0, 3, 2));
    x0 = _mm_max_ps(x0, FR_SIMD_SHUFFLE1(x0, 2, 3, 0, 1));
    return x0;
}

FR_FORCE_INLINE __m128 fr_simd_vhmin(__m128 a) {
    __m128 x0;
    x0 = _mm_min_ps(a, FR_SIMD_SHUFFLE1(a, 1, 0, 3, 2));
    x0 = _mm_min_ps(x0, FR_SIMD_SHUFFLE1(x0, 2, 3, 0, 1));
    return x0;
}

FR_FORCE_INLINE __m128 fr_simd_clamp(__m128 val, f32 min, f32 max) {
    __m128 minv = _mm_set1_ps(min);
    __m128 maxv = _mm_set1_ps(max);
    return _mm_min_ps(_mm_max_ps(val, minv), maxv);
}

FR_FORCE_INLINE __m128 fr_simd_vsmoothstep(__m128 edge0, __m128 edge1, f32 x) {
    // t = fr_clamp((t - a) / (b - a), 0.0f, 1.0f);
    __m128 t = _mm_set1_ps(x);
    t = _mm_div_ps(_mm_sub_ps(t, edge0), _mm_sub_ps(edge1, edge0));
    t = fr_simd_clamp(t, 0.0f, 1.0f);
    //  t * t * (3.0f - 2.0f * t);
    __m128 t2 = _mm_mul_ps(t, _mm_set1_ps(2.0f));
    t2 = _mm_sub_ps(_mm_set1_ps(3.0f), t2);
    t = _mm_mul_ps(t, t);
    return _mm_mul_ps(t, t2);
}


#endif