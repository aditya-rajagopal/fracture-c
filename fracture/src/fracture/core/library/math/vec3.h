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

#include <stdatomic.h>
#include "detail/vec3.h"
#include "fracture/fracture_core.h"
#include "math_constants.h"
#include "utils.h"


// ---------------------------------------------------------
// ------------------- Vec3 Constructors -------------------
// ---------------------------------------------------------

/**
 * @brief Construct a new vec3 object from an array of floats.
 * @details The array must have at least 3 elements and if you give the same
 * pointer for src and dst, the function will have undefined behavior.
 *
 * @param src The source array of floats.
 * @param dst The destination vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_from_array(const f32* restrict src,
                                               vec3* restrict dst) {
    dst->x = src[0];
    dst->y = src[1];
    dst->z = src[2];
}

/**
 * @brief Convert a vec3 object to an array of floats.
 * @details The array must have at least 3 elements and if you give the same
 * pointer for src and dst, the function will have undefined behavior.
 *
 * @param src The source vec3 object.
 * @param dst The destination array of floats.
 */
FR_API FR_FORCE_INLINE void fr_vec3_to_array(const vec3* restrict src,
                                             f32* restrict dst) {
    dst[0] = src->x;
    dst[1] = src->y;
    dst[2] = src->z;
}

/**
 * @brief Construct a new vec3 object from two float values.
 *
 * @param x The x component of the vec3 object.
 * @param y The y component of the vec3 object.
 * @param z The z component of the vec3 object.
 * @return vec3 The constructed vec3 object.
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_new(f32 x, f32 y, f32 z) {
    return (vec3){.x = x, .y = y, .z = z};
}

/**
 * @brief Construct a new vec3 object from a single float value.
 *
 * @param val The value to set the x and y components of the vec3 object to.
 * @return vec3 The constructed vec3 object.
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_new_scalar(f32 val) {
    return (vec3){.x = val, .y = val, .z = val};
}

/**
 * @brief Construct a new vec3 object from an array of floats.
 *
 * @param arr The array of floats to construct the vec3 object from.
 * @return vec3 The constructed vec3 object.
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_new_array(f32* arr) {
    return (vec3){.x = arr[0], .y = arr[1], .z = arr[2]};
}

/**
 * @brief Construct a new vec3 object from a vec3 object.
 *
 * @param src The source vec3 object.
 * @return vec3 The constructed vec3 object.
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_copy(const vec3* src) {
    return (vec3){.x = src->x, .y = src->y, .z = src->z};
}

/**
 * @brief Copy the values of a vec3 object to another vec3 object.
 * @details If you give the same pointer for src and dest, the function will
 * have undefined behavior.
 *
 * @param src The source vec3 object.
 * @param dest The destination vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_copy_to(const vec3* restrict src,
                                            vec3* restrict dest) {
    dest->x = src->x;
    dest->y = src->y;
    dest->z = src->z;
}

/**
 * @brief Get a vec3 object with all components set to zero.
 *
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_zero() {
    return (vec3){.x = 0.0f, .y = 0.0f, .z = 0.0f};
}

/**
 * @brief Set all components of a vec3 object to zero.
 *
 * @param dest The destination vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_make_zero(vec3* dest) {
    dest->x = 0.0f;
    dest->y = 0.0f;
    dest->z = 0.0f;
}

/**
 * @brief Get a vec3 object with all components set to one.
 *
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_one() {
    return (vec3){.x = 1.0f, .y = 1.0f, .z = 1.0f};
}

/**
 * @brief Set all components of a vec3 object to one.
 *
 * @param dest The destination vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_make_one(vec3* dest) {
    dest->x = 1.0f;
    dest->y = 1.0f;
    dest->z = 1.0f;
}

/**
 * @brief Get a vec3 object with (-1, 0, 0).
 *
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_left() {
    return (vec3){.x = -1.0f, .y = 0.0f, .z = 0.0f};
}

/**
 * @brief Set the components of a vec3 object to (-1, 0, 0)
 *
 * @param dest The destination vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_make_left(vec3* dest) {
    dest->x = -1.0f;
    dest->y = 0.0f;
    dest->z = 0.0f;
}

/**
 * @brief Get a vec3 object with (1, 0, 0).
 *
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_right() {
    return (vec3){.x = 1.0f, .y = 0.0f, .z = 0.0f};
}

/**
 * @brief Set the components of a vec3 object to (1, 0, 0).
 *
 * @param dest The destination vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_make_right(vec3* v, vec3* dest) {
    dest->x = 1.0f;
    dest->y = 0.0f;
    dest->z = 0.0f;
}

/**
 * @brief Get a vec3 object with (0, 1, 0).
 *
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_up() {
    return (vec3){.x = 0.0f, .y = 1.0f, .z = 0.0f};
}

/**
 * @brief Set the components of a vec3 object to  (0, 1, 0).
 *
 * @param dest The destination vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_make_up(vec3* dest) {
    dest->x = 0.0f;
    dest->y = 1.0f;
    dest->z = 0.0f;
}

/**
 * @brief Get a vec3 object components (0, -1, 0).
 *
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_down() {
    return (vec3){.x = 0.0f, .y = -1.0f, .z = 0.0f};
}

/**
 * @brief Set the components of a vec3 object to (0, -1, 0).
 *
 * @param dest The destination vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_make_down(vec3* dest) {
    dest->x = 0.0f;
    dest->y = -1.0f;
    dest->z = 0.0f;
}

/**
 * @brief Get a vec3 object with (0, 0, 1).
 *
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_forward() {
    return (vec3){.x = 0.0f, .y = 0.0f, .z = 1.0f};
}

/**
 * @brief Set the components of a vec3 object to (0, 0, 1).
 *
 * @param dest The destination vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_make_forward(vec3* dest) {
    dest->x = 0.0f;
    dest->y = 0.0f;
    dest->z = 1.0f;
}

/**
 * @brief Get a vec3 object with (0, 0, -1).
 *
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_backward() {
    return (vec3){.x = 0.0f, .y = 0.0f, .z = -1.0f};
}

/**
 * @brief Set the components of a vec3 object to (0, 0, -1).
 *
 * @param dest The destination vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_make_backward(vec3* dest) {
    dest->x = 0.0f;
    dest->y = 0.0f;
    dest->z = -1.0f;
}

/**
 * @brief Set all components of a vec3 object to a single float value
 *
 * @param dest The destination vec3 object.
 * @param val The value to set the components to.
 */
FR_API FR_FORCE_INLINE void fr_vec3_fill(vec3* dest, f32 val) {
    dest->x = val;
    dest->y = val;
    dest->z = val;
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
FR_API FR_FORCE_INLINE b8 fr_vec3_eq_scalar(const vec3* v, f32 val) {
    return fr_equal(v->x, val) && fr_equal(v->y, val) && fr_equal(v->z, val);
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
FR_API FR_FORCE_INLINE b8 fr_vec3_eq_scalar_threshold(const vec3* v, f32 val,
                                                      f32 threshold) {
    return fr_equal_threshold(v->x, val, threshold) &&
           fr_equal_threshold(v->y, val, threshold) &&
           fr_equal_threshold(v->z, val, threshold);
}

/**
 * @brief Check if all components of a vec3 object are equal to each other.
 *
 * @param v The vec3 object to check.
 * @return b8 True if all components are equal to each other, false otherwise.
 */
FR_API FR_FORCE_INLINE b8 fr_vec3_eq_all(const vec3* v) {
    return fr_equal(v->x, v->y) && fr_equal(v->y, v->z);
}

/**
 * @brief Check if two vectors are equal to each other
 *
 * @param v0
 * @param v1
 * @return b8 TRUE if the vectors are equal, FALSE otherwise.
 */
FR_API FR_FORCE_INLINE b8 fr_vec3_eq(const vec3* v0, const vec3* v1) {
    return fr_equal(v0->x, v1->x) && fr_equal(v0->y, v1->y) && fr_equal(v0->z, v1->z);
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
FR_API FR_FORCE_INLINE b8 fr_vec3_eq_threshold(const vec3* v0, const vec3* v1,
                                               f32 threshold) {
    return fr_equal_threshold(v0->x, v1->x, threshold) &&
           fr_equal_threshold(v0->y, v1->y, threshold) &&
           fr_equal_threshold(v0->z, v1->z, threshold);
}

/**
 * @brief Get the maximum component of a vec3 object.
 *
 * @param v The vec3 object to check.
 * @return f32 The maximum component of the vec3 object.
 */
FR_API FR_FORCE_INLINE f32 fr_vec3_max(const vec3* v) {
    return fr_max(fr_max(v->x, v->y), v->z);
}

/**
 * @brief Get the minimum component of a vec3 object.
 *
 * @param v The vec3 object to check.
 * @return f32 The minimum component of the vec3 object.
 */
FR_API FR_FORCE_INLINE f32 fr_vec3_min(const vec3* v) {
    return fr_min(fr_min(v->x, v->y), v->z);
}

/**
 * @brief Check if all the components of a vec3 are zero
 *
 * @param v The vec3 object to check.
 * @return b8 TRUE if all components are zero, FALSE otherwise.
 */
FR_API FR_FORCE_INLINE b8 fr_vec3_iszero(const vec3* v) {
    return fr_vec3_eq_scalar(v, 0.0f);
}

/**
 * @brief Get a vec3 object with the sign of each component of the input vec3
 * object.
 *
 * @param v The input vec3 object.
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_sign(const vec3* v) {
    return fr_vec3_new(fr_sign(v->x), fr_sign(v->y), fr_sign(v->z));
}

/**
 * @brief Get a vec3 object with the absolute value of each component of the
 * input vec3 object.
 *
 * @param v The input vec3 object.
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_abs(const vec3* v) {
    return fr_vec3_new(fr_abs(v->x), fr_abs(v->y), fr_abs(v->z));
}

/**
 * @brief Set the components of a vec3 object to their absolute values.
 *
 * @param v The input vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_make_abs(vec3* v) {
    v->x = fr_abs(v->x);
    v->y = fr_abs(v->y);
    v->z = fr_abs(v->z);
}

/**
 * @brief Get a vec3 object with the square root of each component of the input
 * vec3 object.
 *
 * @param v The input vec3 object.
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_sqrt(const vec3* v) {
    return fr_vec3_new(fr_sqrt(v->x), fr_sqrt(v->y), fr_sqrt(v->z));
}

/**
 * @brief Set the components of a vec3 object to their square roots.
 *
 * @param v The input vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_make_sqrt(vec3* v) {
    v->x = fr_sqrt(v->x);
    v->y = fr_sqrt(v->y);
    v->z = fr_sqrt(v->z);
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
FR_API FR_FORCE_INLINE f32 fr_vec3_dot(const vec3* v0, const vec3* v1) {
    return v0->x * v1->x + v0->y * v1->y + v0->z * v1->z;
}

/**
 * @brief Get the cross product of two vec3 objects.
 *
 * @param v0
 * @param v1
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_cross(const vec3* v0, const vec3* v1) {
    return fr_vec3_new(v0->y * v1->z - v0->z * v1->y, v0->z * v1->x - v0->x * v1->z,
                       v0->x * v1->y - v0->y * v1->x);
}
FR_API FR_FORCE_INLINE void fr_vec3_cross_to(const vec3* v0, const vec3* v1, vec3* dest) {
    dest->x = v0->y * v1->z - v0->z * v1->y;
    dest->y = v0->z * v1->x - v0->x * v1->z;
    dest->z = v0->x * v1->y - v0->y * v1->x;
}


/**
 * @brief Get he squared norm2 of a vec3 object.
 *
 * @param v
 * @return f32
 */
FR_API FR_FORCE_INLINE f32 fr_vec3_norm2(const vec3* v) {
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

/**
 * @brief Get the norm of a vec3 object.
 *
 * @param v
 * @return f32
 */
FR_API FR_FORCE_INLINE f32 fr_vec3_norm(const vec3* v) {
    return fr_sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

/**
 * @brief L1 norm of a vec3 object.
 *
 * @param v
 * @return f32
 */
FR_API FR_FORCE_INLINE f32 fr_vec3_norm1(const vec3* v) {
    return fr_abs(v->x) + fr_abs(v->y) + fr_abs(v->z);
}

/**
 * @brief L-infinity norm of a vec3 object.
 *
 * @param v
 * @return f32
 */
FR_API FR_FORCE_INLINE f32 fr_vec3_norm_inf(const vec3* v) {
    return fr_max(fr_max(fr_abs(v->x), fr_abs(v->y)), fr_abs(v->z));
}

/**
 * @brief Add two vec3 objects together.
 *
 * @param v0
 * @param v1
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_add(const vec3* v0, const vec3* v1) {
    return fr_vec3_new(v0->x + v1->x, v0->y + v1->y, v0->z + v1->z);
}

/**
 * @brief Add two vec3 objects together and store the result in a third vec3
 * object.
 *
 * @param v0
 * @param v1
 * @param dest pointer to the destination vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_add_to(const vec3* v0, const vec3* v1, vec3* dest) {
    dest->x = v0->x + v1->x;
    dest->y = v0->y + v1->y;
    dest->z = v0->z + v1->z;
}

/**
 * @brief Add a scalar value to a vec3 object.
 *
 * @param v
 * @param s
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_adds(const vec3* v, f32 s) {
    return fr_vec3_new(v->x + s, v->y + s, v->z + s);
}

/**
 * @brief Add a scalar value to a vec3 object and store the result in a third
 * vec3 object.
 *
 * @param v
 * @param s
 * @param dest pointer to the destination vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_adds_to(const vec3* v, f32 s, vec3* dest) {
    dest->x = v->x + s;
    dest->y = v->y + s;
    dest->z = v->z + s;
}

/**
 * @brief Subtract two vec3 objects v0 - v1.
 *
 * @param v0
 * @param v1
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_sub(const vec3* v0, const vec3* v1) {
    return fr_vec3_new(v0->x - v1->x, v0->y - v1->y, v0->z - v1->z);
}

/**
 * @brief Subtract two vec3 objects v0 - v1 and store the result in a third vec3
 * object.
 *
 * @param v0
 * @param v1
 * @param dest pointer to the destination vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_sub_to(const vec3* v0, const vec3* v1, vec3* dest) {
    dest->x = v0->x - v1->x;
    dest->y = v0->y - v1->y;
    dest->z = v0->z - v1->z;
}

/**
 * @brief Subtract a scalar value from a vec3 object.
 *
 * @param v
 * @param s
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_subs(const vec3* v, f32 s) {
    return fr_vec3_new(v->x - s, v->y - s, v->z - s);
}

/**
 * @brief Subtract a scalar value from a vec3 object and store the result in a
 * third vec3 object.
 *
 * @param v
 * @param s
 * @param dest pointer to the destination vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_subs_to(const vec3* v, f32 s, vec3* dest) {
    dest->x = v->x - s;
    dest->y = v->y - s;
    dest->z = v->z - s;
}

/**
 * @brief Elementwise multiply two vec3 objects together.
 *
 * @param v0
 * @param v1
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_mul(const vec3* v0, const vec3* v1) {
    return fr_vec3_new(v0->x * v1->x, v0->y * v1->y, v0->z * v1->z);
}

/**
 * @brief Elementwise multiply two vec3 objects together and store the result in
 * a third vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest pointer to the destination vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_mul_to(const vec3* v0, const vec3* v1, vec3* dest) {
    dest->x = v0->x * v1->x;
    dest->y = v0->y * v1->y;
    dest->z = v0->z * v1->z;
}

/**
 * @brief Multiply a vec3 object by a scalar value.
 *
 * @param v
 * @param s
 */
FR_API FR_FORCE_INLINE void fr_vec3_scale(vec3* v, f32 s) {
    v->x *= s;
    v->y *= s;
    v->z *= s;
}

/**
 * @brief Scale a vec3 by s and return a new vec3.
 *
 * @param v
 * @param s
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_get_scaled(const vec3* v, f32 s) {
    return fr_vec3_new(v->x * s, v->y * s, v->z * s);
}

/**
 * @brief Scale a vec3 by s and store the result in a third vec3 object.
 *
 * @param v
 * @param s
 * @param dest pointer to the destination vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_scale_to(const vec3* v, f32 s, vec3* dest) {
    dest->x = v->x * s;
    dest->y = v->y * s;
    dest->z = v->z * s;
}

/**
 * @brief Scale the unit vector in the direction of v by s and store the result
 * in the destination.
 *
 * @param v
 * @param s
 * @param dest
 */
FR_API FR_FORCE_INLINE void fr_vec3_scale_direction_to(const vec3* v, f32 s,
                                                       vec3* dest) {
    f32 const norm = fr_vec3_norm(v);
    if (norm == 0.0f) {
        dest->x = 0.0f;
        dest->y = 0.0f;
        dest->z = 0.0f;
        return;
    }
    fr_vec3_scale_to(v, s / norm, dest);
}

/**
 * @brief Divide two vec3 objects elementwise.
 * @details If any component of the divisor is zero the function will return NaN
 * components.
 * @param v0
 * @param v1
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_div(const vec3* v0, const vec3* v1) {
    return fr_vec3_new(v0->x / v1->x, v0->y / v1->y, v0->z / v1->z);
}

/**
 * @brief Divide two vec3 objects elementwise and store the result in a third
 * vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest pointer to the destination vec3 object.
 */
FR_API FR_FORCE_INLINE void fr_vec3_div_to(const vec3* v0, const vec3* v1, vec3* dest) {
    dest->x = v0->x / v1->x;
    dest->y = v0->y / v1->y;
    dest->z = v0->z / v1->z;
}

/**
 * @brief Divide a vec3 object by a scalar value.
 * @details If the scalar value is zero the function will return a vec3 with all
 * NaN components.
 *
 * @param v
 * @param s
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_divs(const vec3* v, f32 s) {
    return fr_vec3_new(v->x / s, v->y / s, v->z / s);
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
FR_API FR_FORCE_INLINE void fr_vec3_divs_to(const vec3* v, f32 s, vec3* dest) {
    dest->x = v->x / s;
    dest->y = v->y / s;
    dest->z = v->z / s;
}

/**
 * @brief Add two vec3 objects together and store the result in a third vec3
 * object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_API FR_FORCE_INLINE void fr_vec3_addadd(const vec3* v0, const vec3* v1, vec3* dest) {
    dest->x += v0->x + v1->x;
    dest->y += v0->y + v1->y;
    dest->z += v0->z + v1->z;
}

/**
 * @brief Subtract two vec3 objects v0 - v1 and add the result to the third vec3
 * object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_API FR_FORCE_INLINE void fr_vec3_subadd(const vec3* v0, const vec3* v1, vec3* dest) {
    dest->x += v0->x - v1->x;
    dest->y += v0->y - v1->y;
    dest->z += v0->z - v1->z;
}

/**
 * @brief elementwise multiply two vec3 objects together and add the result to
 * third vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_API FR_FORCE_INLINE void fr_vec3_muladd(const vec3* v0, const vec3* v1, vec3* dest) {
    dest->x += v0->x * v1->x;
    dest->y += v0->y * v1->y;
    dest->z += v0->z * v1->z;
}

/**
 * @brief Multiply a vec3 object by a scalar value and add the result to a third
 * vec3 object.
 *
 * @param v
 * @param s
 * @param dest
 */
FR_API FR_FORCE_INLINE void fr_vec3_muladds(const vec3* v, f32 s, vec3* dest) {
    dest->x += v->x * s;
    dest->y += v->y * s;
    dest->z += v->z * s;
}

/**
 * @brief Add the maximum components of two vec3 objects to a third vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_API FR_FORCE_INLINE void fr_vec3_maxadd(const vec3* v0, const vec3* v1, vec3* dest) {
    dest->x += fr_max(v0->x, v1->x);
    dest->y += fr_max(v0->y, v1->y);
    dest->z += fr_max(v0->z, v1->z);
}

/**
 * @brief Add the minimum components of two vec3 objects to a third vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_API FR_FORCE_INLINE void fr_vec3_minadd(const vec3* v0, const vec3* v1, vec3* dest) {
    dest->x += fr_min(v0->x, v1->x);
    dest->y += fr_min(v0->y, v1->y);
    dest->z += fr_min(v0->z, v1->z);
}

/**
 * @brief Subtract two vec3 objects v0 - v1 and subtract the result from the
 * third vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_API FR_FORCE_INLINE void fr_vec3_subsub(const vec3* v0, const vec3* v1, vec3* dest) {
    dest->x -= v0->x - v1->x;
    dest->y -= v0->y - v1->y;
    dest->z -= v0->z - v1->z;
}

/**
 * @brief Add two vec3 objects together and subtract the result from the third
 * vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_API FR_FORCE_INLINE void fr_vec3_addsub(const vec3* v0, const vec3* v1, vec3* dest) {
    dest->x -= v0->x + v1->x;
    dest->y -= v0->y + v1->y;
    dest->z -= v0->z + v1->z;
}

/**
 * @brief Elementwise multiply two vec3 objects together and subtract the result
 * from the third vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_API FR_FORCE_INLINE void fr_vec3_mulsub(const vec3* v0, const vec3* v1, vec3* dest) {
    dest->x -= v0->x * v1->x;
    dest->y -= v0->y * v1->y;
    dest->z -= v0->z * v1->z;
}

/**
 * @brief Multiply a vec3 object by a scalar value and subtract the result from
 * a third vec3 object.
 *
 * @param v
 * @param s
 * @param dest
 */
FR_API FR_FORCE_INLINE void fr_vec3_mulsubs(const vec3* v, f32 s, vec3* dest) {
    dest->x -= v->x * s;
    dest->y -= v->y * s;
    dest->z -= v->z * s;
}

/**
 * @brief Add the maximum components of two vec3 objects to a third vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_API FR_FORCE_INLINE void fr_vec3_maxsub(const vec3* v0, const vec3* v1, vec3* dest) {
    dest->x -= fr_max(v0->x, v1->x);
    dest->y -= fr_max(v0->y, v1->y);
    dest->z -= fr_max(v0->z, v1->z);
}

/**
 * @brief Add the minimum components of two vec3 objects to a third vec3 object.
 *
 * @param v0
 * @param v1
 * @param dest
 */
FR_API FR_FORCE_INLINE void fr_vec3_minsub(const vec3* v0, const vec3* v1, vec3* dest) {
    dest->x -= fr_min(v0->x, v1->x);
    dest->y -= fr_min(v0->y, v1->y);
    dest->z -= fr_min(v0->z, v1->z);
}

/**
 * @brief Negate a vec3 object.
 *
 * @param v
 */
FR_API FR_FORCE_INLINE void fr_vec3_negate(vec3* v) {
    v->x = -v->x;
    v->y = -v->y;
    v->z = -v->z;
}

/**
 * @brief Get the negative of a vec3 object.
 *
 * @param v
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_get_negative(const vec3* v) {
    return fr_vec3_new(-v->x, -v->y, -v->z);
}

/**
 * @brief Negate a vec3 object and store the result in an another vec3 object.
 *
 * @param v
 * @param dest
 */
FR_API FR_FORCE_INLINE void fr_vec3_negate_to(const vec3* v, vec3* dest) {
    dest->x = -v->x;
    dest->y = -v->y;
    dest->z = -v->z;
}

/**
 * @brief Normalize a vec3 object.
 *
 * @param v
 */
FR_API FR_FORCE_INLINE void fr_vec3_normalize(vec3* v) {
    f32 const norm = fr_vec3_norm(v);
    if (norm == 0.0f) {
        fr_vec3_make_zero(v);
    }
    fr_vec3_scale(v, 1 / norm);
}

/**
 * @brief Get the normalized version of a vec3 object.
 *
 * @param v
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_get_unit(const vec3* v) {
    f32 const norm = fr_vec3_norm(v);
    if (norm == 0.0f) {
        return fr_vec3_zero();
    }
    return fr_vec3_get_scaled(v, 1 / norm);
}

/**
 * @brief Get a vec3 in the direction of v scaled by s.
 *
 * @param v
 * @param s
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_scale_direction(const vec3* v, f32 s) {
    f32 const norm = fr_vec3_norm(v);
    if (norm == 0.0f) {
        return fr_vec3_zero();
    }
    return fr_vec3_get_scaled(v, s / norm);
}

/**
 * @brief Get a vec3 with v rotated by angle.
 *
 * @param v
 * @param angle angle in radians
 * @param axis the axis to rotate around
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_get_rotated(const vec3* v, f32 angle, const vec3* axis) {
    f32 const c = fr_cos(angle);
    f32 const s = fr_sin(angle);
    vec3 temp1, temp2;
    temp1 = fr_vec3_get_unit(axis);
    temp2 = fr_vec3_cross(&temp1, v);
    vec3 const scaled_cross = fr_vec3_get_scaled(&temp2, s);
    vec3 const scaled_other = fr_vec3_get_scaled(&temp2, fr_vec3_dot(&temp1, v) * (1.0f - c));
    fr_vec3_scale_to(v, c, &temp1);
    fr_vec3_add_to(&scaled_cross, &scaled_other, &temp2);
    // Rodrigues' rotation formula
    // v_rot = v * cos(angle) + (axis x v) * sin(angle) + axis * (axis . v) * (1- cos(angle))
    return fr_vec3_add(&temp1, &temp2);
}

/**
 * @brief Rotate a v by angle.
 *
 * @param v
 * @param angle angle in radians
 */
FR_API FR_FORCE_INLINE void fr_vec3_rotate(vec3* v, f32 angle, const vec3* axis) {
    *v = fr_vec3_get_rotated(v, angle, axis);
}

/**
 * @brief Get the angle between two vec3 objects.
 *
 * @param v0
 * @param v1
 * @return f32
 */
FR_API FR_FORCE_INLINE f32 fr_vec3_angle_between(const vec3* v0, const vec3* v1) {
    f32 const dot = fr_vec3_dot(v0, v1);
    vec3 const cross = fr_vec3_cross(v0, v1);
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
FR_API FR_FORCE_INLINE void fr_vec3_project(vec3* v, vec3* onto, vec3* dest) {
    f32 const dot = fr_vec3_dot(v, onto);
    fr_vec3_scale_direction_to(onto, dot, dest);
}

/**
 * @brief Get the midpoint between two vec3
 *
 * @param v0
 * @param v1
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_center(const vec3* v0, const vec3* v1) {
    return fr_vec3_new((v0->x + v1->x) * 0.5f, (v0->y + v1->y) * 0.5f,
                       (v0->z + v1->z) * 0.5f);
}

/**
 * @brief Get the distance squared between two vec3
 *
 * @param v0
 * @param v1
 * @return f32
 */
FR_API FR_FORCE_INLINE f32 fr_vec3_distance2(const vec3* v0, const vec3* v1) {
    f32 const dx = v0->x - v1->x;
    f32 const dy = v0->y - v1->y;
    f32 const dz = v0->z - v1->z;
    return dx * dx + dy * dy + dz * dz;
}

/**
 * @brief Get the distance between two vec3
 *
 * @param v0
 * @param v1
 * @return f32
 */
FR_API FR_FORCE_INLINE f32 fr_vec3_distance(const vec3* v0, const vec3* v1) {
    f32 const dx = v0->x - v1->x;
    f32 const dy = v0->y - v1->y;
    f32 const dz = v0->z - v1->z;
    return fr_sqrt(dx * dx + dy * dy + dz * dz);
}

/**
 * @brief Get a vec3 with the maximum components of two vec3 objects
 *
 * @param v1
 * @param v2
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_maxv(const vec3* v1, const vec3* v2) {
    return fr_vec3_new(fr_max(v1->x, v2->x), fr_max(v1->y, v2->y),
                       fr_max(v1->z, v2->z));
}

/**
 * @brief Get a vec3 with the minimum components of two vec3 objects
 *
 * @param v1
 * @param v2
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_minv(const vec3* v1, const vec3* v2) {
    return fr_vec3_new(fr_min(v1->x, v2->x), fr_min(v1->y, v2->y),
                       fr_min(v1->z, v2->z));
}

/**
 * @brief Clamp the components of a vec3 object between minVal and maxVal.
 *
 * @param v
 * @param minVal
 * @param maxVal
 */
FR_API FR_FORCE_INLINE void fr_vec3_clamp(vec3* v, f32 minVal, f32 maxVal) {
    v->x = fr_clamp(v->x, minVal, maxVal);
    v->y = fr_clamp(v->y, minVal, maxVal);
    v->z = fr_clamp(v->z, minVal, maxVal);
}

/**
 * @brief Get a vec3 with the components of a vec3 object clamped between minVal
 * and maxVal.
 *
 * @param v
 * @param minVal
 * @param maxVal
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_get_clamped(const vec3* v, f32 minVal,
                                                f32 maxVal) {
    return fr_vec3_new(fr_clamp(v->x, minVal, maxVal),
                       fr_clamp(v->y, minVal, maxVal),
                       fr_clamp(v->z, minVal, maxVal));
}

/**
 * @brief Linearly interpolate between two vec3 objects using t
 *
 * @param from
 * @param to
 * @param t
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_lerp(const vec3* from, const vec3* to, f32 t) {
    return fr_vec3_new(from->x + (to->x - from->x) * t,
                       from->y + (to->y - from->y) * t,
                       from->z + (to->z - from->z) * t);
}
/**
 * @brief Linearly interpolate between two vec3 objects using t and store the
 * result in a third vec3 object.
 *
 * @param from
 * @param to
 * @param t
 * @param dest
 */
FR_API FR_FORCE_INLINE void fr_vec3_lerp_to(const vec3* from, const vec3* to, f32 t,
                                            vec3* dest) {
    dest->x = from->x + (to->x - from->x) * t;
    dest->y = from->y + (to->y - from->y) * t;
    dest->z = from->z + (to->z - from->z) * t;
}

/**
 * @brief smoothstep threshold function with a smooth transition between 0
 * and 1.
 *
 * @param edge0
 * @param edge1
 * @param x the input value
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_smoothstep(const vec3* edge0, const vec3* edge1, f32 x) {
    return fr_vec3_new(fr_smoothstep(edge0->x, edge1->x, x),
                       fr_smoothstep(edge0->y, edge1->y, x),
                       fr_smoothstep(edge0->z, edge1->z, x));
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
FR_API FR_FORCE_INLINE void fr_vec3_smoothstep_to(const vec3* edge0, const vec3* edge1, f32 x,
                                                  vec3* dest) {
    dest->x = fr_smoothstep(edge0->x, edge1->x, x);
    dest->y = fr_smoothstep(edge0->y, edge1->y, x);
    dest->z = fr_smoothstep(edge0->z, edge1->z, x);
}

/**
 * @brief smooth interpolation between two vec3 objects.
 *
 * @param from
 * @param to
 * @param t
 * @return vec3
 */
FR_API FR_FORCE_INLINE vec3 fr_vec3_smooth_interpolation(const vec3* from, const vec3* to,
                                                         f32 t) {
    return fr_vec3_new(fr_smoothinterp(from->x, to->y, t),
                       fr_smoothinterp(from->y, to->y, t),
                       fr_smoothinterp(from->z, to->z, t));
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
FR_API FR_FORCE_INLINE void fr_vec3_smooth_interpolation_to(const vec3* from, const vec3* to,
                                                            f32 t, vec3* dest) {
    dest->x = fr_smoothinterp(from->x, to->x, t);
    dest->y = fr_smoothinterp(from->y, to->y, t);
    dest->z = fr_smoothinterp(from->z, to->z, t);
}