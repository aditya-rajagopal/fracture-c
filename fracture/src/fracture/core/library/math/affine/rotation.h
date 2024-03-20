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
#include "../quaternion.h"

FR_FORCE_INLINE void fr_affine_rotate(mat4* transform, const vec3* axis, f32 angle) {
    mat4 rotation;
    fr_affine_create_rotation(axis, angle, &rotation);
    fr_affine_mul_rot(transform, &rotation, transform);
}

FR_FORCE_INLINE void fr_affine_rotate_x(const mat4* transform, f32 angle, mat4* result) {
    mat4 rotation;
    fr_affine_create_rotation_x(angle, &rotation);
    fr_affine_mul_rot(transform, &rotation, result);
}

FR_FORCE_INLINE void fr_affine_rotate_y(const mat4* transform, f32 angle, mat4* result) {
    mat4 rotation;
    fr_affine_create_rotation_y(angle, &rotation);
    fr_affine_mul_rot(transform, &rotation, result);
}

FR_FORCE_INLINE void fr_affine_rotate_z(const mat4* transform, f32 angle, mat4* result) {
    mat4 rotation;
    fr_affine_create_rotation_z(angle, &rotation);
    fr_affine_mul_rot(transform, &rotation, result);
}

FR_FORCE_INLINE void fr_affine_rotate_xyz(const mat4* transform, f32 x, f32 y, f32 z, mat4* result) {
    mat4 rotation;
    fr_affine_create_rotation_xyz(x, y, z, &rotation);
    fr_affine_mul_rot(transform, &rotation, result);
}

FR_FORCE_INLINE void fr_affine_rotate_around(mat4* transform, const vec3* pivot, f32 angle, const vec3* axis) {
    fr_affine_translate(transform, pivot);
    fr_affine_rotate(transform, axis, angle);
    vec3 inv_pivot = (vec3){.x=-pivot->x, .y=-pivot->y, .z=-pivot->z};
    fr_affine_translate(transform, &inv_pivot);
}

FR_FORCE_INLINE void fr_affine_rotate_around_to(const mat4* transform, const vec3* pivot, f32 angle, const vec3* axis, mat4* result) {
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

FR_FORCE_INLINE void fr_affine_quat_rotate(const mat4* transform, const quat* rotation, mat4* out_transform) {
    mat4 rot_mat;
    fr_quat_to_mat4(rotation, &rot_mat);
    fr_affine_mul_rot(transform, &rot_mat, out_transform);
}

FR_FORCE_INLINE void fr_affine_quat_rotate_around(const mat4* transform, const vec3* pivot, const quat* rotation, mat4* out_transform) {
    mat4 rot_mat;
    fr_quat_to_rotation_matrix(rotation, pivot, &rot_mat);
    fr_affine_mul_rot(transform, &rot_mat, out_transform);
}

FR_FORCE_INLINE void fr_affine_quat_spi(const mat4* transform, const quat* rotation, mat4* out_transform) {
    vec3 pivot = {.x=transform->m03, .y=transform->m13, .z=transform->m23};
    fr_affine_quat_rotate_around(transform, &pivot, rotation, out_transform);
}
