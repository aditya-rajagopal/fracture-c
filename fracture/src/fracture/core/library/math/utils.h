/**
 * @file utils.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-02-29
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include "math_constants.h"
#include "fracture/core/defines.h"
#include "detail/vec2.h"
#include "detail/vec3.h"
#include "detail/vec4.h"
#include "detail/matrix.h"

// Min and Max and Clamp
FR_FORCE_INLINE f32 fr_deg_to_rad(f32 deg) { return deg * DEG_TO_RAD; }
FR_FORCE_INLINE f32 fr_rad_to_deg(f32 rad) { return rad * RAD_TO_DEG; }
FR_FORCE_INLINE f32 fr_deg_to_rev(f32 deg) { return deg * DEG_TO_REV; }
FR_FORCE_INLINE f32 fr_rev_to_deg(f32 rev) { return rev * REV_TO_DEG; }
FR_FORCE_INLINE f32 fr_rad_to_rev(f32 rad) { return rad * RAD_TO_REV; }
FR_FORCE_INLINE f32 fr_rev_to_rad(f32 rev) { return rev * REV_TO_RAD; }

FR_FORCE_INLINE void fr_make_deg(f32* rad) { *rad *= RAD_TO_DEG; }
FR_FORCE_INLINE void fr_make_rad(f32* deg) { *deg *= DEG_TO_RAD; }

FR_FORCE_INLINE f32 fr_min(f32 a, f32 b) { return a < b ? a : b; }
FR_FORCE_INLINE f32 fr_max(f32 a, f32 b) { return a > b ? a : b; }
FR_FORCE_INLINE i32 fr_imin(i32 a, i32 b) { return a < b ? a : b; }
FR_FORCE_INLINE i32 fr_imax(i32 a, i32 b) { return a > b ? a : b; }
FR_FORCE_INLINE u32 fr_umin(u32 a, u32 b) { return a < b ? a : b; }
FR_FORCE_INLINE u32 fr_umax(u32 a, u32 b) { return a > b ? a : b; }
FR_FORCE_INLINE f64 fr_dmin(f64 a, f64 b) { return a < b ? a : b; }
FR_FORCE_INLINE f64 fr_dmax(f64 a, f64 b) { return a > b ? a : b; }

FR_FORCE_INLINE f32 fr_sign(f32 val) {
    return (float)((val > 0.0f) - (val < 0.0f));
}

FR_FORCE_INLINE i32 fr_isign(i32 val) { return val < 0 ? -1 : 1; }
FR_FORCE_INLINE f64 fr_dsign(f64 val) { return val < 0.0 ? -1.0 : 1.0; }

FR_FORCE_INLINE f32 fr_clamp(f32 val, f32 min, f32 max) { return fr_max(min, fr_min(max, val)); }
FR_FORCE_INLINE i32 fr_iclamp(i32 val, i32 min, i32 max) { return fr_imax(min, fr_imin(max, val)); }
FR_FORCE_INLINE u32 fr_uclamp(u32 val, u32 min, u32 max) { return fr_umax(min, fr_umin(max, val)); }
FR_FORCE_INLINE f64 fr_dclamp(f64 val, f64 min, f64 max) { return fr_dmax(min, fr_dmin(max, val)); }

FR_FORCE_INLINE f32 fr_clamp_zo(f32 val) { return fr_clamp(val, 0.0f, 1.0f); }
FR_FORCE_INLINE f64 fr_dclamp_zo(f64 val) { return fr_dclamp(val, 0.0, 1.0); }

FR_FORCE_INLINE f32 fr_lerp(f32 a, f32 b, f32 t) { return a + (b - a) * t; }
FR_FORCE_INLINE f64 fr_dlerp(f64 a, f64 b, f64 t) { return a + (b - a) * t; }

FR_FORCE_INLINE f32 fr_lerp_clamped(f32 a, f32 b, f32 t) { return fr_lerp(a, b, fr_clamp_zo(t)); }
FR_FORCE_INLINE f64 fr_dlerp_clamped(f64 a, f64 b, f64 t) { return fr_dlerp(a, b, fr_dclamp_zo(t)); }

FR_FORCE_INLINE f32 fr_smooth(f32 val) { return val * val * (3.0f - 2.0f * val); }
FR_FORCE_INLINE f32 fr_smooth_fifth(f32 val) { return val * val * val * (val * (val * 6.0f - 15.0f) + 10.0f); }

/**
 * @brief Implementation of the 3rd order smoothstep function.
 * 
 * @param a first edge of the interpolation range
 * @param b second edge of the interpolation range
 * @param t interpolation value
 * @return f32 the interpolated value
 */
FR_FORCE_INLINE f32 fr_smoothstep(f32 a, f32 b, f32 t) {
    t = fr_clamp((t - a) / (b - a), 0.0f, 1.0f);
    return fr_smooth(t);
}

/**
 * @brief Implementation of the 5th order smootherstep function.
 * 
 * @param a first edge of the interpolation range
 * @param b second edge of the interpolation range
 * @param t interpolation value
 * @return f32 the interpolated value
 */
FR_FORCE_INLINE f32 fr_smootherstep(f32 a, f32 b, f32 t) {
    t = fr_clamp((t - a) / (b - a), 0.0f, 1.0f);
    return fr_smooth_fifth(t);
}

/**
 * @brief Implementation of the step function with branching.
 * 
 * @param edge 
 * @param val 
 * @return 0 if val < edge, 1 otherwise 
 */
FR_FORCE_INLINE f32 fr_step(f32 edge, f32 val) {
    return val < edge ? 0.0f : 1.0f;
}

/**
 * @brief Implementation of the step function without branching but with typecasting.
 * 
 * @param edge the boundary of the step function
 * @param val the value to be compared
 * @return 0 if val < edge, 1 otherwise
 */
FR_FORCE_INLINE f32 fr_step_nobranch(f32 edge, f32 val) {
    return (f32)(val >= edge);
}

FR_FORCE_INLINE f32 fr_smoothinterp(f32 a, f32 b, f32 t) {
    return fr_lerp(a, b, fr_smooth(t));
}

FR_FORCE_INLINE f32 fr_smoothinterp_clamped(f32 a, f32 b, f32 t) {
    return fr_lerp(a, b, fr_smooth(fr_clamp_zo(t)));
}

FR_FORCE_INLINE void fr_swap(f32* restrict a, f32* restrict b) {
    f32 temp = *a;
    *a = *b;
    *b = temp;
}

FR_FORCE_INLINE void fr_iswap(i32* restrict a, i32* restrict b) {
    i32 temp = *a;
    *a = *b;
    *b = temp;
}

FR_FORCE_INLINE void fr_uswap(u32* restrict a, u32* restrict b) {
    u32 temp = *a;
    *a = *b;
    *b = temp;
}

FR_FORCE_INLINE void fr_dswap(f64* restrict a, f64* restrict b) {
    f64 temp = *a;
    *a = *b;
    *b = temp;
}

FR_FORCE_INLINE f32 fr_abs(f32 val) { return val < 0.0f ? -val : val; }
FR_FORCE_INLINE i32 fr_iabs(i32 val) { return val < 0 ? -val : val; }
FR_FORCE_INLINE f64 fr_dabs(f64 val) { return val < 0.0 ? -val : val; }

FR_FORCE_INLINE b8 fr_equal(f32 a, f32 b) { return fr_abs(a - b) < FLOAT_EPSILON; }
FR_FORCE_INLINE b8 fr_equal_threshold(f32 a, f32 b, f32 thresh) { return fr_abs(a - b) < thresh; }
FR_FORCE_INLINE b8 fr_equal_precise(f64 a, f64 b) { return fr_dabs(a - b) < DOUBLE_EPSILON; }

FR_FORCE_INLINE f32 fr_pow2(f32 val) { return val * val; }
FR_FORCE_INLINE i32 fr_ipow2(i32 val) { return val * val; }
FR_FORCE_INLINE u32 fr_upow2(u32 val) { return val * val; }
FR_FORCE_INLINE f64 fr_dpow2(f64 val) { return val * val; }

// Functions that need math.h to be included will be in a C file.
// This is to avoid including math.h in the header file.

FR_API f32 fr_sqrt(f32 val);
FR_API f32 fr_inv_sqrt(f32 val);
FR_API f32 fr_exp(f32 val);
FR_API f32 fr_log(f32 val);
FR_API f32 fr_log2(f32 val);
FR_API f32 fr_log10(f32 val);
FR_API f32 fr_pow(f32 base, f32 exp);
FR_API f32 fr_sin(f32 val);
FR_API f32 fr_cos(f32 val);
FR_API f32 fr_tan(f32 val);
FR_API f32 fr_asin(f32 val);
FR_API f32 fr_acos(f32 val);
FR_API f32 fr_atan(f32 val);
FR_API f32 fr_atan2(f32 y, f32 x);
FR_API f32 fr_sinh(f32 val);
FR_API f32 fr_cosh(f32 val);
FR_API f32 fr_tanh(f32 val);
FR_API f32 fr_asinh(f32 val);
FR_API f32 fr_acosh(f32 val);
FR_API f32 fr_atanh(f32 val);
FR_API i32 fr_next_pow2(i32 val);
FR_API i32 fr_prev_pow2(i32 val);

//-----------------------------------------------------------------------------------------------
// IO
//-----------------------------------------------------------------------------------------------

FR_API void fr_mat2_print(const char* name, int* len, const mat2* m, char* out_string);

FR_API void fr_mat3_print(const char* name, int* len, const mat3* m, char* out_string);

FR_API void fr_mat4_print(const char* name, int* len, const mat4* m, char* out_string);