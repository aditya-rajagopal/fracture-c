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

#include "detail/vec4.h"
#include "detail/vec3.h"
#include "detail/vec2.h"

#include "math_constants.h"
#include "utils.h"

#include "fracture/core/library/random/fr_random.h"

// ------------------------------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------------------------------

/**
 * @brief Create a new vec4 from the given values
 * 
 * @param x The x component 
 * @param y The y component
 * @param z The z component
 * @param w The w component
 * @param dest The destination vec4
 */
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

/**
 * @brief Create a new vec4 from the given vec3 and w component
 * 
 * @param xyz The vec3
 * @param w The w component
 *@param dest The destination vec4
 */
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

/**
 * @brief Create a new vec4 from the given vec2 and z, w components
 * 
 * @param xy The vec2
 * @param z The z component
 * @param w The w component
 * @param dest The destination vec4
 */
FR_FORCE_INLINE void fr_vec4_v2ss(const vec2* xy, f32 z, f32 w, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_set_ps(w, z, xy->y, xy->x);
#else
    dest->x = xy->x;
    dest->y = xy->y;
    dest->z = z;
    dest->w = w;
#endif
}

/**
 * @brief Create a new vec4 from the given vec2 and vec2
 * 
 * @param xy The vec2 for the xy components
 * @param zw The vec2 for the zw components
 * @param dest The destination vec4
 */
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

/**
 * @brief Fill the given vec4 with the given value s
 * 
 * @param s The value to fill the vec4 with
 * @param dest The destination vec4
 */
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

/**
 * @brief Fill the given vec4 with zeros
 * 
 * @param dest The destination vec4
 */
FR_FORCE_INLINE void fr_vec4_zeros(vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_setzero_ps();
#else
    dest->x = 0.0f;
    dest->y = 0.0f;
    dest->z = 0.0f;
    dest->w = 0.0f;
#endif
}

/**
 * @brief Fill the given vec4 with ones
 * 
 * @param dest The destination vec4
 */
FR_FORCE_INLINE void fr_vec4_ones(vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_set1_ps(1.0f);
#else
    dest->x = 1.0f;
    dest->y = 1.0f;
    dest->z = 1.0f;
    dest->w = 1.0f;
#endif
}

/**
 * @brief Create a new vec4 from the given array
 * 
 * @param arr The array of f32 with at least 4 elements
 * @param dest The destination vec4
 */
FR_FORCE_INLINE void fr_vec4_create(const f32* restrict data, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_loadu_ps(data);
#else
    dest->x = data[0];
    dest->y = data[1];
    dest->z = data[2];
    dest->w = data[3];
#endif
}

/**
 * @brief Create a new vec4 from the given 16bit aligned array.
 * 
 * @param arr The array of f32 with at least 4 elements
 * @param dest The destination vec4
 */
FR_FORCE_INLINE void fr_vec4_create_aligned(const f32* restrict data, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_load_ps(data);
#else
    dest->x = data[0];
    dest->y = data[1];
    dest->z = data[2];
    dest->w = data[3];
#endif
}

/**
 * @brief Create a new vec4 with random uniform values
 * 
 * @param dest The destination vec4
 * @param config The configuration of the random number generator
 */
FR_FORCE_INLINE void fr_vec4_random_uniform(vec4* dest, void* config) {
#if FR_SIMD == 1
    f32 FR_ALIGN(16) data[4] = {
        fr_random_uniform(config), fr_random_uniform(config), fr_random_uniform(config), fr_random_uniform(config)};
    dest->simd = _mm_load_ps(data);
#else
    dest->x = fr_random_uniform(state);
    dest->y = fr_random_uniform(state);
    dest->z = fr_random_uniform(state);
    dest->w = fr_random_uniform(state);
#endif
}

/**
 * @brief Create a new vec4 with random uniform values in the given range
 *
 * @param dest The destination vec4
 * @param state The configuration of the random number generator
 * @param min The minimum value
 * @param max The maximum value
 */
FR_FORCE_INLINE void fr_vec4_random_uniform_range(vec4* dest, void* config, f32 min, f32 max) {
#if FR_SIMD == 1
    FR_ALIGN(16)
    f32 data[4] = {fr_random_uniform_range(config, min, max),
                   fr_random_uniform_range(config, min, max),
                   fr_random_uniform_range(config, min, max),
                   fr_random_uniform_range(config, min, max)};
    dest->simd = _mm_load_ps(data);
#else
    dest->x = fr_random_uniform_range(config, min, max);
    dest->y = fr_random_uniform_range(config, min, max);
    dest->z = fr_random_uniform_range(config, min, max);
    dest->w = fr_random_uniform_range(config, min, max);
#endif
}

// ------------------------------------------------------------------------------------------------
// Utilities
// ------------------------------------------------------------------------------------------------

/**
 * @brief Compare 2 vec4s and store the element-wise equality in a bvec4.
 * @details The comparison is done with a threshold of FLOAT_EPSILON
 * @param a
 * @param b 
 * @param dest 
 */
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

/**
 * @brief Compare 2 vec4s and store the element-wise inequality in a bvec4.
 * @details The comparison is done with a threshold of FLOAT_EPSILON
 * @param a
 * @param b 
 * @param dest 
 */
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


/**
 * @brief Compare 2 vec4s and store the element-wise equality in a vec4.
 * @details The comparison is done with a threshold of FLOAT_EPSILON
 * @param a
 * @param b 
 * @param dest 
 */
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

/**
 * @brief Compare 2 vec4s and store the element-wise inequality in a vec4.
 * @details The comparison is done with a threshold of FLOAT_EPSILON
 * @param a
 * @param b 
 * @param dest 
 */
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

/**
 * @brief Compare 2 vec4s and return true if they are equal
 * @details The comparison is done with a threshold of FLOAT_EPSILON
 * @param a
 * @param b 
 * @return b8 TRUE if every component of a is equal to the corresponding component of b
 */
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

/**
 * @brief Compare 2 vec4s and return true if they are equal exactly
 *
 * @param a
 * @param b 
 * @return b8 TRUE if every component of a is equal to the corresponding component of b
 */
FR_FORCE_INLINE b8 fr_vec4_eqv_exact(const vec4* a, const vec4* b) {
#if FR_SIMD == 1
    return _mm_movemask_ps(_mm_cmpeq_ps(a->simd, b->simd)) == 0x0F;
#else
    return a->x == b->x && a->y == b->y && a->z == b->z && a->w == b->w;
#endif
}

/**
 * @brief Compare 2 vec4s and return true if they are not equal
 * @details The comparison is done with a threshold of FLOAT_EPSILON
 * @param a
 * @param b 
 * @return b8 TRUE if ANY component of a is not equal to the corresponding component of b
 */
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

/**
 * @brief Compare 2 vec4s and return true if they are not equal exactly
 *
 * @param a
 * @param b 
 * @return b8 TRUE if ANY component of a is not equal to the corresponding component of b
 */
FR_FORCE_INLINE b8 fr_vec4_neqv_exact(const vec4* a, const vec4* b) {
#if FR_SIMD == 1
    return _mm_movemask_ps(_mm_cmpneq_ps(a->simd, b->simd)) != 0x0F;
#else
    return a->x != b->x || a->y != b->y || a->z != b->z || a->w != b->w;
#endif
}

/**
 * @brief Compare a vec4 with a scalar and store the element-wise equality in a bvec4.
 * @details The comparison is done with a threshold of FLOAT_EPSILON
 * @param a
 * @param b 
 * @param dest
 */
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

/**
 * @brief Compare a vec4 with a scalar and store the element-wise inequality in a bvec4.
 * @details The comparison is done with a threshold of FLOAT_EPSILON
 * @param a
 * @param b 
 * @param dest
 */
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

/**
 * @brief Compare a vec4 with a scalar and store the element-wise inequality in a bvec4.
 * @details The comparison is done with a threshold of FLOAT_EPSILON
 * @param a
 * @param b 
 * @param dest
 */
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

/**
 * @brief Compare a vec4 with a scalar and return true if they are equal
 * @details The comparison is done with a threshold of FLOAT_EPSILON
 * @param a
 * @param b 
 * @return b8 TRUE if every component of a is equal to s
 */
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

/**
 * @brief Compare a vec4 with a scalar and return true if they are not equal
 * @details The comparison is done with a threshold of FLOAT_EPSILON
 * @param a
 * @param b 
 * @return b8 TRUE if ANY component of a is not equal to s
 */
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

/**
 * @brief Compare a vec4 with a scalar and return true if they are equal exactly
 *
 * @param a
 * @param b 
 * @return b8 TRUE if every component of a is equal to s
 */
FR_FORCE_INLINE b8 fr_vec4_eqs_exact(const vec4* a, f32 s) {
#if FR_SIMD == 1
    return _mm_movemask_ps(_mm_cmpeq_ps(a->simd, _mm_set1_ps(s))) == 0x0F;
#else
    return a->x == s && a->y == s && a->z == s && a->w == s;
#endif
}

/**
 * @brief Compare a vec4 with a scalar and return true if they are not equal exactly
 *
 * @param a
 * @param b 
 * @return b8 TRUE if ANY component of a is not equal to s
 */
FR_FORCE_INLINE b8 fr_vec4_neqs_exact(const vec4* a, f32 s) {
#if FR_SIMD == 1
    return _mm_movemask_ps(_mm_cmpneq_ps(a->simd, _mm_set1_ps(s))) != 0x0F;
#else
    return a->x != s || a->y != s || a->z != s || a->w != s;
#endif
}

/**
 * @brief Compare 2 vec4s and store the element-wise greater than in a vec4.
 * @param a
 * @param b 
 * @param dest 
 */
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

/**
 * @brief Compare 2 vec4s and store the element-wise less than in a vec4.
 * @param a
 * @param b 
 * @param dest 
 */
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

/**
 * @brief Compare 2 vec4s and store the element-wise greater than or equal in a vec4.
 * @param a
 * @param b 
 * @param dest 
 */
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

/**
 * @brief Compare 2 vec4s and store the element-wise less than or equal in a vec4.
 * @param a
 * @param b 
 * @param dest 
 */
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

/**
 * @brief Compare a vec4 with a scalar and store the element-wise greater than in a vec4.
 * @param a
 * @param b 
 * @param dest 
 */
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

/**
 * @brief Compare a vec4 with a scalar and store the element-wise less than in a vec4.
 * @param a
 * @param b 
 * @param dest 
 */
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

/**
 * @brief Compare a vec4 with a scalar and store the element-wise greater than or equal in a vec4.
 * @param a
 * @param b 
 * @param dest 
 */
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

/**
 * @brief Compare a vec4 with a scalar and store the element-wise less than or equal in a vec4.
 * @param a
 * @param b 
 * @param dest 
 */
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

/**
 * @brief Checks if a vec4 is zero
 * 
 * @param a 
 * @return b8 TRUE if every component of a is equal to 0
 */
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

/**
 * @brief Compute the dot product of 2 vec4s
 * 
 * @param a 
 * @param b 
 * @return f32 The dot product of a and b
 */
FR_FORCE_INLINE f32 fr_vec4_dot(const vec4* a, const vec4* b) {
#if FR_SIMD == 1 // In O0 optimization non simd is faster than simd
   return _mm_cvtss_f32(fr_simd_vdot(a->simd, b->simd));
#else
    return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
#endif
}

/**
 * @brief Computes the L2 norm of a vec4
 * 
 * @param a 
 * @return f32 The L2 norm of a
 */
FR_FORCE_INLINE f32 fr_vec4_norm(const vec4* a) {
#if 1 // non simd is faster if you dont have O2 optimization
    return _mm_cvtss_f32(fr_simd_vnorm(a->simd));
#else
    return fr_sqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
#endif
}


/**
 * @brief Computes the inverse of the L2 norm of a vec4 using a fast approximation
 * 
 * @param a 
 * @return f32 The inverse of the L2 norm of a
 */
FR_FORCE_INLINE f32 fr_vec4_invnorm_fast(const vec4* a) {
#if 1 // non simd is faster if you dont have O2 optimization
    return _mm_cvtss_f32(fr_simd_vinvnorm_fast(a->simd));
#else
    return 1.0 / fr_sqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
#endif
}

/**
 * @brief Computes the inverse of the L2 norm of a vec4
 * 
 * @param a 
 * @return f32 The inverse of the L2 norm of a
 */
FR_FORCE_INLINE f32 fr_vec4_invnorm(const vec4* a) {
#if 1 // non simd is faster if you dont have O2 optimization
    return _mm_cvtss_f32(fr_simd_vinvnorm(a->simd));
#else
    return fr_sqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
#endif
}

/**
 * @brief Computes the squared L2 norm of a vec4
 * 
 * @param a 
 * @return f32 The squared L2 norm of a
 */
FR_FORCE_INLINE f32 fr_vec4_norm2(const vec4* a) {
#if 1 // non simd is faster if you dont have O2 optimization
    return _mm_cvtss_f32(fr_simd_vnorm2(a->simd));
#else
    return a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w;
#endif
}

/**
 * @brief Computes the L1 norm of a vec4
 * 
 * @param a 
 * @return f32 The L1 norm of a
 */
FR_FORCE_INLINE f32 fr_vec4_norm1(const vec4* a) {
#if 1 // non simd is faster if you dont have O2 optimization
    return _mm_cvtss_f32(fr_simd_vnorm1(a->simd));
#else
    return fr_abs(a->x) + fr_abs(a->y) + fr_abs(a->z) + fr_abs(a->w);
#endif
}

/**
 * @brief Computes the L_inf norm of a vec4
 * 
 * @param a 
 * @return f32 The L_inf norm of a
 */
FR_FORCE_INLINE f32 fr_vec4_norm_inf(const vec4* a) {
#if 1 // non simd is faster if you dont have O2 optimization
    return _mm_cvtss_f32(fr_simd_vnorm_inf(a->simd));
#else
    return fr_max(fr_max(fr_abs(a->x), fr_abs(a->y)), fr_max(fr_abs(a->z), fr_abs(a->w)));
#endif
}

/**
 * @brief Compute a unit vector from the given vec4 and store it in dest
 * 
 * @param a 
 * @param dest 
 */
FR_FORCE_INLINE void fr_vec4_normalize(const vec4* a, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = fr_simd_unit_vector(a->simd);
#else
    f32 norm = fr_vec4_norm(a);
    if (norm == 0.0f) {
        fr_vec4_zero(dest);
        return;
    }
    f32 inv_norm = 1.0f / norm;
    dest->x = a->x * inv_norm;
    dest->y = a->y * inv_norm;
    dest->z = a->z * inv_norm;
    dest->w = a->w * inv_norm;
#endif
}

/**
 * @brief Compute a unit vector from the given vec4 and store it in dest
 * @details This function does not check for division by zero
 * 
 * @param a 
 * @param dest 
 */
FR_FORCE_INLINE void fr_vec4_normalize_unsafe(const vec4* a, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = fr_simd_unit_vector_unsafe(a->simd);
#else
    f32 inv_norm = 1.0f / fr_vec4_norm(a);
    dest->x = a->x * inv_norm;
    dest->y = a->y * inv_norm;
    dest->z = a->z * inv_norm;
    dest->w = a->w * inv_norm;
#endif
}

/**
 * @brief Compute the absolute value of a vec4 and store it in dest
 * 
 * @param a 
 * @param dest 
 */
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

/**
 * @brief Compute the negation of a vec4 and store it in dest
 * 
 * @param a 
 * @param dest 
 */
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

/**
 * @brief Compute the sign of the elements in a vec4 and store it in dest
 *
 * @param a
 * @param dest
 */
FR_FORCE_INLINE void fr_vec4_sign(const vec4* a, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = fr_simd_sign(a->simd);
#else
    dest->x = fr_sign(a->x);
    dest->y = fr_sign(a->y);
    dest->z = fr_sign(a->z);
    dest->w = fr_sign(a->w);
#endif
}

/**
 * @brief Compute the sum of 2 vec4s and store it in dest
 * 
 * @param a 
 * @param b 
 * @param dest 
 */
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

/**
 * @brief Compute the sum of a vec4 and a scalar and store it in dest
 * 
 * @param a 
 * @param s 
 * @param dest 
 */
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

/**
 * @brief Compute the difference of 2 vec4s and store it in dest
 * 
 * @param a 
 * @param b 
 * @param dest 
 */
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

/**
 * @brief Compute the difference of a vec4 and a scalar and store it in dest
 * 
 * @param a 
 * @param s 
 * @param dest 
 */
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

/**
 * @brief Compute the element-wise product of 2 vec4s and store it in dest
 * 
 * @param a 
 * @param b 
 * @param dest 
 */
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

/**
 * @brief Scale a vec4 by a scalar and store it in dest
 * 
 * @param a 
 * @param s 
 * @param dest 
 */
FR_FORCE_INLINE void fr_vec4_scale(const vec4* a, f32 s, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_mul_ps(a->simd, _mm_set1_ps(s));
#else
    dest->x = a->x * s;
    dest->y = a->y * s;
    dest->z = a->z * s;
    dest->w = a->w * s;
#endif
}

/**
 * @brief Compute the element-wise division of 2 vec4s and store it in dest
 *
 * @param a
 * @param b
 * @param dest
 */
FR_FORCE_INLINE void fr_vec4_div(const vec4* a, const vec4* b, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_div_ps(a->simd, b->simd);
#else
    dest->x = a->x / b->x;
    dest->y = a->y / b->y;
    dest->z = a->z / b->z;
    dest->w = a->w / b->w;
#endif
}

/**
 * @brief Compute the element-wise division of a vec4 and a scalar and store it in dest
 * @details This function does not check for division by zero
 * @param a
 * @param s
 * @param dest
 */
FR_FORCE_INLINE void fr_vec4_divs(const vec4* a, f32 s, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_div_ps(a->simd, _mm_set1_ps(s));
#else
    dest->x = a->x / s;
    dest->y = a->y / s;
    dest->z = a->z / s;
    dest->w = a->w / s;
#endif
}

/**
 * @brief Compute the element-wise division of a scalar and a vec4 and store it in dest
 * @param s
 * @param a
 * @param dest
 */
FR_FORCE_INLINE void fr_vec4_maxv(const vec4* a, const vec4* b, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_max_ps(a->simd, b->simd);
#else
    dest->x = fr_max(a->x, b->x);
    dest->y = fr_max(a->y, b->y);
    dest->z = fr_max(a->z, b->z);
    dest->w = fr_max(a->w, b->w);
#endif
}

/**
 * @brief Compute the element-wise division of a scalar and a vec4 and store it in dest
 * @param s
 * @param a
 * @param dest
 */
FR_FORCE_INLINE void fr_vec4_minv(const vec4* a, const vec4* b, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_min_ps(a->simd, b->simd);
#else
    dest->x = fr_min(a->x, b->x);
    dest->y = fr_min(a->y, b->y);
    dest->z = fr_min(a->z, b->z);
    dest->w = fr_min(a->w, b->w);
#endif
}

/**
 * @brief Compute the element-wise maximum of a vec4 and a scalar and store it in dest
 * @param a
 * @param s
 * @param dest
 */
FR_FORCE_INLINE void fr_vec4_maxs(const vec4* a, f32 s, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_max_ps(a->simd, _mm_set1_ps(s));
#else
    dest->x = fr_max(a->x, s);
    dest->y = fr_max(a->y, s);
    dest->z = fr_max(a->z, s);
    dest->w = fr_max(a->w, s);
#endif
}

/**
 * @brief Compute the element-wise minimum of a vec4 and a scalar and store it in dest
 * @param a
 * @param s
 * @param dest
 */
FR_FORCE_INLINE void fr_vec4_mins(const vec4* a, f32 s, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = _mm_min_ps(a->simd, _mm_set1_ps(s));
#else
    dest->x = fr_min(a->x, s);
    dest->y = fr_min(a->y, s);
    dest->z = fr_min(a->z, s);
    dest->w = fr_min(a->w, s);
#endif
}

/**
 * @brief Get a vector in the diretion of a with a magnitude of s
 * @details If a is a zero vector, the result will be a zero vector 
 * @param a 
 * @param s 
 * @param dest 
 */
FR_FORCE_INLINE void fr_vec4_scale_direction(const vec4* a, f32 s, vec4* dest) {
#if FR_SIMD == 1
    __m128 unit_vector = fr_simd_unit_vector(a->simd);
    dest->simd = _mm_mul_ps(unit_vector, _mm_set1_ps(s));
#else
    f32 norm = fr_vec4_norm(a);
    if (norm == 0.0f) {
        fr_vec4_zero(dest);
        return;
    }
    f32 inv_norm = 1.0f / norm;
    dest->x = a->x * inv_norm * s;
    dest->y = a->y * inv_norm * s;
    dest->z = a->z * inv_norm * s;
    dest->w = a->w * inv_norm * s;
#endif
}

/**
 * @brief Clamp the elements of a vec4 between min and max and store the result in dest
 * 
 * @param a the vector to clamp
 * @param min a vector of minimum values
 * @param max a vector of maximum values
 * @param dest 
 */
FR_FORCE_INLINE void fr_vec4_clampv(const vec4* a, const vec4* min, const vec4* max, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = fr_simd_clampv(a->simd, min->simd, max->simd);
#else
    dest->x = fr_clamp(a->x, min->x, max->x);
    dest->y = fr_clamp(a->y, min->y, max->y);
    dest->z = fr_clamp(a->z, min->z, max->z);
    dest->w = fr_clamp(a->w, min->w, max->w);
#endif
}

/**
 * @brief Clamp the elements of a vec4 between min and max and store the result in dest
 * 
 * @param a the vector to clamp
 * @param min the minimum value
 * @param max the maximum value
 * @param dest 
 */
FR_FORCE_INLINE void fr_vec4_clamp(const vec4* a, f32 min, f32 max, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = fr_simd_clamp(a->simd, min, max);
#else
    dest->x = fr_clamp(a->x, min, max);
    dest->y = fr_clamp(a->y, min, max);
    dest->z = fr_clamp(a->z, min, max);
    dest->w = fr_clamp(a->w, min, max);
#endif
}

/**
 * @brief Clamp the elements of a vec4 between 0 and 1 and store the result in dest
 * 
 * @param a the vector to clamp
 * @param dest 
 */
FR_FORCE_INLINE void fr_vec4_clamp01(const vec4* a, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = fr_simd_clamp(a->simd, 0.0f, 1.0f);
#else
    dest->x = fr_clamp(a->x, 0.0f, 1.0f);
    dest->y = fr_clamp(a->y, 0.0f, 1.0f);
    dest->z = fr_clamp(a->z, 0.0f, 1.0f);
    dest->w = fr_clamp(a->w, 0.0f, 1.0f);
#endif
}

/**
 * @brief For each component of x, if x[i] < edge[i] then dest[i] = 0 else dest[i] = 1
 * 
 * @param edge 
 * @param x 
 * @param dest 
 */
FR_FORCE_INLINE void fr_vec4_step(const vec4* edge, const vec4* x, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = fr_simd_step(edge->simd, x->simd);
#else
    dest->x = fr_step(edge->x, x->x);
    dest->y = fr_step(edge->y, x->y);
    dest->z = fr_step(edge->z, x->z);
    dest->w = fr_step(edge->w, x->w);
#endif
}

/**
 * @brief For each component of x, if x[i] < edge then dest[i] = 0 else dest[i] = 1
 * 
 * @param edge 
 * @param x 
 * @param dest 
 */
FR_FORCE_INLINE void fr_vec4_steps(f32 edge, const vec4* x, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = fr_simd_step(_mm_set1_ps(edge), x->simd);
#else
    dest->x = fr_step(edge, x->x);
    dest->y = fr_step(edge, x->y);
    dest->z = fr_step(edge, x->z);
    dest->w = fr_step(edge, x->w);
#endif
}

/**
 * @brief Linearly interpolate between 2 vec4s with parameter t and store the result in dest
 * 
 * @param a 
 * @param b 
 * @param t 
 * @param dest 
 */
FR_FORCE_INLINE void fr_vec4_lerp(const vec4* a, const vec4* b, f32 t, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = fr_simd_lerp(a->simd, b->simd, t);
#else
    dest->x = fr_lerp(a->x, b->x, t);
    dest->y = fr_lerp(a->y, b->y, t);
    dest->z = fr_lerp(a->z, b->z, t);
    dest->w = fr_lerp(a->w, b->w, t);
#endif
}

/**
 * @brief Linearly interpolate between 2 vec4s with parameter t and store the result in dest
 * @details This function clamps t between 0 and 1
 * 
 * @param a 
 * @param b 
 * @param t 
 * @param dest 
 */
FR_FORCE_INLINE void fr_vec4_lerp_clamped(const vec4* a, const vec4* b, f32 t, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = fr_simd_lerp(a->simd, b->simd, fr_clamp_zo(t));
#else
    dest->x = fr_lerp_clamped(a->x, b->x, t);
    dest->y = fr_lerp_clamped(a->y, b->y, t);
    dest->z = fr_lerp_clamped(a->z, b->z, t);
    dest->w = fr_lerp_clamped(a->w, b->w, t);
#endif
}

/**
 * @brief Compute a smooth transition between 2 edges with parameter t and store the result in dest
 * @details for each component of t, the result is 0 if t < edge0, 1 if t > edge1, and the smooth interpolation between
 * 0 and 1 otherwise using a polynomial of degree 3
 * @param edge0
 * @param edge1
 * @param t
 * @param dest
 */
FR_FORCE_INLINE void fr_vec4_smoothstep(const vec4* edge0, const vec4* edge1, const vec4* t, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = fr_simd_vsmoothstep(edge0->simd, edge1->simd, t->simd);
#else
    dest->x = fr_smoothstep(edge0->x, edge1->x, t->x);
    dest->y = fr_smoothstep(edge0->y, edge1->y, t->y);
    dest->z = fr_smoothstep(edge0->z, edge1->z, t->z);
    dest->w = fr_smoothstep(edge0->w, edge1->w, t->w);
#endif
}

/**
 * @brief Compute a smooth transition between 2 edges with parameter t and store the result in dest. t is clamped
 * between 0 and 1
 * @details for each component of t, the result is 0 if t < edge0, 1 if t > edge1, and the smooth interpolation between
 * 0 and 1 otherwise using a polynomial of degree 3
 * @param edge0
 * @param edge1
 * @param t
 * @param dest
 */
FR_FORCE_INLINE void fr_vec4_smoothstep_clamped(const vec4* edge0, const vec4* edge1, const vec4* t, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = fr_simd_vsmoothstep(edge0->simd, edge1->simd, fr_simd_clamp(t->simd, 0.0f, 1.0f));
#else
    dest->x = fr_smoothstep(edge0->x, edge1->x, fr_clamp(t->x, 0.0f, 1.0f));
    dest->y = fr_smoothstep(edge0->y, edge1->y, fr_clamp(t->y, 0.0f, 1.0f));
    dest->z = fr_smoothstep(edge0->z, edge1->z, fr_clamp(t->z, 0.0f, 1.0f));
    dest->w = fr_smoothstep(edge0->w, edge1->w, fr_clamp(t->w, 0.0f, 1.0f));
#endif
}

/**
 * @brief Compute a smooth transition between 2 edges with parameter t and store the result in dest
 * @details for each component of t, the result is 0 if t < edge0, 1 if t > edge1, and the smooth interpolation between
 * 0 and 1 otherwise using a polynomial of degree 5
 * @param edge0
 * @param edge1
 * @param t
 * @param dest
 */
FR_FORCE_INLINE void fr_vec4_smoothsteps(f32 edge0, f32 edge1, const vec4* t, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = fr_simd_vsmoothstep(_mm_set1_ps(edge0), _mm_set1_ps(edge1), t->simd);
#else
    dest->x = fr_smoothsteps(edge0, edge1, t->x);
    dest->y = fr_smoothsteps(edge0, edge1, t->y);
    dest->z = fr_smoothsteps(edge0, edge1, t->z);
    dest->w = fr_smoothsteps(edge0, edge1, t->w);
#endif
}

/**
 * @brief Compute a smooth transition between 2 edges with parameter t and store the result in dest. t is clamped
 * between 0 and 1
 * @details for each component of t, the result is 0 if t < edge0, 1 if t > edge1, and the smooth interpolation between
 * 0 and 1 otherwise using a polynomial of degree 5
 * @param edge0
 * @param edge1
 * @param t
 * @param dest
 */
FR_FORCE_INLINE void fr_vec4_smoothsteps_clamped(f32 edge0, f32 edge1, const vec4* t, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = fr_simd_vsmoothstep(_mm_set1_ps(edge0), _mm_set1_ps(edge1), fr_simd_clamp(t->simd, 0.0f, 1.0f));
#else
    dest->x = fr_smoothstep(edge0, edge1, fr_clamp(t->x, 0.0f, 1.0f));
    dest->y = fr_smoothstep(edge0, edge1, fr_clamp(t->y, 0.0f, 1.0f));
    dest->z = fr_smoothstep(edge0, edge1, fr_clamp(t->z, 0.0f, 1.0f));
    dest->w = fr_smoothstep(edge0, edge1, fr_clamp(t->w, 0.0f, 1.0f));
#endif
}

/**
 * @brief Generates a linearly interpolated value between 2 vec4s using a parameter t smoothed by a polynomial of degree 3
 * 
 * @param a 
 * @param b 
 * @param t 
 * @param dest 
 */
FR_FORCE_INLINE void fr_vec4_smooth_interp(const vec4* a, const vec4* b, f32 t, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = fr_simd_lerp(a->simd, b->simd, fr_smooth(t));
#else
    dest->x = fr_lerp(a->x, b->x, fr_smooth(t));
    dest->y = fr_lerp(a->y, b->y, fr_smooth(t));
    dest->z = fr_lerp(a->z, b->z, fr_smooth(t));
    dest->w = fr_lerp(a->w, b->w, fr_smooth(t));
#endif
}

/**
 * @brief Projects a onto b and stores the result in dest
 * 
 * @param a 
 * @param b 
 * @param dest 
 */
FR_FORCE_INLINE void fr_vec4_project(const vec4* a, const vec4* b, vec4* dest) {
#if FR_SIMD == 1
    __m128 unit_b = fr_simd_unit_vector(b->simd);
    __m128 dot = fr_simd_vdot(a->simd, b->simd);
    dest->simd = _mm_mul_ps(unit_b, dot);
#else
    f32 dot = fr_vec4_dot(a, b);
    f32 norm2 = fr_vec4_norm2(b);
    if (norm2 == 0.0f) {
        fr_vec4_zero(dest);
        return;
    }
    f32 t = dot / norm2;
    fr_vec4_scale(b, t, dest);
#endif
}

/**
 * @brief Projects a onto b and stores the result in dest
 * @details This function does not check for division by zero for the norm of b
 * 
 * @param a 
 * @param b 
 * @param dest 
 */
FR_FORCE_INLINE void fr_vec4_project_unsafe(const vec4* a, const vec4* b, vec4* dest) {
#if FR_SIMD == 1
    __m128 unit_b = fr_simd_unit_vector_unsafe(b->simd);
    __m128 dot = fr_simd_vdot(a->simd, b->simd);
    dest->simd = _mm_mul_ps(unit_b, dot);
#else
    f32 dot = fr_vec4_dot(a, b);
    f32 norm2 = fr_vec4_norm2(b);
    if (norm2 == 0.0f) {
        fr_vec4_zero(dest);
        return;
    }
    f32 t = dot / norm2;
    fr_vec4_scale(b, t, dest);
#endif
}

/**
 * @brief Reflects the incident vector off the plane defined by the normal vector and stores the result in dest
 * 
 * @param incident 
 * @param normal 
 * @param dest 
 */
FR_FORCE_INLINE void fr_vec4_reflect(const vec4* incident, const vec4* normal, vec4* dest) {
#if FR_SIMD == 1
    __m128 unit_normal = fr_simd_unit_vector(normal->simd);
    dest->simd = fr_simd_reflect(unit_normal, incident->simd);
#else
    vec4 unit_normal;
    fr_vec4_normalize(normal, &unit_normal);
    f32 dot = fr_vec4_dot(incident, &unit_normal);
    fr_vec4_scale(&unit_normal, 2.0f * dot, dest);
    fr_vec4_sub(incident, dest, dest);
#endif
}

/**
 * @brief Reflects the incident vector off the plane defined by the unit normal vector and stores the result in dest
 * 
 * @param incident 
 * @param unit_normal 
 * @param dest 
 */
FR_FORCE_INLINE void fr_vec4_reflect_unit(const vec4* incident, const vec4* unit_normal, vec4* dest) {
#if FR_SIMD == 1
    dest->simd = fr_simd_reflect(unit_normal->simd, incident->simd);
#else
    f32 dot = fr_vec4_dot(incident, unit_normal);
    fr_vec4_scale(unit_normal, 2.0f * dot, dest);
    fr_vec4_sub(incident, dest, dest);
#endif
}

/**
 * @brief Sphereical linear interpolation between 2 vec4s with parameter t and store the result in dest
 * 
 * @param a 
 * @param b 
 * @param t 
 * @param dest 
 */
FR_FORCE_INLINE void fr_vec4_slerp(const vec4* a, const vec4* b, f32 t, vec4* dest) {
#if FR_SIMD == 1
    __m128 const dot = fr_simd_vdot(a->simd, b->simd);
    f32 const angle = fr_acos(_mm_cvtss_f32(dot));
    f32 const inv_sin_angle = 1.0f / fr_sin(angle);
    f32 const a_coeff = fr_sin((1.0f - t) * angle) * inv_sin_angle;
    f32 const b_coeff = fr_sin(t * angle) * inv_sin_angle;
    __m128 a_scaled = _mm_mul_ps(a->simd, _mm_set1_ps(a_coeff));
    __m128 b_scaled = _mm_mul_ps(b->simd, _mm_set1_ps(b_coeff));
    dest->simd = _mm_add_ps(a_scaled, b_scaled);
#else
    f32 const dot = fr_vec4_dot(a, b);
    f32 const angle = fr_acos(dot);
    f32 const inv_sin_angle = 1.0f / fr_sin(angle);
    f32 const a_coeff = fr_sin((1.0f - t) * angle) * inv_sin_angle;
    f32 const b_coeff = fr_sin(t * angle) * inv_sin_angle;
    fr_vec4_scale(a, a_coeff, dest);
    vec4 b_scaled;
    fr_vec4_scale(b, b_coeff, &b_scaled);
    fr_vec4_add(dest, &b_scaled, dest);
#endif
}
