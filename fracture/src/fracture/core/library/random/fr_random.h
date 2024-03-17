/**
 * @file fr_random.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-03-15
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include "fracture/fracture_core.h"

typedef enum fr_rng_type {
    FR_RNG_UNKNOWN = 0,
    FR_RNG_PCG32,
    FR_RNG_XORWOW,
    FR_RNG_TOTAL
} fr_rng_type;

typedef struct fr_rng_config{
    void* state;
    fr_rng_type type;
} fr_rng_config;

FR_API b8 fr_random_pcg32_init(u32 seed, fr_rng_config* config, u32* memory_required);
FR_API u32 fr_random_pcg32(void* config);
FR_API f32 fr_random_pcg32f(void* config);
FR_API f32 fr_random_pcg32_range(void* state, f32 min, f32 max);
FR_API i32 fr_random_pcg32_rangei(void* state, i32 min, i32 max); 
FR_API i32 fr_random_pcg32_i0N(void* state, i32 n); 

FR_API b8 fr_random_xorwow_init(u32 seed, fr_rng_config* config, u32* memory_required);
FR_API u32 fr_random_xorwow(void* config);
FR_API f32 fr_random_xorwowf(void* config);
FR_API f32 fr_random_xorwow_range(void* config, f32 min, f32 max);
FR_API i32 fr_random_xorwow_rangei(void* config, i32 min, i32 max);
FR_API i32 fr_random_xorwow_i0N(void* config, i32 n);

#ifdef _RNG_XORWOW
    #define fr_random_init fr_random_xorwow_init
    #define fr_random_uniform fr_random_xorwowf
    #define fr_random_uniform_range fr_random_xorwow_range
    #define fr_random_uniform_rangei fr_random_xorwow_rangei
    #define fr_random_uniform_i0N fr_random_xorwow_i0N
#elif defined(_RNG_PCG32)
    #define fr_random_init fr_random_pcg32_init
    #define fr_random_uniform fr_random_pcg32f
    #define fr_random_uniform_range fr_random_pcg32_range
    #define fr_random_uniform_rangei fr_random_pcg32_rangei
    #define fr_random_uniform_i0N fr_random_pcg32_i0N
#else
    #error "No RNG defined"
#endif

