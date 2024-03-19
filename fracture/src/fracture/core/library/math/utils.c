#include "utils.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

f32 fr_sqrt(f32 val) { return sqrtf(val); }
f32 fr_inv_sqrt(f32 val) { return 1.0f / sqrtf(val); }
f32 fr_exp(f32 val) { return expf(val); }
f32 fr_log(f32 val) { return logf(val); }
f32 fr_log2(f32 val) { return log2f(val); }
f32 fr_log10(f32 val) { return log10f(val); }
f32 fr_pow(f32 base, f32 exp) { return powf(base, exp); }
f32 fr_sin(f32 val) { return sinf(val); }
f32 fr_cos(f32 val) { return cosf(val); }
f32 fr_tan(f32 val) { return tanf(val); }
f32 fr_asin(f32 val) { return asinf(val); }
f32 fr_acos(f32 val) { return acosf(val); }
f32 fr_atan(f32 val) { return atanf(val); }
f32 fr_atan2(f32 y, f32 x) { return atan2f(y, x); }
f32 fr_sinh(f32 val) { return sinhf(val); }
f32 fr_cosh(f32 val) { return coshf(val); }
f32 fr_tanh(f32 val) { return tanhf(val); }
f32 fr_asinh(f32 val) { return asinhf(val); }
f32 fr_acosh(f32 val) { return acoshf(val); }
f32 fr_atanh(f32 val) { return atanhf(val); }

i32 fr_next_pow2(i32 val) {
    val--;
    val |= val >> 1;
    val |= val >> 2;
    val |= val >> 4;
    val |= val >> 8;
    val |= val >> 16;
    val++;
    return val;
}

i32 fr_prev_pow2(i32 val) {
    val--;
    val |= val >> 1;
    val |= val >> 2;
    val |= val >> 4;
    val |= val >> 8;
    val |= val >> 16;
    return val - (val >> 1);
}

void fr_mat2_print(const char* name, int* len, const mat2* m, char* out_string) {
    char buffer[1024];
    u32 offset = 0;
    offset = snprintf(buffer, 1024, "mat2(float:%s) = \n", name);
    offset += snprintf(buffer + offset, 1024 - offset, "| %f, %f |\n", m->m00, m->m01);
    offset += snprintf(buffer + offset, 1024 - offset, "| %f, %f |\n", m->m10, m->m11);
    if (out_string == NULL_PTR) {
        *len = offset;
        return;
    }
    snprintf(out_string, *len, "%s", buffer);
}

void fr_mat3_print(const char* name, int* len, const mat3* m, char* out_string) {
    char buffer[1024];
    u32 offset = 0;
    offset = snprintf(buffer, 1024, "mat3(float:%s) = \n", name);
    offset += snprintf(buffer + offset, 1024 - offset, "| %f, %f, %f |\n", m->m00, m->m01, m->m02);
    offset += snprintf(buffer + offset, 1024 - offset, "| %f, %f, %f |\n", m->m10, m->m11, m->m12);
    offset += snprintf(buffer + offset, 1024 - offset, "| %f, %f, %f |\n", m->m20, m->m21, m->m22);
    if (out_string == NULL_PTR) {
        *len = offset;
        return;
    }
    snprintf(out_string, *len, "%s", buffer);
}

FR_API void fr_mat4_print(const char* name, int* len, const mat4* m, char* out_string) {
    char buffer[1024];
    u32 offset = 0;
    offset = snprintf(buffer, 1024, "mat4(float:%s) = \n", name);
    offset += snprintf(buffer + offset, 1024 - offset, "| %f, %f, %f, %f |\n", m->m00, m->m01, m->m02, m->m03);
    offset += snprintf(buffer + offset, 1024 - offset, "| %f, %f, %f, %f |\n", m->m10, m->m11, m->m12, m->m13);
    offset += snprintf(buffer + offset, 1024 - offset, "| %f, %f, %f, %f |\n", m->m20, m->m21, m->m22, m->m23);
    offset += snprintf(buffer + offset, 1024 - offset, "| %f, %f, %f, %f |\n", m->m30, m->m31, m->m32, m->m33);
    if (out_string == NULL_PTR) {
        *len = offset;
        return;
    }
    snprintf(out_string, *len, "%s", buffer);
}
