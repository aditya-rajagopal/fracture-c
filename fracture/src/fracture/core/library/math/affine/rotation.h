/**
 * @file rotation.h
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
#include "../simd/sse.h"
#include "constructors.h"
#include "operations.h"

FR_FORCE_INLINE void fr_affine_rotate(mat4* transform, vec3* axis, f32 angle) {
    mat4 rotation;
    fr_affine_create_rotation(axis, angle, &rotation);
    fr_affine_mul_rot(transform, &rotation, transform);
}

FR_FORCE_INLINE void fr_affine_rotate_x(mat4* transform, f32 angle, mat4* result) {
    mat4 rotation;
    fr_affine_create_rotation_x(angle, &rotation);
    fr_affine_mul_rot(transform, &rotation, result);
}

FR_FORCE_INLINE void fr_affine_rotate_y(mat4* transform, f32 angle, mat4* result) {
    mat4 rotation;
    fr_affine_create_rotation_y(angle, &rotation);
    fr_affine_mul_rot(transform, &rotation, result);
}

FR_FORCE_INLINE void fr_affine_rotate_z(mat4* transform, f32 angle, mat4* result) {
    mat4 rotation;
    fr_affine_create_rotation_z(angle, &rotation);
    fr_affine_mul_rot(transform, &rotation, result);
}

FR_FORCE_INLINE void fr_affine_rotate_around(mat4* transform, vec3* pivot, f32 angle, vec3* axis) {
    fr_affine_translate(transform, pivot);
    fr_affine_rotate(transform, axis, angle);
    vec3 inv_pivot = (vec3){.x=-pivot->x, .y=-pivot->y, .z=-pivot->z};
    fr_affine_translate(transform, &inv_pivot);
}

FR_FORCE_INLINE void fr_affine_rotate_around_to(mat4* transform, vec3* pivot, f32 angle, vec3* axis, mat4* result) {
    fr_affine_translate_to(transform, pivot, result);
    fr_affine_rotate(result, axis, angle);
    vec3 inv_pivot = (vec3){.x=-pivot->x, .y=-pivot->y, .z=-pivot->z};
    fr_affine_translate(result, &inv_pivot);
}

FR_FORCE_INLINE void fr_affine_spin(mat4* transform, f32 angle, vec3* axis) {
    mat4 rotation;
    vec3 pivot = (vec3){.x=transform->m03, .y=transform->m13, .z=transform->m23};
    fr_affine_create_pivot_rotation(&pivot, axis, angle, &rotation);
    fr_affine_mul_rot(&rotation, transform, transform);
}
