#include "utils.h"
#include <math.h>
#include <stdlib.h>

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

f32 fr_random() { return (f32)rand() / (f32)RAND_MAX; }