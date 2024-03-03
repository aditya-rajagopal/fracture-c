/**
 * @file vec2.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-02-29
 * 
 * @copyright Fracture Game Engine is Copyright (c) Aditya Rajagopal 2024-2024
 * 
 */
#pragma once

#include "fracture/core/defines.h"

typedef union vec2_u {
    struct {
       union {
           f32 x;
           f32 u;
           f32 r;
           f32 s;
       };
       union {
           f32 y;
           f32 v;
           f32 i;
           f32 t;
       };
    };
    f32 data[2];
} vec2;

typedef union ivec2_u {
    struct {
       union {
           i32 x;
           i32 u;
           i32 r;
           i32 s;
       };
       union {
           i32 y;
           i32 v;
           i32 i;
           i32 t;
       };
    };
    i32 data[2];
} ivec2;

typedef union uvec2_u {
    struct {
       union {
           u32 x;
           u32 u;
           u32 r;
           u32 s;
       };
       union {
           u32 y;
           u32 v;
           u32 i;
           u32 t;
       };
    };
    u32 data[2];
} uvec2;

typedef union dvec2_u {
    struct {
       union {
           f64 x;
           f64 u;
           f64 r;
           f64 s;
       };
       union {
           f64 y;
           f64 v;
           f64 i;
           f64 t;
       };
    };
    f64 data[2];
} dvec2;

// TODO: Add i8vec2, i16vec2, i64vec2, u8vec2, u16vec2, u64vec2, bvec2
