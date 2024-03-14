/**
 * @file constructors.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-03-13
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include "fracture/core/defines.h"
#include "../mat4.h"
#include "../vec3.h"
#include "../simd/sse.h"

#include "operations.h"
#include "translation.h"

FR_FORCE_INLINE void fr_affine_create_translation(vec3* translation, mat4* out) {
    out->simd[0] = _mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f);
    out->simd[1] = _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f);
    out->simd[2] = _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f);
    out->simd[3] = _mm_setr_ps(translation->x, translation->y, translation->z, 1.0f);
}

FR_FORCE_INLINE void fr_affine_create_scale(vec3* scale, mat4* out) {
    out->simd[0] = _mm_setr_ps(scale->x, 0.0f, 0.0f, 0.0f);
    out->simd[1] = _mm_setr_ps(0.0f, scale->y, 0.0f, 0.0f);
    out->simd[2] = _mm_setr_ps(0.0f, 0.0f, scale->z, 0.0f);
    out->simd[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
}

FR_FORCE_INLINE void fr_affine_create_rotation(vec3* axis, f32 angle, mat4* out) {
    const f32 c = fr_cos(angle);
    const f32 s = fr_sin(angle);
    const f32 t = 1.0f - c;
    vec4 axis_n, axis_c, axis_s;
    vec4 axis4 = (vec4){.x = axis->x, .y = axis->y, .z = axis->z, .w = 0.0f};
    fr_vec4_normalize(&axis4, &axis_n);
    fr_vec4_scale(&axis_n, t, &axis_c);
    fr_vec4_scale(&axis_n, s, &axis_s);

    out->simd[0] = _mm_mul_ps(axis_n.simd, _mm_set1_ps(axis_c.x));
    out->simd[1] = _mm_mul_ps(axis_n.simd, _mm_set1_ps(axis_c.y));
    out->simd[2] = _mm_mul_ps(axis_n.simd, _mm_set1_ps(axis_c.z));

    out->simd[0] = _mm_add_ps(out->simd[0], _mm_setr_ps(c, axis_s.z, -axis_s.y, 0.0f));
    out->simd[1] = _mm_add_ps(out->simd[1], _mm_setr_ps(-axis_s.z, c, axis_s.x, 0.0f));
    out->simd[2] = _mm_add_ps(out->simd[2], _mm_setr_ps(axis_s.y, -axis_s.x, c, 0.0f));

    out->simd[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
}

FR_FORCE_INLINE void fr_affine_create_rotation_x(f32 angle, mat4* out) {
    const f32 c = fr_cos(angle);
    const f32 s = fr_sin(angle);
    out->simd[0] = _mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f);
    out->simd[1] = _mm_setr_ps(0.0f, c, s, 0.0f);
    out->simd[2] = _mm_setr_ps(0.0f, -s, c, 0.0f);
    out->simd[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
}

FR_FORCE_INLINE void fr_affine_create_rotation_y(f32 angle, mat4* out) {
    const f32 c = fr_cos(angle);
    const f32 s = fr_sin(angle);
    out->simd[0] = _mm_setr_ps(c, 0.0f, -s, 0.0f);
    out->simd[1] = _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f);
    out->simd[2] = _mm_setr_ps(s, 0.0f, c, 0.0f);
    out->simd[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
}

FR_FORCE_INLINE void fr_affine_create_rotation_z(f32 angle, mat4* out) {
    const f32 c = fr_cos(angle);
    const f32 s = fr_sin(angle);
    out->simd[0] = _mm_setr_ps(c, s, 0.0f, 0.0f);
    out->simd[1] = _mm_setr_ps(-s, c, 0.0f, 0.0f);
    out->simd[2] = _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f);
    out->simd[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
}

FR_FORCE_INLINE void fr_affine_create_pivot_rotation(vec3* pivot, vec3* axis, f32 angle, mat4* out) {
    fr_affine_create_translation(pivot, out); // T to pivot
    mat4 rotation;
    fr_affine_create_rotation(axis, angle, &rotation); // R
    fr_affine_mul_rot(out, &rotation, out); // T * R about axis
    vec3 neg_pivot;
    fr_vec3_negative(pivot, &neg_pivot);
    fr_affine_translate(out, &neg_pivot); // T * R * -T back to origianl position
}
