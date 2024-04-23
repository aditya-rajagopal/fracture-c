#include "fr_random.h"

static u64 const multiplier = 6364136223846793005u;
static u64 const increment = 1442695040888963407u;

static u32 _rotr32(u32 x, u32 r);

typedef struct pcg32_state_t {
    u64 state;
} pcg32_internal_state;

b8 fr_random_pcg32_init(u32 seed, fr_rng_config* config, u32* memory_requirement) {
    if (!config) {
        *memory_requirement = sizeof(pcg32_internal_state);
        return FALSE;
    }
    FR_ASSERT(config->type == FR_RNG_PCG32);
    pcg32_internal_state* state = (pcg32_internal_state*)config->state;
    state->state = seed + increment;
    (void)fr_random_pcg32(config);
    return TRUE;
}

u32 fr_random_pcg32(void* config) {
    FR_ASSERT(config != NULL_PTR);
    fr_rng_config* cfg = (fr_rng_config*)config;
    FR_ASSERT(cfg->type == FR_RNG_PCG32);

    pcg32_internal_state* state = (pcg32_internal_state*)cfg->state;
    u64 oldstate = state->state;
    unsigned count = (unsigned)(oldstate >> 59);  // 59 = 64 - 5

    state->state = oldstate * multiplier + increment;
    oldstate ^= oldstate >> 18;  // 18 = (64 - 27)/2
    return _rotr32((u32)(oldstate >> 27), count);
}

f32 fr_random_pcg32f(void* config) { return (f32)fr_random_pcg32(config) / (f32)0xFFFFFFFF; }

f32 fr_random_pcg32_range(void* config, f32 min, f32 max) { return min + (max - min) * fr_random_pcg32f(config); }

i32 fr_random_pcg32_rangei(void* config, i32 min, i32 max) { return min + fr_random_pcg32(config) % (max - min); }

i32 fr_random_pcg32_i0N(void* config, i32 n) { return fr_random_pcg32(config) % n; }

//---------------------------------------------------------------------
// Private functions
//---------------------------------------------------------------------

static u32 _rotr32(u32 x, u32 r) { return (x >> r) | (x << (-r & 31)); }
