/**
 * @file vec3.h
 * @author Aditya Rajagopal
 * @brief
 * @version 0.0.1
 * @date 2024-03-02
 *
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 *
 */
#pragma once

#include "detail/vec3.h"
#include "fracture/core/library/random/fr_random.h"
#include "simd/sse.h"
#include "utils.h"

// ---------------------------------------------------------
// ------------------- Vec3 Constructors -------------------
// ---------------------------------------------------------

/**
 * @brief Construct a new vec3 object from an array of floats.
 * @details The array must have at least 3 elements and if you give the same
 * pointer for src and dst, the function will have undefined behavior.
 *
 * @param data The source array of floats.
 * @param dest The destination vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_create(const f32* restrict data, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_setr_ps(data[0], data[1], data[2], 0.0f);
#else
    dest->x = data[0];
    dest->y = data[1];
    dest->z = data[2];
#endif
}

/**
 * @brief Convert a vec3 object to an array of floats.
 * @details The array must have at least 3 elements and if you give the same
 * pointer for src and dst, the function will have undefined behavior.
 *
 * @param src The source vec3 object.
 * @param dest The destination array of floats.
 */
FR_FORCE_INLINE void fr_vec3_to_array(const vec3* restrict data, f32* dest) {
    dest[0] = data->x;
    dest[1] = data->y;
    dest[2] = data->z;
}

/**
 * @brief Construct a new vec3 object from two float values.
 *
 * @param x The x component of the vec3 object.
 * @param y The y component of the vec3 object.
 * @param z The z component of the vec3 object.
 * @param dest The destination vec3 object.
 */
FR_FORCE_INLINE void fr_vec3(f32 x, f32 y, f32 z, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_setr_ps(x, y, z, 0.0f);
#else
    dest->x = x;
    dest->y = y;
    dest->z = z;
#endif
}

/**
 * @brief Construct a new vec3 object from a single float value.
 *
 * @param val The value to set the x and y components of the vec3 object to.
 * @return vec3 The constructed vec3 object.
 */
FR_FORCE_INLINE vec3 fr_vec3s(f32 val, vec3* dest) {
#if FR_VEC3_SIMD == 1
    return (vec3){.simd = _mm_setr_ps(val, val, val, 0.0f)};
#else
    return (vec3){.x = val, .y = val, .z = val};
#endif
}

/**
 * @brief Construct a new vec3 object from a vec3 object.
 * @details If you give the same pointer for src and dest, the function will have undefined behavior.
 * @param src The source vec3 object.
 * @return vec3 The constructed vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_copy(const vec3* restrict src, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = src->simd;
#else
    dest->x = src->x;
    dest->y = src->y;
    dest->z = src->z;
#endif
}

/**
 * @brief Get a vec3 object with all components set to zero.
 *
 * @return vec3
 */
FR_FORCE_INLINE void fr_vec3_zeros(vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_setzero_ps();
#else
    dest->x = 0.0f;
    dest->y = 0.0f;
    dest->z = 0.0f;
#endif
}

/**
 * @brief Set all components of a vec3 object to one.
 *
 * @param dest The destination vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_ones(vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_setr_ps(1.0f, 1.0f, 1.0f, 0.0f);
#else
    dest->x = 1.0f;
    dest->y = 1.0f;
    dest->z = 1.0f;
#endif
}

/**
 * @brief Set the components of a vec3 object to (-1, 0, 0)
 *
 * @param dest The destination vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_left(vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_setr_ps(-1.0f, 0.0f, 0.0f, 0.0f);
#else
    dest->x = -1.0f;
    dest->y = 0.0f;
    dest->z = 0.0f;
#endif
}

/**
 * @brief Set the components of a vec3 object to (1, 0, 0).
 *
 * @param dest The destination vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_right(vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f);
#else
    dest->x = 1.0f;
    dest->y = 0.0f;
    dest->z = 0.0f;
#endif
}

/**
 * @brief Set the components of a vec3 object to  (0, 1, 0).
 *
 * @param dest The destination vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_up(vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f);
#else
    dest->x = 0.0f;
    dest->y = 1.0f;
    dest->z = 0.0f;
#endif
}

/**
 * @brief Set the components of a vec3 object to (0, -1, 0).
 *
 * @param dest The destination vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_down(vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_setr_ps(0.0f, -1.0f, 0.0f, 0.0f);
#else
    dest->x = 0.0f;
    dest->y = -1.0f;
    dest->z = 0.0f;
#endif
}

/**
 * @brief Set the components of a vec3 object to (0, 0, 1).
 *
 * @param dest The destination vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_forward(vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f);
#else
    dest->x = 0.0f;
    dest->y = 0.0f;
    dest->z = 1.0f;
#endif
}

/**
 * @brief Set the components of a vec3 object to (0, 0, -1).
 *
 * @param dest The destination vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_backward(vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_setr_ps(0.0f, 0.0f, -1.0f, 0.0f);
#else
    dest->x = 0.0f;
    dest->y = 0.0f;
    dest->z = -1.0f;
#endif
}

/**
 * @brief Set all components of a vec3 object to a single float value
 *
 * @param val The value to set the components to.
 * @param dest The destination vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_fill(f32 val, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_setr_ps(val, val, val, 0.0f);
#else
    dest->x = val;
    dest->y = val;
    dest->z = val;
#endif
}

/**
 * @brief Set the components of a vec3 object to random values between 0 and 1.
 * @details The random number generator state must be initialized before calling this function.
 * @param dest The destination vec3 object.
 * @param state The random number generator state.
 */
FR_FORCE_INLINE void fr_vec3_random_uniform(vec3* dest, void* state) {
    dest->x = fr_random_uniform(state);
    dest->y = fr_random_uniform(state);
    dest->z = fr_random_uniform(state);
}

/**
 * @brief Set the components of a vec3 object to random values between min and max.
 * @details The random number generator state must be initialized before calling this function.
 * @param dest The destination vec3 object.
 * @param state The random number generator state.
 * @param min The minimum value for the random numbers.
 * @param max The maximum value for the random numbers.
 */
FR_FORCE_INLINE void fr_vec3_random_uniform_range(vec3* dest, void* state, f32 min, f32 max) {
    dest->x = fr_random_uniform_range(state, min, max);
    dest->y = fr_random_uniform_range(state, min, max);
    dest->z = fr_random_uniform_range(state, min, max);
}

// ---------------------------------------------
// -------------- vec3 utilities ---------------
// ---------------------------------------------

/**
 * @brief Check if all components of a vec3 object are equal to a single float
 * value.
 *
 * @param v The vec3 object to check.
 * @param val The value to check against.
 * @return b8 True if all components are equal to the value, false otherwise.
 */
FR_FORCE_INLINE b8 fr_vec3_eqs(const vec3* v, f32 val) {
#if FR_VEC3_SIMD == 1
    __m128 thresh = _mm_set1_ps(FLOAT_EPSILON);
    __m128 diff = _mm_sub_ps(v->simd, _mm_set_ps(val, val, val, 0.0f));
    __m128 abs_diff = fr_simd_abs(diff);
    __m128 cmp = _mm_cmplt_ps(abs_diff, thresh);
    return _mm_movemask_ps(cmp) == 0x0F;
#else
    return fr_equal(v->x, val) && fr_equal(v->y, val) && fr_equal(v->z, val);
#endif
}

/**
 * @brief Check if all components of a vec3 object are equal to a single float
 * value within a certain threshold.
 *
 * @param v The vec3 object to check.
 * @param val The value to check against.
 * @param threshold The threshold to use for the comparison.
 * @return b8 True if all components are equal to the value within the
 * threshold, false otherwise.
 */
FR_FORCE_INLINE b8 fr_vec3_eqs_threshold(const vec3* v, f32 val, f32 threshold) {
#if FR_VEC3_SIMD == 1
    __m128 diff = _mm_sub_ps(v->simd, _mm_set_ps(val, val, val, 0.0f));
    __m128 abs_diff = fr_simd_abs(diff);
    __m128 cmp = _mm_cmplt_ps(abs_diff, _mm_set1_ps(threshold));
    return _mm_movemask_ps(cmp) == 0x0F;
#else
    return fr_equal_threshold(v->x, val, threshold) && fr_equal_threshold(v->y, val, threshold) &&
           fr_equal_threshold(v->z, val, threshold);
#endif
}

/**
 * @brief Check if all components of a vec3 object are equal to each other.
 *
 * @param v The vec3 object to check.
 * @return b8 True if all components are equal to each other, false otherwise.
 */
FR_FORCE_INLINE b8 fr_vec3_eq_all(const vec3* v) { return fr_equal(v->x, v->y) && fr_equal(v->y, v->z); }

/**
 * @brief Check if two vectors are equal to each other
 *
 * @param v0
 * @param v1
 * @return b8 TRUE if the vectors are equal, FALSE otherwise.
 */
FR_FORCE_INLINE b8 fr_vec3_eq(const vec3* v0, const vec3* v1) {
#if FR_VEC3_SIMD == 1
    __m128 thresh = _mm_set1_ps(FLOAT_EPSILON);
    __m128 diff = _mm_sub_ps(v0->simd, v1->simd);
    __m128 abs_diff = fr_simd_abs(diff);
    __m128 cmp = _mm_cmplt_ps(abs_diff, thresh);
    return _mm_movemask_ps(cmp) == 0x0F;
#else
    return fr_equal(v0->x, v1->x) && fr_equal(v0->y, v1->y) && fr_equal(v0->z, v1->z);
#endif
}

FR_FORCE_INLINE void fr_vec3_veq(const vec3* v0, const vec3* v1, vec3* dest) {
#if FR_VEC3_SIMD == 1
    __m128 thresh = _mm_set1_ps(FLOAT_EPSILON);
    __m128 diff = _mm_sub_ps(v0->simd, v1->simd);
    __m128 abs_diff = fr_simd_abs(diff);
    __m128 cmp = _mm_cmplt_ps(abs_diff, thresh);
    dest->simd = _mm_and_ps(cmp, _mm_set1_ps(1.0f));
#else
    dest->x = fr_equal(v0->x, v1->x);
    dest->y = fr_equal(v0->y, v1->y);
    dest->z = fr_equal(v0->z, v1->z);
#endif
}

FR_FORCE_INLINE void fr_vec3_veqs(const vec3* v0, f32 val, vec3* dest) {
#if FR_VEC3_SIMD == 1
    __m128 thresh = _mm_set1_ps(FLOAT_EPSILON);
    __m128 diff = _mm_sub_ps(v0->simd, _mm_set_ps(val, val, val, 0.0f));
    __m128 abs_diff = fr_simd_abs(diff);
    __m128 cmp = _mm_cmplt_ps(abs_diff, thresh);
    dest->simd = _mm_and_ps(cmp, _mm_set1_ps(1.0f));
#else
    dest->x = fr_equal(v0->x, val);
    dest->y = fr_equal(v0->y, val);
    dest->z = fr_equal(v0->z, val);
#endif
}

/**
 * @brief Check if two vectors are equal to each other within a certain
 * threshold
 *
 * @param v0
 * @param v1
 * @param threshold The threshold to use for the comparison.
 * @return b8 TRUE if the vectors are equal within the threshold, FALSE
 * otherwise.
 */
FR_FORCE_INLINE b8 fr_vec3_eq_threshold(const vec3* v0, const vec3* v1, f32 threshold) {
#if FR_VEC3_SIMD == 1
    __m128 diff = _mm_sub_ps(v0->simd, v1->simd);
    __m128 abs_diff = fr_simd_abs(diff);
    __m128 cmp = _mm_cmplt_ps(abs_diff, _mm_set1_ps(threshold));
    return _mm_movemask_ps(cmp) == 0x0F;
#else
    return fr_equal_threshold(v0->x, v1->x, threshold) && fr_equal_threshold(v0->y, v1->y, threshold) &&
           fr_equal_threshold(v0->z, v1->z, threshold);
#endif
}

/**
 * @brief Get the maximum component of a vec3 object.
 *
 * @param v The vec3 object to check.
 * @return f32 The maximum component of the vec3 object.
 */
FR_FORCE_INLINE f32 fr_vec3_max(const vec3* v) {
#if FR_VEC3_SIMD == 1
    return _mm_cvtss_f32(fr_simd_vhmax(v->simd));
#else
    f32 max = v->x;
    if (v->y > max) {
        max = v->y;
    }
    if (v->z > max) {
        max = v->z;
    }
    return max;
#endif
}

/**
 * @brief Get the minimum component of a vec3 object.
 *
 * @param v The vec3 object to check.
 * @return f32 The minimum component of the vec3 object.
 */
FR_FORCE_INLINE f32 fr_vec3_min(const vec3* v) {
#if FR_VEC3_SIMD == 1
    return _mm_cvtss_f32(fr_simd_vhmin(v->simd));
#else
    f32 min = v->x;
    if (v->y < min) {
        min = v->y;
    }
    if (v->z < min) {
        min = v->z;
    }
    return min;
#endif
}

/**
 * @brief Check if all the components of a vec3 are zero
 *
 * @param v The vec3 object to check.
 * @return b8 TRUE if all components are zero, FALSE otherwise.
 */
FR_FORCE_INLINE b8 fr_vec3_iszero(const vec3* v) { return fr_vec3_eqs(v, 0.0f); }

/**
 * @brief Get a vec3 object with the sign of each component of the input vec3
 * object.
 *
 * @param v The input vec3 object.
 * @return vec3
 */
FR_FORCE_INLINE void fr_vec3_sign(const vec3* v, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = fr_simd_sign(v->simd);
#else
    dest->x = fr_sign(v->x);
    dest->y = fr_sign(v->y);
    dest->z = fr_sign(v->z);
#endif
}

/**
 * @brief Get a vec3 object with the absolute value of each component of the
 * input vec3 object.
 *
 * @param v The input vec3 object.
 * @return vec3
 */
FR_FORCE_INLINE void fr_vec3_abs(const vec3* v, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = fr_simd_abs(v->simd);
#else
    dest->x = fr_abs(v->x);
    dest->y = fr_abs(v->y);
    dest->z = fr_abs(v->z);
#endif
}

/**
 * @brief Set the components of a vec3 object to their absolute values.
 *
 * @param v The input vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_make_abs(vec3* v) {
#if FR_VEC3_SIMD == 1
    v->simd = _mm_and_ps(v->simd, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)));
#else
    v->x = fr_abs(v->x);
    v->y = fr_abs(v->y);
    v->z = fr_abs(v->z);
#endif
}

/**
 * @brief Get a vec3 object with the square root of each component of the input
 * vec3 object.
 *
 * @param v The input vec3 object.
 * @return vec3
 */
FR_FORCE_INLINE void fr_vec3_sqrt(const vec3* v, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_sqrt_ps(v->simd);
#else
    dest->x = fr_sqrt(v->x);
    dest->y = fr_sqrt(v->y);
    dest->z = fr_sqrt(v->z);
#endif
}

/**
 * @brief Set the components of a vec3 object to their square roots.
 *
 * @param v The input vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_make_sqrt(vec3* v) {
#if FR_VEC3_SIMD == 1
    v->simd = _mm_sqrt_ps(v->simd);
#else
    v->x = fr_sqrt(v->x);
    v->y = fr_sqrt(v->y);
    v->z = fr_sqrt(v->z);
#endif
}

// ---------------------------------------------
// ------------ vec3 math functions ------------
// ---------------------------------------------

/**
 * @brief Get the dot product of two vec3 objects.
 *
 * @param v0
 * @param v1
 * @return f32
 */
FR_FORCE_INLINE f32 fr_vec3_dot(const vec3* v0, const vec3* v1) {
    // Non SIMD implementation is way faster than the SIMD implementation
    return v0->x * v1->x + v0->y * v1->y + v0->z * v1->z;
}

#if 0
FR_FORCE_INLINE f32 fr_vec3_dot_simd(const vec3* v0, const vec3* v1) {
#if FR_VEC3_SIMD == 0
    __m128 x0, x1;
    x0 = _mm_mul_ps(v0->simd, v1->simd);
    x1 = _mm_add_ps(x0, FR_SIMD_SHUFFLE1(x0, 0, 1, 2, 3));
    x1 = _mm_add_ps(x1, FR_SIMD_SHUFFLE1(x1, 1, 0, 0, 1));
    return _mm_cvtss_f32(x1);
    // Supposed faster implementation dont know why it is slower
    // __m128 prod, sum, shuff;
    // prod = _mm_mul_ps(v0->simd, v1->simd);
    // shuff = FR_VEC3_SIMD_SHUFFLE1(prod, 2, 3, 0, 1);
    // sum = _mm_add_ps(prod, shuff);
    // shuff = _mm_movehl_ps(shuff, sum);
    // sum = _mm_add_ss(sum, shuff);
    // return _mm_cvtss_f32(sum);
#else
    return fr_vec3_dot(v0, v1);
#endif
}
#endif

/**
 * @brief Get the cross product of two vec3 objects.
 *
 * @param v0
 * @param v1
 * @return vec3
 */
FR_FORCE_INLINE void fr_vec3_cross(const vec3* v0, const vec3* v1, vec3* dest) {
#if 0  // The normal cross product implementation is faster than the SIMD
       // implementation
    __m128 x0 = FR_VEC3_SIMD_SHUFFLE1(v0->simd, 3, 0, 2, 1); // x0 = {v0->z, v0->x, v0->y, 0.0f}
    __m128 x1 = FR_VEC3_SIMD_SHUFFLE1(v1->simd, 3, 1, 0, 2); // x1 = {v1->y, v1->z, v1->x, 0.0f}
    __m128 x2 = _mm_mul_ps(x0, v1->simd);
    __m128 x3 = _mm_mul_ps(x0, x1);
    __m128 x4 = FR_VEC3_SIMD_SHUFFLE1(x2, 3, 0, 2, 1);
    _mm_store_ps(dest->data, _mm_sub_ps(x3, x4));
#else
    dest->x = v0->y * v1->z - v0->z * v1->y;
    dest->y = v0->z * v1->x - v0->x * v1->z;
    dest->z = v0->x * v1->y - v0->y * v1->x;
#endif
}

FR_FORCE_INLINE vec3 fr_vec3_get_cross(const vec3* v0, const vec3* v1) {
    vec3 dest;
    fr_vec3_cross(v0, v1, &dest);
    return dest;
}

/**
 * @brief Get he squared norm2 of a vec3 object.
 *
 * @param v
 * @return f32
 */
FR_FORCE_INLINE f32 fr_vec3_norm2(const vec3* v) {
    // Non SIMD implementation is faster than the SIMD implementation
    return fr_vec3_dot(v, v);
}

/**
 * @brief Get the L2 norm of a vec3 object.
 *
 * @param v
 * @return f32
 */
FR_FORCE_INLINE f32 fr_vec3_norm(const vec3* v) {
#if FR_VEC3_SIMD == 1  // Simd seems faster by like 8% for 10million iterations
    return _mm_cvtss_f32(fr_simd_vnorm(v->simd))
#else
    __m128 x = _mm_setr_ps(v->x, v->y, v->z, 0.0f);
    return _mm_cvtss_f32(fr_simd_vnorm(x));
#endif
}

FR_FORCE_INLINE f32 fr_vec3_invnorm_fast(const vec3* v) {
#if FR_VEC3_SIMD == 1
    return _mm_cvtss_f32(fr_simd_vinvnorm_fast(v->simd));
#else
    __m128 x = _mm_setr_ps(v->x, v->y, v->z, 0.0f);
    return _mm_cvtss_f32(fr_simd_vinvnorm_fast(x));
#endif
}

FR_FORCE_INLINE f32 fr_vec3_invnorm(const vec3* v) {
#if FR_VEC3_SIMD == 1
    return _mm_cvtss_f32(fr_simd_vinvnorm(v->simd));
#else
    __m128 x = _mm_setr_ps(v->x, v->y, v->z, 0.0f);
    return _mm_cvtss_f32(fr_simd_vinvnorm(x));
#endif
}

/**
 * @brief L1 norm of a vec3 object.
 *
 * @param v
 * @return f32
 */
FR_FORCE_INLINE f32 fr_vec3_norm1(const vec3* v) {
#if FR_VEC3_SIMD == 1
    return _mm_cvtss_f32(fr_simd_vnorm1(v->simd));
#else
    return fr_abs(v->x) + fr_abs(v->y) + fr_abs(v->z);
#endif
}

/**
 * @brief L-infinity norm of a vec3 object.
 *
 * @param v
 * @return f32
 */
FR_FORCE_INLINE f32 fr_vec3_norm_inf(const vec3* v) {
#if FR_VEC3_SIMD == 1
    return _mm_cvtss_f32(fr_simd_vnorm_inf(v->simd));
#else
    f32 max = fr_abs(v->x);
    f32 temp = fr_abs(v->y);
    if (temp > max) {
        max = temp;
    }
    temp = fr_abs(v->z);
    if (temp > max) {
        max = temp;
    }
    return max;
#endif
}

/**
 * @brief Add two vec3 objects together.
 *
 * @param v0
 * @param v1
 * @return vec3
 */
FR_FORCE_INLINE void fr_vec3_add(const vec3* v0, const vec3* v1, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_add_ps(v0->simd, v1->simd);
#else
    dest->x = v0->x + v1->x;
    dest->y = v0->y + v1->y;
    dest->z = v0->z + v1->z;
#endif
}

/**
 * @brief Add a scalar value to a vec3 object.
 *
 * @param v
 * @param s
 * @return vec3
 */
FR_FORCE_INLINE void fr_vec3_adds(const vec3* v, f32 s, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_add_ps(v->simd, _mm_set1_ps(s));
#else
    dest->x = v->x + s;
    dest->y = v->y + s;
    dest->z = v->z + s;
#endif
}

/**
 * @brief Subtract two vec3 objects v0 - v1 and store the result in a third vec3
 * object.
 *
 * @param v0
 * @param v1
 * @param dest pointer to the destination vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_sub(const vec3* v0, const vec3* v1, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_sub_ps(v0->simd, v1->simd);
#else
    dest->x = v0->x - v1->x;
    dest->y = v0->y - v1->y;
    dest->z = v0->z - v1->z;
#endif
}

/**
 * @brief Subtract a scalar value from a vec3 object and store the result in a
 * third vec3 object.
 *
 * @param v
 * @param s
 * @param dest pointer to the destination vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_subs(const vec3* v, f32 s, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_sub_ps(v->simd, _mm_set1_ps(s));
#else
    dest->x = v->x - s;
    dest->y = v->y - s;
    dest->z = v->z - s;
#endif
}

/**
 * @brief Elementwise multiply two vec3 objects together and store the result in
 * a third vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest pointer to the destination vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_mul(const vec3* v0, const vec3* v1, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_mul_ps(v0->simd, v1->simd);
#else
    dest->x = v0->x * v1->x;
    dest->y = v0->y * v1->y;
    dest->z = v0->z * v1->z;
#endif
}

/**
 * @brief Scale a vec3 by s and store the result in a third vec3 object.
 *
 * @param v
 * @param s
 * @param dest pointer to the destination vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_scale(const vec3* v, f32 s, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_mul_ps(v->simd, _mm_set1_ps(s));
#else
    dest->x = v->x * s;
    dest->y = v->y * s;
    dest->z = v->z * s;
#endif
}

/**
 * @brief Scale the unit vector in the direction of v by s and store the result
 * in the destination.
 *
 * @param v
 * @param s
 * @param dest
 */
FR_FORCE_INLINE void fr_vec3_scale_direction(const vec3* v, f32 s, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = fr_simd_unit_vector(v->simd);
    dest->simd = _mm_mul_ps(dest->simd, _mm_set1_ps(s));
#else
    f32 const norm = fr_vec3_norm(v);
    if (norm == 0.0f) {
        fr_vec3_zeros(dest);
        return;
    }
    fr_vec3_scale(v, s / norm, dest);
#endif
}

/**
 * @brief Divide two vec3 objects elementwise and store the result in a third
 * vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest pointer to the destination vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_div(const vec3* v0, const vec3* v1, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_div_ps(v0->simd, v1->simd);
#else
    dest->x = v0->x / v1->x;
    dest->y = v0->y / v1->y;
    dest->z = v0->z / v1->z;
#endif
}

/**
 * @brief Divide a vec3 object by a scalar value and store the result in a third
 * vec3 object.
 * @details If the scalar value is zero the function will return a vec3 with all
 * NaN components.
 *
 * @param v
 * @param s
 * @param dest pointer to the destination vec3 object.
 */
FR_FORCE_INLINE void fr_vec3_divs(const vec3* v, f32 s, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_div_ps(v->simd, _mm_set1_ps(s));
#else
    dest->x = v->x / s;
    dest->y = v->y / s;
    dest->z = v->z / s;
#endif
}

/**
 * @brief Add two vec3 objects together and store the result in a third vec3
 * object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_FORCE_INLINE void fr_vec3_faddadd(const vec3* v0, const vec3* v1, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_add_ps(dest->simd, _mm_add_ps(v0->simd, v1->simd));
#else
    dest->x += v0->x + v1->x;
    dest->y += v0->y + v1->y;
    dest->z += v0->z + v1->z;
#endif
}

/**
 * @brief Subtract two vec3 objects v0 - v1 and add the result to the third vec3
 * object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_FORCE_INLINE void fr_vec3_fsubadd(const vec3* v0, const vec3* v1, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_add_ps(dest->simd, _mm_sub_ps(v0->simd, v1->simd));
#else
    dest->x += v0->x - v1->x;
    dest->y += v0->y - v1->y;
    dest->z += v0->z - v1->z;
#endif
}

/**
 * @brief elementwise multiply two vec3 objects together and add the result to
 * third vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_FORCE_INLINE void fr_vec3_fmuladd(const vec3* v0, const vec3* v1, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_add_ps(dest->simd, _mm_mul_ps(v0->simd, v1->simd));
#else
    dest->x += v0->x * v1->x;
    dest->y += v0->y * v1->y;
    dest->z += v0->z * v1->z;
#endif
}

/**
 * @brief Multiply a vec3 object by a scalar value and add the result to a third
 * vec3 object.
 *
 * @param v
 * @param s
 * @param dest
 */
FR_FORCE_INLINE void fr_vec3_fmuladds(const vec3* v, f32 s, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_add_ps(dest->simd, _mm_mul_ps(v->simd, _mm_set1_ps(s)));
#else
    dest->x += v->x * s;
    dest->y += v->y * s;
    dest->z += v->z * s;
#endif
}

/**
 * @brief Add the maximum components of two vec3 objects to a third vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_FORCE_INLINE void fr_vec3_fmaxadd(const vec3* v0, const vec3* v1, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_add_ps(dest->simd, _mm_max_ps(v0->simd, v1->simd));
#else
    dest->x += fr_max(v0->x, v1->x);
    dest->y += fr_max(v0->y, v1->y);
    dest->z += fr_max(v0->z, v1->z);
#endif
}

/**
 * @brief Add the minimum components of two vec3 objects to a third vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_FORCE_INLINE void fr_vec3_fminadd(const vec3* v0, const vec3* v1, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_add_ps(dest->simd, _mm_min_ps(v0->simd, v1->simd));
#else
    dest->x += fr_min(v0->x, v1->x);
    dest->y += fr_min(v0->y, v1->y);
    dest->z += fr_min(v0->z, v1->z);
#endif
}

/**
 * @brief Subtract two vec3 objects v0 - v1 and subtract the result from the
 * third vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_FORCE_INLINE void fr_vec3_fsubsub(const vec3* v0, const vec3* v1, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_sub_ps(dest->simd, _mm_sub_ps(v0->simd, v1->simd));
#else
    dest->x -= v0->x - v1->x;
    dest->y -= v0->y - v1->y;
    dest->z -= v0->z - v1->z;
#endif
}

/**
 * @brief Add two vec3 objects together and subtract the result from the third
 * vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_FORCE_INLINE void fr_vec3_faddsub(const vec3* v0, const vec3* v1, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_sub_ps(dest->simd, _mm_add_ps(v0->simd, v1->simd));
#else
    dest->x -= v0->x + v1->x;
    dest->y -= v0->y + v1->y;
    dest->z -= v0->z + v1->z;
#endif
}

/**
 * @brief Elementwise multiply two vec3 objects together and subtract the result
 * from the third vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_FORCE_INLINE void fr_vec3_fmulsub(const vec3* v0, const vec3* v1, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_sub_ps(dest->simd, _mm_mul_ps(v0->simd, v1->simd));
#else
    dest->x -= v0->x * v1->x;
    dest->y -= v0->y * v1->y;
    dest->z -= v0->z * v1->z;
#endif
}

/**
 * @brief Multiply a vec3 object by a scalar value and subtract the result from
 * a third vec3 object.
 *
 * @param v
 * @param s
 * @param dest
 */
FR_FORCE_INLINE void fr_vec3_fmulsubs(const vec3* v, f32 s, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_sub_ps(dest->simd, _mm_mul_ps(v->simd, _mm_set1_ps(s)));
#else
    dest->x -= v->x * s;
    dest->y -= v->y * s;
    dest->z -= v->z * s;
#endif
}

/**
 * @brief Add the maximum components of two vec3 objects to a third vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_FORCE_INLINE void fr_vec3_fmaxsub(const vec3* v0, const vec3* v1, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_sub_ps(dest->simd, _mm_max_ps(v0->simd, v1->simd));
#else
    dest->x -= fr_max(v0->x, v1->x);
    dest->y -= fr_max(v0->y, v1->y);
    dest->z -= fr_max(v0->z, v1->z);
#endif
}

/**
 * @brief Add the minimum components of two vec3 objects to a third vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_FORCE_INLINE void fr_vec3_fminsub(const vec3* v0, const vec3* v1, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_sub_ps(dest->simd, _mm_min_ps(v0->simd, v1->simd));
#else
    dest->x -= fr_min(v0->x, v1->x);
    dest->y -= fr_min(v0->y, v1->y);
    dest->z -= fr_min(v0->z, v1->z);
#endif
}

/**
 * @brief Negate a vec3 object.
 *
 * @param v
 */
FR_FORCE_INLINE void fr_vec3_negate(vec3* v) {
#if FR_VEC3_SIMD == 1
    v->simd = _mm_xor_ps(v->simd, FR_SIGN_BITf32x4);
#else
    v->x = -v->x;
    v->y = -v->y;
    v->z = -v->z;
#endif
}

/**
 * @brief Negate a vec3 object and store the result in an another vec3 object.
 *
 * @param v
 * @param dest
 */
FR_FORCE_INLINE void fr_vec3_negative(const vec3* v, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_xor_ps(v->simd, FR_SIGN_BITf32x4);
#else
    dest->x = -v->x;
    dest->y = -v->y;
    dest->z = -v->z;
#endif
}

/**
 * @brief Normalize a vec3 object.
 *
 * @param v
 */
FR_FORCE_INLINE void fr_vec3_normalize(const vec3* v, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = fr_simd_unit_vector(v->simd);
#else
    f32 const norm = fr_vec3_norm(v);
    if (norm == 0.0f) {
        fr_vec3_zeros(dest);
        return;
    }
    fr_vec3_scale(v, 1 / norm, dest);
#endif
}

/**
 * @brief Get the angle between two vec3 objects.
 *
 * @param v0
 * @param v1
 * @return f32
 */
FR_FORCE_INLINE f32 fr_vec3_angle_between(const vec3* v0, const vec3* v1) {
    f32 const dot = fr_vec3_dot(v0, v1);
    vec3 const cross = fr_vec3_get_cross(v0, v1);
    f32 const norm = fr_vec3_norm(&cross);
    return fr_atan2(norm, dot);
}

/**
 * @brief Get the projection of a vec3 object onto another vec3 object.
 *
 * @param v
 * @param onto
 * @return vec3
 */
FR_FORCE_INLINE void fr_vec3_project(vec3* v, vec3* onto, vec3* dest) {
    f32 const dot = fr_vec3_dot(v, onto);
    fr_vec3_scale_direction(onto, dot, dest);
}

/**
 * @brief Get the midpoint between two vec3
 *
 * @param v0
 * @param v1
 * @return vec3
 */
FR_FORCE_INLINE void fr_vec3_center(const vec3* v0, const vec3* v1, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_mul_ps(_mm_add_ps(v0->simd, v1->simd), _mm_set1_ps(0.5f));
#else
    dest->x = (v0->x + v1->x) * 0.5f;
    dest->y = (v0->y + v1->y) * 0.5f;
    dest->z = (v0->z + v1->z) * 0.5f;
#endif
}

/**
 * @brief Get the distance squared between two vec3
 *
 * @param v0
 * @param v1
 * @return f32
 */
FR_FORCE_INLINE f32 fr_vec3_distance2(const vec3* v0, const vec3* v1) {
#if FR_VEC3_SIMD == 1
    __m128 x0 = _mm_sub_ps(v0->simd, v1->simd);
    return _mm_cvtss_f32(fr_simd_vdot(x0, x0));
#else
    f32 const dx = v0->x - v1->x;
    f32 const dy = v0->y - v1->y;
    f32 const dz = v0->z - v1->z;
    return dx * dx + dy * dy + dz * dz;
#endif
}

/**
 * @brief Get the distance between two vec3
 *
 * @param v0
 * @param v1
 * @return f32
 */
FR_FORCE_INLINE f32 fr_vec3_distance(const vec3* v0, const vec3* v1) {
#if FR_VEC3_SIMD == 1
    __m128 x0 = _mm_sub_ps(v0->simd, v1->simd);
    return _mm_cvtss_f32(fr_simd_vnorm(x0));
#else
    f32 const dx = v0->x - v1->x;
    f32 const dy = v0->y - v1->y;
    f32 const dz = v0->z - v1->z;
    return fr_sqrt(dx * dx + dy * dy + dz * dz);
#endif
}

/**
 * @brief Get a vec3 with the maximum components of two vec3 objects
 *
 * @param v1
 * @param v2
 * @return vec3
 */
FR_FORCE_INLINE void fr_vec3_maxv(const vec3* v1, const vec3* v2, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_max_ps(v1->simd, v2->simd);
#else
    dest->x = fr_max(v1->x, v2->x);
    dest->y = fr_max(v1->y, v2->y);
    dest->z = fr_max(v1->z, v2->z);
#endif
}

/**
 * @brief Get a vec3 with the minimum components of two vec3 objects
 *
 * @param v1
 * @param v2
 * @return vec3
 */
FR_FORCE_INLINE void fr_vec3_minv(const vec3* v1, const vec3* v2, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_min_ps(v1->simd, v2->simd);
#else
    dest->x = fr_min(v1->x, v2->x);
    dest->y = fr_min(v1->y, v2->y);
    dest->z = fr_min(v1->z, v2->z);
#endif
}

/**
 * @brief Clamp the components of a vec3 object between minVal and maxVal.
 *
 * @param v
 * @param minVal
 * @param maxVal
 */
FR_FORCE_INLINE void fr_vec3_clamp(vec3* v, f32 minVal, f32 maxVal, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_min_ps(_mm_max_ps(v->simd, _mm_set1_ps(minVal)), _mm_set1_ps(maxVal));
#else
    dest->x = fr_clamp(v->x, minVal, maxVal);
    dest->y = fr_clamp(v->y, minVal, maxVal);
    dest->z = fr_clamp(v->z, minVal, maxVal);
#endif
}

/**
 * @brief Linearly interpolate between two vec3 objects using t
 *
 * @param from
 * @param to
 * @param t
 * @return vec3
 */
FR_FORCE_INLINE void fr_vec3_lerp(const vec3* from, const vec3* to, f32 t, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = _mm_add_ps(from->simd, _mm_mul_ps(_mm_sub_ps(to->simd, from->simd), _mm_set1_ps(t)));
#else
    dest->x = from->x + (to->x - from->x) * t;
    dest->y = from->y + (to->y - from->y) * t;
    dest->z = from->z + (to->z - from->z) * t;
#endif
}

/**
 * @brief smoothstep threshold function with a smooth transition between 0 and 1
 * and store the result in a third vec3 object.
 *
 * @param edge0
 * @param edge1
 * @param x the input value
 * @param dest
 */
FR_FORCE_INLINE void fr_vec3_smoothstep(const vec3* edge0, const vec3* edge1, f32 x, vec3* dest) {
#if FR_VEC3_SIMD == 1
    dest->simd = fr_simd_vsmoothstep(edge0->simd, edge1->simd, x);
#else
    dest->x = fr_smoothstep(edge0->x, edge1->x, x);
    dest->y = fr_smoothstep(edge0->y, edge1->y, x);
    dest->z = fr_smoothstep(edge0->z, edge1->z, x);
#endif
}

/**
 * @brief smooth interpolation between two vec3 objects and store the result in
 * a third vec3 object.
 *
 * @param from
 * @param to
 * @param t
 * @param dest
 */
FR_FORCE_INLINE void fr_vec3_smooth_interp(const vec3* from, const vec3* to, f32 t, vec3* dest) {
    fr_vec3_lerp(from, to, fr_smooth(t), dest);
}
