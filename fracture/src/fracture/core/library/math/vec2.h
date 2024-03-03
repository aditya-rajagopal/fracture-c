/**
 * @file vec2.h
 * @author Aditya Rajagopal
 * @brief Vector 2D math functions and utilities.
 * @version 0.0.1
 * @date 2024-02-29
 *
 * @copyright Fracture Game Engine is Copyright (c); Aditya Rajagopal 2024-2024
 *
 */
#pragma once

#include "detail/vec2.h"
#include "math_constants.h"
#include "fracture/fracture_core.h"
#include "utils.h"


// ---------------------------------------------
// -------------- Vec2 constructors ------------
// ---------------------------------------------

/**
 * @brief Construct a new vec2 object from an array of floats.
 * @details The array must have at least 2 elements and if you give the same
 * pointer for src and dst, the function will have undefined behavior.
 *
 * @param src The source array of floats.
 * @param dst The destination vec2 object.
 */
FR_API FR_FORCE_INLINE void fr_vec2_from_array(const f32* restrict src,
                                               vec2* restrict dst) {
    dst->x = src[0];
    dst->y = src[1];
}

/**
 * @brief Convert a vec2 object to an array of floats.
 * @details The array must have at least 2 elements and if you give the same
 * pointer for src and dst, the function will have undefined behavior.
 *
 * @param src The source vec2 object.
 * @param dst The destination array of floats.
 */
FR_API FR_FORCE_INLINE void fr_vec2_to_array(const vec2* restrict src,
                                             f32* restrict dst) {
    dst[0] = src->x;
    dst[1] = src->y;
}

/**
 * @brief Construct a new vec2 object from two float values.
 *
 * @param x The x component of the vec2 object.
 * @param y The y component of the vec2 object.
 * @return vec2 The constructed vec2 object.
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_new(f32 x, f32 y) {
    return (vec2){.x = x, .y = y};
}

/**
 * @brief Construct a new vec2 object from a single float value.
 *
 * @param val The value to set the x and y components of the vec2 object to.
 * @return vec2 The constructed vec2 object.
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_new_scalar(f32 val) {
    return (vec2){.x = val, .y = val};
}

/**
 * @brief Construct a new vec2 object from an array of floats.
 *
 * @param arr The array of floats to construct the vec2 object from.
 * @return vec2 The constructed vec2 object.
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_new_array(f32* arr) {
    return (vec2){.x = arr[0], .y = arr[1]};
}

/**
 * @brief Construct a new vec2 object from a vec2 object.
 *
 * @param src The source vec2 object.
 * @return vec2 The constructed vec2 object.
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_copy(const vec2* src) {
    return (vec2){.x = src->x, .y = src->y};
}

/**
 * @brief Copy the values of a vec2 object to another vec2 object.
 * @details If you give the same pointer for src and dest, the function will
 * have undefined behavior.
 *
 * @param src The source vec2 object.
 * @param dest The destination vec2 object.
 */
FR_API FR_FORCE_INLINE void fr_vec2_copy_to(const vec2* restrict src, vec2* restrict dest) {
    dest->x = src->x;
    dest->y = src->y;
}

/**
 * @brief Get a vec2 object with all components set to zero.
 * 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_zero() {
    return (vec2){.x = 0.0f, .y = 0.0f};
}

/**
 * @brief Set all components of a vec2 object to zero.
 * 
 * @param dest The destination vec2 object.
 */
FR_API FR_FORCE_INLINE void fr_vec2_make_zero(vec2* dest) {
    dest->x = 0.0f;
    dest->y = 0.0f;
}

/**
 * @brief Get a vec2 object with all components set to one.
 * 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_one() {
    return (vec2){.x = 1.0f, .y = 1.0f};
}

/**
 * @brief Set all components of a vec2 object to one.
 * 
 * @param dest The destination vec2 object.
 */
FR_API FR_FORCE_INLINE void fr_vec2_make_one(vec2* dest) {
    dest->x = 1.0f;
    dest->y = 1.0f;
}

/**
 * @brief Get a vec2 object with the x component set to -1 and the y component set to 0.
 * 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_left() {
    return (vec2){.x = -1.0f, .y = 0.0f};
}

/**
 * @brief Set the components of a vec2 object to -1 and 0.
 * 
 * @param dest The destination vec2 object.
 */
FR_API FR_FORCE_INLINE void fr_vec2_make_left(vec2* dest) {
    dest->x = -1.0f;
    dest->y = 0.0f;
}

/**
 * @brief Get a vec2 object with the x component set to 1 and the y component set to 0.
 * 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_right() {
    return (vec2){.x = 1.0f, .y = 0.0f};
}

/**
 * @brief Set the components of a vec2 object to 1 and 0.
 * 
 * @param dest The destination vec2 object.
 */
FR_API FR_FORCE_INLINE void fr_vec2_make_right(vec2* v, vec2* dest) {
    dest->x = 1.0f;
    dest->y = 0.0f;
}

/**
 * @brief Get a vec2 object with the x component set to 0 and the y component set to 1.
 * 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_up() {
    return (vec2){.x = 0.0f, .y = 1.0f};
}

/**
 * @brief Set the components of a vec2 object to 0 and 1.
 * 
 * @param dest The destination vec2 object.
 */
FR_API FR_FORCE_INLINE void fr_vec2_make_up(vec2* dest) {
    dest->x = 0.0f;
    dest->y = 1.0f;
}

/**
 * @brief Get a vec2 object with the x component set to 0 and the y component set to -1.
 * 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_down() {
    return (vec2){.x = 0.0f, .y = -1.0f};
}

/**
 * @brief Set the components of a vec2 object to 0 and -1.
 * 
 * @param dest The destination vec2 object.
 */
FR_API FR_FORCE_INLINE void fr_vec2_make_down(vec2* dest) {
    dest->x = 0.0f;
    dest->y = -1.0f;
}

/**
 * @brief Set all components of a vec2 object to a single float value
 * 
 * @param dest The destination vec2 object.
 * @param val The value to set the components to.
 */
FR_API FR_FORCE_INLINE void fr_vec2_fill(vec2* dest, f32 val) {
    dest->x = val;
    dest->y = val;
}

// ---------------------------------------------
// -------------- Vec2 utilities ---------------
// ---------------------------------------------
/**
 * @brief Check if all components of a vec2 object are equal to a single float value.
 * 
 * @param v The vec2 object to check.
 * @param val The value to check against.
 * @return b8 True if all components are equal to the value, false otherwise.
 */
FR_API FR_FORCE_INLINE b8 fr_vec2_eq_scalar(const vec2* v, f32 val) {
    return fr_equal(v->x, val) && fr_equal(v->y, val);
}

/**
 * @brief Check if all components of a vec2 object are equal to a single float value within a certain threshold.
 * 
 * @param v The vec2 object to check.
 * @param val The value to check against.
 * @param threshold The threshold to use for the comparison.
 * @return b8 True if all components are equal to the value within the threshold, false otherwise.
 */
FR_API FR_FORCE_INLINE b8 fr_vec2_eq_scalar_threshold(const vec2* v, f32 val,
                                                      f32 threshold) {
    return fr_equal_threshold(v->x, val, threshold) &&
           fr_equal_threshold(v->y, val, threshold);
}

/**
 * @brief Check if all components of a vec2 object are equal to each other.
 * 
 * @param v The vec2 object to check.
 * @return b8 True if all components are equal to each other, false otherwise.
 */
FR_API FR_FORCE_INLINE b8 fr_vec2_eq_all(const vec2* v) { return fr_equal(v->x, v->y); }

/**
 * @brief Check if two vectors are equal to each other
 * 
 * @param v0
 * @param v1
 * @return b8 TRUE if the vectors are equal, FALSE otherwise. 
 */
FR_API FR_FORCE_INLINE b8 fr_vec2_eq(const vec2* v0, const vec2* v1) {
    return fr_equal(v0->x, v1->x) && fr_equal(v0->y, v1->y);
}

/**
 * @brief Check if two vectors are equal to each other within a certain threshold
 * 
 * @param v0
 * @param v1
 * @param threshold The threshold to use for the comparison.
 * @return b8 TRUE if the vectors are equal within the threshold, FALSE otherwise. 
 */
FR_API FR_FORCE_INLINE b8 fr_vec2_eq_threshold(const vec2* v0, const vec2* v1, f32 threshold) {
    return fr_equal_threshold(v0->x, v1->x, threshold) &&
           fr_equal_threshold(v0->y, v1->y, threshold);
}

/**
 * @brief Get the maximum component of a vec2 object.
 * 
 * @param v The vec2 object to check.
 * @return f32 The maximum component of the vec2 object.
 */
FR_API FR_FORCE_INLINE f32 fr_vec2_max(const vec2* v) { return fr_max(v->x, v->y); }

/**
 * @brief Get the minimum component of a vec2 object.
 * 
 * @param v The vec2 object to check.
 * @return f32 The minimum component of the vec2 object.
 */
FR_API FR_FORCE_INLINE f32 fr_vec2_min(const vec2* v) { return fr_min(v->x, v->y); }

/**
 * @brief Check if all the components of a vec2 are zero
 * 
 * @param v The vec2 object to check.
 * @return b8 TRUE if all components are zero, FALSE otherwise. 
 */
FR_API FR_FORCE_INLINE b8 fr_vec2_iszero(const vec2* v) {
    return fr_equal(v->x, 0.0f) && fr_equal(v->y, 0.0f);
}

/**
 * @brief Get a vec2 object with the sign of each component of the input vec2 object.
 * 
 * @param v The input vec2 object.
 * @return vec2
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_sign(const vec2* v) {
    return fr_vec2_new(fr_sign(v->x), fr_sign(v->y));
}

/**
 * @brief Get a vec2 object with the absolute value of each component of the input vec2 object.
 * 
 * @param v The input vec2 object.
 * @return vec2
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_abs(const vec2* v) {
    return fr_vec2_new(fr_abs(v->x), fr_abs(v->y));
}

/**
 * @brief Set the components of a vec2 object to their absolute values.
 * 
 * @param v The input vec2 object.
 */
FR_API FR_FORCE_INLINE void fr_vec2_make_abs(vec2* v) {
    v->x = fr_abs(v->x);
    v->y = fr_abs(v->y);
}

/**
 * @brief Get a vec2 object with the square root of each component of the input vec2 object.
 * 
 * @param v The input vec2 object.
 * @return vec2
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_sqrt(const vec2* v) {
    return fr_vec2_new(fr_sqrt(v->x), fr_sqrt(v->y));
}

/**
 * @brief Set the components of a vec2 object to their square roots.
 * 
 * @param v The input vec2 object.
 */
FR_API FR_FORCE_INLINE void fr_vec2_make_sqrt(vec2* v) {
    v->x = fr_sqrt(v->x);
    v->y = fr_sqrt(v->y);
}

/**
 * @brief Do a complex multiplication of two vec2 objects.
 * 
 * @param c0
 * @param c1 
 * @return vec2 New complex number. 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_complex_mul(const vec2* c0, const vec2* c1) {
    return fr_vec2_new(c0->r * c1->r - c0->i * c1->i, c0->r * c1->i + c0->i * c1->r);
}

/**
 * @brief Do a complex division of two vec2 objects.
 * 
 * @param c0
 * @param c1 
 * @return vec2 New complex number. 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_complex_div(const vec2* c0, const vec2* c1) {
    f32 const inv_denom = 1.0f / (c1->r * c1->r + c1->i * c1->i);
    return fr_vec2_new((c0->r * c1->r + c0->i * c1->i) * inv_denom,
                       (c0->i * c1->r - c0->r * c1->i) * inv_denom);
}

/**
 * @brief Get the complex conjugate of a vec2 object.
 * 
 * @param c 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_complex_conjugate(const vec2* c) {
    return fr_vec2_new(c->r, -c->i);
}

/**
 * @brief Set the components of a vec2 object to their complex conjugates.
 * 
 * @param c 
 */
FR_API FR_FORCE_INLINE void fr_vec2_make_complex_conjugate(vec2* c) {
    c->i = -c->i;
}

// ---------------------------------------------
// ------------ Vec2 math functions ------------
// ---------------------------------------------

/**
 * @brief Get the dot product of two vec2 objects.
 * 
 * @param v0 
 * @param v1 
 * @return f32 
 */
FR_API FR_FORCE_INLINE f32 fr_vec2_dot(const vec2* v0, const vec2* v1) {
    return v0->x * v1->x + v0->y * v1->y;
}

/**
 * @brief Get the cross product norm of two vec2 objects assuming the z
 * component is 0.
 * @brief Since the 2 vectors are in 2D space, the cross product only has a z
 * component which this function returns.
 *
 * @param v0
 * @param v1
 * @return f32
 */
FR_API FR_FORCE_INLINE f32 fr_vec2_cross(const vec2* v0, const vec2* v1) {
    return v0->x * v1->y - v0->y * v1->x;
}

/**
 * @brief Get he squared norm2 of a vec2 object.
 * 
 * @param v 
 * @return f32 
 */
FR_API FR_FORCE_INLINE f32 fr_vec2_norm2(const vec2* v) {
    return v->x * v->x + v->y * v->y;
}

/**
 * @brief Get the norm of a vec2 object.
 * 
 * @param v 
 * @return f32 
 */
FR_API FR_FORCE_INLINE f32 fr_vec2_norm(const vec2* v) {
    return fr_sqrt(v->x * v->x + v->y * v->y);
}

/**
 * @brief Add two vec2 objects together.
 * 
 * @param v0
 * @param v1 
 * @return vec2  
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_add(const vec2* v0, const vec2* v1) {
    return fr_vec2_new(v0->x + v1->x, v0->y + v1->y);
}

/**
 * @brief Add two vec2 objects together and store the result in a third vec2 object.
 * 
 * @param v0
 * @param v1
 * @param dest pointer to the destination vec2 object. 
 */
FR_API FR_FORCE_INLINE void fr_vec2_add_to(const vec2* v0, const vec2* v1, vec2* dest) {
    dest->x = v0->x + v1->x;
    dest->y = v0->y + v1->y;
}

/**
 * @brief Add a scalar value to a vec2 object.
 * 
 * @param v
 * @param s 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_adds(const vec2* v, f32 s) {
    return fr_vec2_new(v->x + s, v->y + s);
}

/**
 * @brief Add a scalar value to a vec2 object and store the result in a third vec2 object.
 * 
 * @param v
 * @param s
 * @param dest pointer to the destination vec2 object. 
 */
FR_API FR_FORCE_INLINE void fr_vec2_adds_to(const vec2* v, f32 s, vec2* dest) {
    dest->x = v->x + s;
    dest->y = v->y + s;
}

/**
 * @brief Subtract two vec2 objects v0 - v1.
 * 
 * @param v0
 * @param v1
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_sub(const vec2* v0, const vec2* v1) {
    return fr_vec2_new(v0->x - v1->x, v0->y - v1->y);
}

/**
 * @brief Subtract two vec2 objects v0 - v1 and store the result in a third vec2 object.
 * 
 * @param v0
 * @param v1
 * @param dest pointer to the destination vec2 object. 
 */
FR_API FR_FORCE_INLINE void fr_vec2_sub_to(const vec2* v0, const vec2* v1, vec2* dest) {
    dest->x = v0->x - v1->x;
    dest->y = v0->y - v1->y;
}

/**
 * @brief Subtract a scalar value from a vec2 object.
 * 
 * @param v
 * @param s
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_subs(const vec2* v, f32 s) {
    return fr_vec2_new(v->x - s, v->y - s);
}

/**
 * @brief Subtract a scalar value from a vec2 object and store the result in a third vec2 object.
 * 
 * @param v
 * @param s
 * @param dest pointer to the destination vec2 object. 
 */
FR_API FR_FORCE_INLINE void fr_vec2_subs_to(const vec2* v, f32 s, vec2* dest) {
    dest->x = v->x - s;
    dest->y = v->y - s;
}

/**
 * @brief Elementwise multiply two vec2 objects together.
 * 
 * @param v0
 * @param v1 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_mul(const vec2* v0, const vec2* v1) {
    return fr_vec2_new(v0->x * v1->x, v0->y * v1->y);
}

/**
 * @brief Elementwise multiply two vec2 objects together and store the result in a third vec2 object.
 * 
 * @param v0
 * @param v1
 * @param dest pointer to the destination vec2 object. 
 */
FR_API FR_FORCE_INLINE void fr_vec2_mul_to(const vec2* v0, const vec2* v1, vec2* dest) {
    dest->x = v0->x * v1->x;
    dest->y = v0->y * v1->y;
}

/**
 * @brief Multiply a vec2 object by a scalar value.
 * 
 * @param v
 * @param s 
 */
FR_API FR_FORCE_INLINE void fr_vec2_scale(vec2* v, f32 s) {
    v->x *= s;
    v->y *= s;
}

/**
 * @brief Scale a vec2 by s and return a new vec2.
 * 
 * @param v 
 * @param s 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_get_scaled(const vec2* v, f32 s) {
    return fr_vec2_new(v->x * s, v->y * s);
}

/**
 * @brief Scale a vec2 by s and store the result in a third vec2 object.
 * 
 * @param v
 * @param s
 * @param dest pointer to the destination vec2 object. 
 */
FR_API FR_FORCE_INLINE void fr_vec2_scale_to(const vec2* v, f32 s, vec2* dest) {
    dest->x = v->x * s;
    dest->y = v->y * s;
}

/**
 * @brief Scale the unit vector in the direction of v by s and store the result in the destination.
 * 
 * @param v 
 * @param s 
 * @param dest 
 */
FR_API FR_FORCE_INLINE void fr_vec2_scale_direction_to(const vec2* v, f32 s,
                                                       vec2* dest) {
    f32 const norm = fr_vec2_norm(v);
    if (norm == 0.0f) {
        dest->x = 0.0f;
        dest->y = 0.0f;
        return;
    }
    fr_vec2_scale_to(v, s / norm, dest);
}

/**
 * @brief Divide two vec2 objects elementwise.
 * 
 * @param v0
 * @param v1 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_div(const vec2* v0, const vec2* v1) {
    return fr_vec2_new(v0->x / v1->x, v0->y / v1->y);
}

/**
 * @brief Divide two vec2 objects elementwise and store the result in a third vec2 object.
 * 
 * @param v0
 * @param v1
 * @param dest pointer to the destination vec2 object. 
 */
FR_API FR_FORCE_INLINE void fr_vec2_div_to(const vec2* v0, const vec2* v1, vec2* dest) {
    dest->x = v0->x / v1->x;
    dest->y = v0->y / v1->y;
}

/**
 * @brief Divide a vec2 object by a scalar value.
 * @details If the scalar value is zero the function will return a vec2 with all NaN components.
 *
 * @param v
 * @param s 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_divs(const vec2* v, f32 s) {
    return fr_vec2_new(v->x / s, v->y / s);
}

/**
 * @brief Divide a vec2 object by a scalar value and store the result in a third vec2 object.
 * @details If the scalar value is zero the function will return a vec2 with all NaN components.
 *
 * @param v
 * @param s
 * @param dest pointer to the destination vec2 object. 
 */
FR_API FR_FORCE_INLINE void fr_vec2_divs_to(const vec2* v, f32 s, vec2* dest) {
    dest->x = v->x / s;
    dest->y = v->y / s;
}

/**
 * @brief Add two vec2 objects together and store the result in a third vec2 object.
 * 
 * @param v0 
 * @param v1 
 * @param dest 
 */
FR_API FR_FORCE_INLINE void fr_vec2_addadd(const vec2* v0, const vec2* v1, vec2* dest) {
    dest->x += v0->x + v1->x;
    dest->y += v0->y + v1->y;
}

/**
 * @brief Subtract two vec2 objects v0 - v1 and add the result to the third vec2 object.
 * 
 * @param v0 
 * @param v1 
 * @param dest 
 */
FR_API FR_FORCE_INLINE void fr_vec2_subadd(const vec2* v0, const vec2* v1, vec2* dest) {
    dest->x += v0->x - v1->x;
    dest->y += v0->y - v1->y;
}

/**
 * @brief elementwise multiply two vec2 objects together and add the result to third vec2 object.
 * 
 * @param v0 
 * @param v1 
 * @param dest 
 */
FR_API FR_FORCE_INLINE void fr_vec2_muladd(const vec2* v0, const vec2* v1, vec2* dest) {
    dest->x += v0->x * v1->x;
    dest->y += v0->y * v1->y;
}

/**
 * @brief Multiply a vec2 object by a scalar value and add the result to a third vec2 object.
 * 
 * @param v
 * @param s 
 * @param dest 
 */
FR_API FR_FORCE_INLINE void fr_vec2_muladds(const vec2* v, f32 s, vec2* dest) {
    dest->x += v->x * s;
    dest->y += v->y * s;
}

/**
 * @brief Add the maximum components of two vec2 objects to a third vec2 object.
 * 
 * @param v0 
 * @param v1 
 * @param dest 
 */
FR_API FR_FORCE_INLINE void fr_vec2_maxadd(const vec2* v0, const vec2* v1, vec2* dest) {
    dest->x += fr_max(v0->x, v1->x);
    dest->y += fr_max(v0->y, v1->y);
}

/**
 * @brief Add the minimum components of two vec2 objects to a third vec2 object.
 * 
 * @param v0 
 * @param v1 
 * @param dest 
 */
FR_API FR_FORCE_INLINE void fr_vec2_minadd(const vec2* v0, const vec2* v1, vec2* dest) {
    dest->x += fr_min(v0->x, v1->x);
    dest->y += fr_min(v0->y, v1->y);
}

/**
 * @brief Subtract two vec2 objects v0 - v1 and subtract the result from the third vec2 object.
 * 
 * @param v0 
 * @param v1 
 * @param dest 
 */
FR_API FR_FORCE_INLINE void fr_vec2_subsub(const vec2* v0, const vec2* v1, vec2* dest) {
    dest->x -= v0->x - v1->x;
    dest->y -= v0->y - v1->y;
}

/**
 * @brief Add two vec2 objects together and subtract the result from the third vec2 object.
 * 
 * @param v0 
 * @param v1 
 * @param dest 
 */
FR_API FR_FORCE_INLINE void fr_vec2_addsub(const vec2* v0, const vec2* v1, vec2* dest) {
    dest->x -= v0->x + v1->x;
    dest->y -= v0->y + v1->y;
}

/**
 * @brief Elementwise multiply two vec2 objects together and subtract the result from the third vec2 object.
 * 
 * @param v0 
 * @param v1 
 * @param dest 
 */
FR_API FR_FORCE_INLINE void fr_vec2_mulsub(const vec2* v0, const vec2* v1, vec2* dest) {
    dest->x -= v0->x * v1->x;
    dest->y -= v0->y * v1->y;
}

/**
 * @brief Multiply a vec2 object by a scalar value and subtract the result from a third vec2 object.
 * 
 * @param v
 * @param s 
 * @param dest 
 */
FR_API FR_FORCE_INLINE void fr_vec2_mulsubs(const vec2* v, f32 s, vec2* dest) {
    dest->x -= v->x * s;
    dest->y -= v->y * s;
}

/**
 * @brief Add the maximum components of two vec2 objects to a third vec2 object.
 * 
 * @param v0 
 * @param v1 
 * @param dest 
 */
FR_API FR_FORCE_INLINE void fr_vec2_maxsub(const vec2* v0, const vec2* v1, vec2* dest) {
    dest->x -= fr_max(v0->x, v1->x);
    dest->y -= fr_max(v0->y, v1->y);
}

/**
 * @brief Add the minimum components of two vec2 objects to a third vec2 object.
 * 
 * @param v0 
 * @param v1 
 * @param dest 
 */
FR_API FR_FORCE_INLINE void fr_vec2_minsub(const vec2* v0, const vec2* v1, vec2* dest) {
    dest->x -= fr_min(v0->x, v1->x);
    dest->y -= fr_min(v0->y, v1->y);
}

/**
 * @brief Negate a vec2 object.
 * 
 * @param v 
 */
FR_API FR_FORCE_INLINE void fr_vec2_negate(vec2* v) {
    v->x = -v->x;
    v->y = -v->y;
}

/**
 * @brief Get the negative of a vec2 object.
 * 
 * @param v 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_get_negative(const vec2* v) {
    return fr_vec2_new(-v->x, -v->y);
}

/**
 * @brief Negate a vec2 object and store the result in an another vec2 object.
 * 
 * @param v 
 * @param dest 
 */
FR_API FR_FORCE_INLINE void fr_vec2_negate_to(const vec2* v, vec2* dest) {
    dest->x = -v->x;
    dest->y = -v->y;
}

/**
 * @brief Normalize a vec2 object.
 * 
 * @param v 
 */
FR_API FR_FORCE_INLINE void fr_vec2_normalize(vec2* v) {
    f32 const norm = fr_vec2_norm(v);
    if (norm == 0.0f) {
        v->x = 0.0f;
        v->y = 0.0f;
    }
    fr_vec2_scale(v, 1 / norm);
}

/**
 * @brief Get the normalized version of a vec2 object.
 * 
 * @param v 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_get_unit(const vec2* v) {
    f32 const norm = fr_vec2_norm(v);
    if (norm == 0.0f) {
        return fr_vec2_zero();
    }
    return fr_vec2_get_scaled(v, 1 / norm);
}

/**
 * @brief Get a vec2 in the direction of v scaled by s.
 * 
 * @param v 
 * @param s 
 * @return vec2
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_scale_direction(const vec2* v, f32 s) {
    f32 const norm = fr_vec2_norm(v);
    if (norm == 0.0f) {
        return fr_vec2_zero();
    }
    return fr_vec2_get_scaled(v, s / norm);
}

/**
 * @brief Get a vec2 with v rotated by angle.
 * 
 * @param v 
 * @param angle angle in radians
 * @return vec2
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_get_rotated(const vec2* v, f32 angle) {
    f32 const c = fr_cos(angle);
    f32 const s = fr_sin(angle);
    return fr_vec2_new(v->x * c - v->y * s, v->x * s + v->y * c);
}

/**
 * @brief Rotate a v by angle.
 * 
 * @param v 
 * @param angle angle in radians
 */
FR_API FR_FORCE_INLINE void fr_vec2_rotate(vec2* v, f32 angle) {
    f32 const c = fr_cos(angle);
    f32 const s = fr_sin(angle);
    f32 const x = v->x;
    v->x = x * c - v->y * s;
    v->y = x * s + v->y * c;
}

/**
 * @brief Get the perpendicular of a vec2 object.
 * 
 * @param v 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_get_perpendicular(const vec2* v) {
    return fr_vec2_new(-v->y, v->x);
}

/**
 * @brief Make a vec2 object perpendicular to itself.
 * 
 * @param v 
 */
FR_API FR_FORCE_INLINE void fr_vec2_make_perpendicular(vec2* v) {
    f32 const x = v->x;
    v->x = -v->y;
    v->y = x;
}

/**
 * @brief Get the angle between two vec2 objects.
 * 
 * @param v0 
 * @param v1 
 * @return f32 
 */
FR_API FR_FORCE_INLINE f32 fr_vec2_angle_between(const vec2* v0, const vec2* v1) {
    f32 const dot = fr_vec2_dot(v0, v1);
    f32 const cross = fr_vec2_cross(v0, v1);
    return fr_atan2(cross, dot);
}

/**
 * @brief Get the projection of a vec2 object onto another vec2 object.
 * 
 * @param v 
 * @param onto 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_project(const vec2* v, const vec2* onto) {
    f32 const dot = fr_vec2_dot(v, onto);
    return fr_vec2_scale_direction(onto, dot);
}

/**
 * @brief Get the midpoint between two vec2
 * 
 * @param v0
 * @param v1 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_center(const vec2* v0, const vec2* v1) {
    return fr_vec2_new((v0->x + v1->x) * 0.5f, (v0->y + v1->y) * 0.5f);
}

/**
 * @brief Get the distance squared between two vec2
 * 
 * @param v0
 * @param v1 
 * @return f32 
 */
FR_API FR_FORCE_INLINE f32 fr_vec2_distance2(const vec2* v0, const vec2* v1) {
    f32 const dx = v0->x - v1->x;
    f32 const dy = v0->y - v1->y;
    return dx * dx + dy * dy;
}

/**
 * @brief Get the distance between two vec2
 * 
 * @param v0
 * @param v1 
 * @return f32 
 */
FR_API FR_FORCE_INLINE f32 fr_vec2_distance(const vec2* v0, const vec2* v1) {
    f32 const dx = v0->x - v1->x;
    f32 const dy = v0->y - v1->y;
    return fr_sqrt(dx * dx + dy * dy);
}

/**
 * @brief Get a vec2 with the maximum components of two vec2 objects
 * 
 * @param v1 
 * @param v2 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_maxv(const vec2* v1, const vec2* v2) {
    return fr_vec2_new(fr_max(v1->x, v2->x), fr_max(v1->y, v2->y));
}

/**
 * @brief Get a vec2 with the minimum components of two vec2 objects
 * 
 * @param v1 
 * @param v2 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_minv(const vec2* v1, const vec2* v2) {
    return fr_vec2_new(fr_min(v1->x, v2->x), fr_min(v1->y, v2->y));
}

/**
 * @brief Clamp the components of a vec2 object between minVal and maxVal.
 * 
 * @param v 
 * @param minVal 
 * @param maxVal 
 */
FR_API FR_FORCE_INLINE void fr_vec2_clamp(vec2* v, f32 minVal, f32 maxVal) {
    v->x = fr_clamp(v->x, minVal, maxVal);
    v->y = fr_clamp(v->y, minVal, maxVal);
}

/**
 * @brief Get a vec2 with the components of a vec2 object clamped between minVal and maxVal.
 * 
 * @param v 
 * @param minVal 
 * @param maxVal 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_get_clamped(const vec2* v, f32 minVal,
                                                f32 maxVal) {
    return fr_vec2_new(fr_clamp(v->x, minVal, maxVal),
                       fr_clamp(v->y, minVal, maxVal));
}

/**
 * @brief Linearly interpolate between two vec2 objects using t
 * 
 * @param from 
 * @param to 
 * @param t 
 * @return vec2 
 */
FR_API FR_FORCE_INLINE vec2 fr_vec2_lerp(const vec2* from, const vec2* to, f32 t) {
    return fr_vec2_new(from->x + (to->x - from->x) * t,
                       from->y + (to->y - from->y) * t);
}

/**
 * @brief Linearly interpolate between two vec2 objects using t and store the result in a third vec2 object.
 * 
 * @param from 
 * @param to 
 * @param t 
 * @param dest 
 */
FR_API FR_FORCE_INLINE void fr_vec2_lerp_to(const vec2* from, const vec2* to, f32 t,
                                            vec2* dest) {
    dest->x = from->x + (to->x - from->x) * t;
    dest->y = from->y + (to->y - from->y) * t;
}
