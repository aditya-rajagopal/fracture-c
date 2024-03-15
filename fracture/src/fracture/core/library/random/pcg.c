#include "fr_random.h"

static u32 state = 0;
static u64 const multiplier = 6364136223846793005u;
static u64 const increment  = 1442695040888963407u;

static u32 _rotr32(u32 x, u32 r);

void fr_random_pcg32_init(u32 seed) {
    state = seed + increment;
    (void)fr_random_pcg32();
}

u32 fr_random_pcg32(){
    u64 oldstate = state;
    unsigned count = (unsigned)(oldstate >> 59);  // 59 = 64 - 5

    state = oldstate * multiplier + increment;
    oldstate ^= oldstate >> 18;  // 18 = (64 - 27)/2
    return _rotr32((u32)(oldstate >> 27), count);
}

f32 fr_random_pcg32f() {
    return (f32)fr_random_pcg32() / (f32)0xFFFFFFFF;
}

f32 fr_random_pcg32_range(f32 min, f32 max) {
    return min + (max - min) * fr_random_pcg32f();
}

i32 fr_random_pcg32_rangei(i32 min, i32 max) {
    return min + (i32)(fr_random_pcg32f() * (max - min));
}

//---------------------------------------------------------------------
// Private functions
//---------------------------------------------------------------------

static u32 _rotr32(u32 x, u32 r) {
    return (x >> r) | (x << (-r & 31));
}
