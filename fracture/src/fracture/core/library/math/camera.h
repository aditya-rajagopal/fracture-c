/**
 * @file camera.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-03-19
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include "fracture/core/defines.h"
#include "fracture/core/library/math/mat4.h"
#include "fracture/core/library/math/vec3.h"
#include "fracture/core/library/math/vec4.h"
#include "fracture/core/library/math/affine/constructors.h"
#include "fracture/core/library/math/affine/rotation.h"
#include "fracture/core/library/math/affine/translation.h"

FR_FORCE_INLINE void fr_camera_orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far, mat4* out) {
    const f32 lr = 1.0f / (right - left);
    const f32 bt = 1.0f / (top - bottom);
    const f32 nf = 1.0f / (far - near);

    out->m00 = 2.0f * lr; out->m01 = 0.0f       ; out->m02 = 0.0f       ; out->m03 = -(right + left) * lr;
    out->m10 = 0.0f     ; out->m11 = 2.0f * bt  ; out->m12 = 0.0f       ; out->m13 = -(top + bottom) * bt;
    out->m20 = 0.0f     ; out->m21 = 0.0f       ; out->m22 = -2.0f * nf ; out->m23 = -(far + near) * nf;
    out->m30 = 0.0f     ; out->m31 = 0.0f       ; out->m32 = 0.0f       ; out->m33 = 1.0f;
}

FR_FORCE_INLINE void fr_camera_perspective(f32 fov_radians, f32 aspect_ratio, f32 near_clip, f32 far_clia, mat4* out) {
    const f32 half_tan_fov = fr_tan(fov_radians * 0.5f);

    out->m00 = 1.0f / (aspect_ratio * half_tan_fov);
    out->m01 = 0.0f;
    out->m02 = 0.0f;
    out->m03 = 0.0f;
    out->m10 = 0.0f;
    out->m11 = 1.0f / half_tan_fov;
    out->m12 = 0.0f;
    out->m13 = 0.0f;
    out->m20 = 0.0f;
    out->m21 = 0.0f;
    out->m22 = (far_clia + near_clip) / (near_clip - far_clia);
    out->m23 = 2.0f * far_clia * near_clip / (near_clip - far_clia);
    out->m30 = 0.0f;
    out->m31 = 0.0f;
    out->m32 = -1.0f;
    out->m33 = 0.0f;
}

FR_FORCE_INLINE void fr_camera_look_at(vec3* position, vec3* target, vec3* up, mat4* out) {
    vec3 z_axis, x_axis, y_axis;
    // vector from position to target
    z_axis.x = target->x - position->x;
    z_axis.y = target->y - position->y;
    z_axis.z = target->z - position->z;

    fr_vec3_normalize(&z_axis, &z_axis);
    fr_vec3_cross(&z_axis, up, &x_axis);
    fr_vec3_normalize(&x_axis, &x_axis);
    fr_vec3_cross(&x_axis, &z_axis, &y_axis);
    fr_vec3_normalize(&y_axis, &y_axis);

    out->m00 = x_axis.x;
    out->m10 = y_axis.x;
    out->m20 = -z_axis.x;
    out->m30 = 0.0f;
    out->m01 = x_axis.y;
    out->m11 = y_axis.y;
    out->m21 = -z_axis.y;
    out->m31 = 0.0f;
    out->m02 = x_axis.z;
    out->m12 = y_axis.z;
    out->m22 = -z_axis.z;
    out->m32 = 0.0f;
    out->m03 = -fr_vec3_dot(&x_axis, position);
    out->m13 = -fr_vec3_dot(&y_axis, position);
    out->m23 = fr_vec3_dot(&z_axis, position);
    out->m33 = 1.0f;
}