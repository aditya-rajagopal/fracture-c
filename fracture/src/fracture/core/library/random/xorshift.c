#include "fr_random.h"

typedef struct xorwow_state_t {
    u32 x[5];
    u32 counter;
} xorwow_state;

static xorwow_state xorwow = {0};

void fr_random_xorwow_init(u32 seed) {
    xorwow.x[0] = seed;
    xorwow.x[1] = seed + 0x9e3779b9;
    xorwow.x[2] = seed + 0x9e3779b9 * 2;
    xorwow.x[3] = seed + 0x9e3779b9 * 3;
    xorwow.x[4] = seed + 0x9e3779b9 * 4;
    xorwow.counter = 362436;
}

u32 fr_random_xorwow() {
    u32 t = (xorwow.x[0] ^ (xorwow.x[0] << 11));

    xorwow.x[0] = xorwow.x[1];
    xorwow.x[1] = xorwow.x[2];
    xorwow.x[2] = xorwow.x[3];
    xorwow.x[3] = xorwow.x[4];
    xorwow.x[4] = (xorwow.x[4] ^ (xorwow.x[4] >> 19)) ^ (t ^ (t >> 8));

    xorwow.counter += 362437;

    return xorwow.x[4] + xorwow.counter;
}

f32 fr_random_xorwowf() {
    return (f32)fr_random_xorwow() / (f32)0xFFFFFFFF;
}

f32 fr_random_xorwow_range(f32 min, f32 max) {
    return min + (max - min) * fr_random_xorwowf();
}