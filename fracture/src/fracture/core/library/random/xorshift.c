#include "fr_random.h"

typedef struct xorwow_state_t {
    u32 x[5];
    u32 counter;
} xorwow_internal_state;

b8 fr_random_xorwow_init(u32 seed, fr_rng_config* config, u32* memory_requirement) {
    if (!config) {
        *memory_requirement = sizeof(xorwow_internal_state);
        return FALSE;
    }
    FR_ASSERT(config->type == FR_RNG_XORWOW);
    xorwow_internal_state* state = (xorwow_internal_state*)config->state;
    state->x[0] = seed;
    state->x[1] = seed + 0x9e3779b9;
    state->x[2] = seed + 0x9e3779b9 * 2;
    state->x[3] = seed + 0x9e3779b9 * 3;
    state->x[4] = seed + 0x9e3779b9 * 4;
    state->counter = 362436;
    (void)fr_random_xorwow(config);
    return TRUE;
}

u32 fr_random_xorwow(void* config) {
    FR_ASSERT(config != NULL_PTR);
    fr_rng_config* cfg = (fr_rng_config*)config;
    FR_ASSERT(cfg->type == FR_RNG_XORWOW);
    xorwow_internal_state* state = (xorwow_internal_state*)cfg->state;
    u32 t = (state->x[0] ^ (state->x[0] << 11));

    state->x[0] = state->x[1];
    state->x[1] = state->x[2];
    state->x[2] = state->x[3];
    state->x[3] = state->x[4];
    state->x[4] = (state->x[4] ^ (state->x[4] >> 19)) ^ (t ^ (t >> 8));

    state->counter += 362437;

    return state->x[4] + state->counter;
}

// u32 fr_random_xor128(rng_config* state) {
//     u32 t  = state->x[3];

//     u32 s  = state->x[0];  /* Perform a contrived 32-bit shift. */
// 	state->x[3] = state->x[2];
// 	state->x[2] = state->x[1];
// 	state->x[1] = s;

// 	t ^= t << 11;
// 	t ^= t >> 8;
// 	return state->x[0] = t ^ s ^ (s >> 19);
// }

f32 fr_random_xorwowf(void* config) { return (f32)fr_random_xorwow(config) / (f32)0xFFFFFFFF; }

// f32 fr_random_xorwowf_2(rng_config* state) {
//     return (f32)fr_random_xorwow2(state) / (f32)0xFFFFFFFF;
// }

f32 fr_random_xorwow_range(void* config, f32 min, f32 max) { return min + (max - min) * fr_random_xorwowf(config); }

i32 fr_random_xorwow_rangei(void* config, i32 min, i32 max) {
    return min + (fr_random_xorwow(config) >> 16) % (max - min);
}

i32 fr_random_xorwow_i0N(void* config, i32 n) { return (fr_random_xorwow(config) >> 16) % n; }
