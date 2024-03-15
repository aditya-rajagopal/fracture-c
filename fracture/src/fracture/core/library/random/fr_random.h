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

FR_API void fr_random_pcg32_init(u32 seed);
FR_API u32 fr_random_pcg32();
FR_API f32 fr_random_pcg32f();
FR_API f32 fr_random_pcg32_range(f32 min, f32 max);

FR_API void fr_random_xorwow_init(u32 seed);
FR_API u32 fr_random_xorwow();
FR_API f32 fr_random_xorwowf();
FR_API f32 fr_random_xorwow_range(f32 min, f32 max);
