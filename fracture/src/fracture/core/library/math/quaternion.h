/**
 * @file quaternion.h
 * @author Aditya Rajagopal
 * @brief
 * @version 0.0.1
 * @date 2024-03-20
 *
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 *
 */
#pragma once

#include "fracture/core/defines.h"
#include "simd/sse.h"
#include "vec3.h"
#include "vec4.h"

FR_FORCE_INLINE void fr_quat_identity(quat* out) { out->simd = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f); }

FR_FORCE_INLINE void fr_quat(f32 x, f32 y, f32 z, f32 w, quat* out) { out->simd = _mm_setr_ps(x, y, z, w); }

FR_FORCE_INLINE void fr_quat_vec3(const vec3* v, f32 w, quat* out) { out->simd = _mm_setr_ps(v->x, v->y, v->z, w); }

FR_FORCE_INLINE f32 fr_quat_norm(const quat* q) { return fr_vec4_norm(q); }

FR_FORCE_INLINE void fr_quat_normalize(const quat* q, quat* out) { fr_vec4_normalize(q, out); }

FR_FORCE_INLINE void fr_quat_copy(const quat* q, quat* out) { out->simd = q->simd; }

FR_FORCE_INLINE void fr_quat_conjugate(const quat* q, quat* out) {
    out->simd = _mm_mul_ps(q->simd, _mm_setr_ps(-1.0f, -1.0f, -1.0f, 1.0f));
}

FR_FORCE_INLINE void fr_quat_inverse(const quat* q, quat* out) {
    fr_quat_conjugate(q, out);
    __m128 invnorm = fr_simd_vinvnorm(q->simd);
    out->simd = _mm_mul_ps(out->simd, invnorm);
}

FR_FORCE_INLINE f32 fr_quat_real(const quat* q) { return q->w; }

FR_FORCE_INLINE void fr_quat_imag(const quat* q, vec3* out) { fr_vec3_create(q->data, out); }

FR_FORCE_INLINE void fr_quat_mul(const quat* a, const quat* b, quat* out) {
    /*
     * (a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y)i
     * (a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x)j
     * (a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w)k
     * (a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z)
     */
#if FR_SIMD == 1
    __m128 sign_npnp = FR_SIGNMASK_NPNPf32x4;
    __m128 sign_nppn = FR_SIMD_SHUFFLE1(sign_npnp, 3, 2, 0, 1);
    __m128 sign_nnpp = _mm_unpackhi_ps(sign_npnp, sign_npnp);
    __m128 x = FR_SIMD_SPLAT_X(a->simd);
    __m128 y = FR_SIMD_SPLAT_Y(a->simd);
    __m128 z = FR_SIMD_SPLAT_Z(a->simd);

    x = _mm_xor_ps(x, sign_npnp);
    y = _mm_xor_ps(y, sign_nnpp);
    z = _mm_xor_ps(z, sign_nppn);

    out->simd = _mm_mul_ps(FR_SIMD_SPLAT_W(a->simd), b->simd);
    out->simd = fr_simd_fmadd(x, FR_SIMD_SHUFFLE1(b->simd, 0, 1, 2, 3), out->simd);
    out->simd = fr_simd_fmadd(y, FR_SIMD_SHUFFLE1(b->simd, 1, 0, 3, 2), out->simd);
    out->simd = fr_simd_fmadd(z, FR_SIMD_SHUFFLE1(b->simd, 2, 3, 0, 1), out->simd);
#else
    out->x = a->w * b->x + a->x * b->w + a->y * b->z - a->z * b->y;
    out->y = a->w * b->y - a->x * b->z + a->y * b->w + a->z * b->x;
    out->z = a->w * b->z + a->x * b->y - a->y * b->x + a->z * b->w;
    out->w = a->w * b->w - a->x * b->x - a->y * b->y - a->z * b->z;
#endif
}

FR_FORCE_INLINE f32 fr_quat_dot(const quat* a, const quat* b) { return fr_vec4_dot(a, b); }

FR_FORCE_INLINE void fr_quat_to_mat4(const quat* q, mat4* out) {
    quat normq;
    fr_quat_normalize(q, &normq);

    out->m00 = 1.0f - 2.0f * (normq.y * normq.y + normq.z * normq.z);
    out->m01 = 2.0f * (normq.x * normq.y - normq.z * normq.w);
    out->m02 = 2.0f * (normq.x * normq.z + normq.y * normq.w);
    out->m03 = 0.0f;

    out->m10 = 2.0f * (normq.x * normq.y + normq.z * normq.w);
    out->m11 = 1.0f - 2.0f * (normq.x * normq.x + normq.z * normq.z);
    out->m12 = 2.0f * (normq.y * normq.z - normq.x * normq.w);
    out->m13 = 0.0f;

    out->m20 = 2.0f * (normq.x * normq.z - normq.y * normq.w);
    out->m21 = 2.0f * (normq.y * normq.z + normq.x * normq.w);
    out->m22 = 1.0f - 2.0f * (normq.x * normq.x + normq.y * normq.y);
    out->m23 = 0.0f;

    out->m30 = 0.0f;
    out->m31 = 0.0f;
    out->m32 = 0.0f;
    out->m33 = 1.0f;
}

FR_FORCE_INLINE void fr_quat_to_mat4t(const quat* q, mat4* out) {
    quat normq;
    fr_quat_normalize(q, &normq);

    out->m00 = 1.0f - 2.0f * (normq.y * normq.y + normq.z * normq.z);
    out->m01 = 2.0f * (normq.x * normq.y + normq.z * normq.w);
    out->m02 = 2.0f * (normq.x * normq.z - normq.y * normq.w);
    out->m03 = 0.0f;

    out->m10 = 2.0f * (normq.x * normq.y - normq.z * normq.w);
    out->m11 = 1.0f - 2.0f * (normq.x * normq.x + normq.z * normq.z);
    out->m12 = 2.0f * (normq.y * normq.z + normq.x * normq.w);
    out->m13 = 0.0f;

    out->m20 = 2.0f * (normq.x * normq.z + normq.y * normq.w);
    out->m21 = 2.0f * (normq.y * normq.z - normq.x * normq.w);
    out->m22 = 1.0f - 2.0f * (normq.x * normq.x + normq.y * normq.y);
    out->m23 = 0.0f;

    out->m30 = 0.0f;
    out->m31 = 0.0f;
    out->m32 = 0.0f;
    out->m33 = 1.0f;
}

FR_FORCE_INLINE void fr_quat_to_rotation_matrix(const quat* q, const vec3* center, mat4* out) {
    out->m00 = (q->x * q->x) - (q->y * q->y) - (q->z * q->z) + (q->w * q->w);
    out->m01 = 2.0f * ((q->x * q->y) + (q->z * q->w));
    out->m02 = 2.0f * ((q->x * q->z) - (q->y * q->w));
    out->m03 = center->x - (center->x * out->m00 + center->y * out->m01 + center->z * out->m02);

    out->m10 = 2.0f * ((q->x * q->y) - (q->z * q->w));
    out->m11 = (-q->x * q->x) + (q->y * q->y) - (q->z * q->z) + (q->w * q->w);
    out->m12 = 2.0f * ((q->y * q->z) + (q->x * q->w));
    out->m13 = center->y - (center->x * out->m10 + center->y * out->m11 + center->z * out->m12);

    out->m20 = 2.0f * ((q->x * q->z) + (q->y * q->w));
    out->m21 = 2.0f * ((q->y * q->z) - (q->x * q->w));
    out->m22 = (-q->x * q->x) - (q->y * q->y) + (q->z * q->z) + (q->w * q->w);
    out->m23 = center->z - (center->x * out->m20 + center->y * out->m21 + center->z * out->m22);

    out->m30 = 0.0f;
    out->m31 = 0.0f;
    out->m32 = 0.0f;
    out->m33 = 1.0f;
}

FR_FORCE_INLINE void fr_quat_from_axis_angle(const vec3* axis, f32 angle, b8 normalize, quat* out) {
    const f32 half_angle = angle * 0.5f;
    const f32 s = fr_sin(half_angle);
    const f32 c = fr_cos(half_angle);

    fr_quat(axis->x * s, axis->y * s, axis->z * s, c, out);
    if (normalize) {
        fr_quat_normalize(out, out);
    }
}

FR_FORCE_INLINE void fr_quat_to_axis_angle(const quat* q, b8 normalize, vec3* out_axis, f32* out_angle) {
    fr_vec3_create(q->data, out_axis);
    const f32 sin_half_angle = fr_vec3_norm(out_axis);
    const f32 cos_half_angle = q->w;
    *out_angle = 2.0f * fr_atan2(sin_half_angle, cos_half_angle);
    if (normalize) {
        fr_vec3_normalize(out_axis, out_axis);
    }
}

FR_FORCE_INLINE void fr_quat_from_eulers321(f32 z, f32 y, f32 x, quat* out) {
    const f32 c1 = fr_cos(x * 0.5f);
    const f32 c2 = fr_cos(y * 0.5f);
    const f32 c3 = fr_cos(z * 0.5f);
    const f32 s1 = fr_sin(x * 0.5f);
    const f32 s2 = fr_sin(y * 0.5f);
    const f32 s3 = fr_sin(z * 0.5f);

    out->x = s1 * c2 * c3 - c1 * s2 * s3;
    out->y = c1 * s2 * c3 + s1 * c2 * s3;
    out->z = c1 * c2 * s3 - s1 * s2 * c3;
    out->w = c1 * c2 * c3 + s1 * s2 * s3;
}

FR_FORCE_INLINE void fr_quat_vec3_rot(const vec3* v, const quat* q, vec3* out) {
    vec3 vec_q, temp;
    fr_quat_imag(q, &vec_q);
    fr_vec3_cross(&vec_q, v, &temp);
    vec3 t_cross;
    fr_vec3_cross(&vec_q, &temp, &t_cross);
    fr_vec3_scale(&temp, 2.0f * q->w, &temp);
    fr_vec3_scale(&t_cross, 2.0f, &t_cross);
    fr_vec3_add(v, &temp, out);
    fr_vec3_add(out, &t_cross, out);
}

FR_FORCE_INLINE void fr_quat_lerp(const quat* from, const quat* to, f32 percentage, quat* out) {
    fr_vec4_lerp(from, to, percentage, out);
}

FR_FORCE_INLINE void fr_quat_nlerp(const quat* from, const quat* to, f32 percentage, quat* out) {
    f32 dot = fr_quat_dot(from, to);
    quat target;
    fr_vec4_scale(to, (dot >= 0.0f) ? 1.0f : -1.0f, &target);
    fr_quat_lerp(from, &target, percentage, out);
    fr_quat_normalize(out, out);
}

FR_FORCE_INLINE void fr_quat_slerp(const quat* from, const quat* to, f32 percentage, quat* out) {
    f32 dot = fr_quat_dot(from, to);
    if (fr_abs(dot) >= 1.0f) {
        fr_quat_copy(from, out);
        return;
    }
    quat target;
    if (dot < 0.0f) {
        fr_vec4_scale(to, -1.0f, &target);
        dot = -dot;
    } else {
        fr_quat_copy(to, &target);
    }

    f32 sinTheta = fr_sqrt(1.0f - dot * dot);

    if (fr_abs(sinTheta) < 0.001f) {
        fr_quat_lerp(from, &target, percentage, out);
        return;
    }

    f32 angle = fr_atan2(sinTheta, dot);
    fr_vec4_scale(&target, fr_sin((1.0f - percentage) * angle), &target);
    fr_vec4_scale(to, fr_sin(percentage * angle), out);
    fr_vec4_add(&target, out, out);
    fr_vec4_scale(out, 1.0f / sinTheta, out);
}
