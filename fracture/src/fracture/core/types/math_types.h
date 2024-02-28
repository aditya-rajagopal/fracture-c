/**
 * @file math_types.h
 * @author Aditya Rajagopal
 * @brief 
 * @version 0.0.1
 * @date 2024-02-26
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
       };
       union {
           f32 y;
           f32 v;
       };
    };
    f32 data[2];
} vec2_u;

typedef union vec3_u {
    struct {
       union {
           f32 x;
           f32 r;
           f32 u;
       };
       union {
           f32 y;
           f32 g;
           f32 v;
       };
       union {
           f32 z;
           f32 b;
           f32 w;
       };
    };
    f32 data[3];
} vec3_u;

typedef union vec4_u {
    struct {
       union {
           f32 x;
           f32 r;
           f32 s;
       };
       union {
           f32 y;
           f32 g;
           f32 t;
       };
       union {
           f32 z;
           f32 b;
           f32 p;
           f32 width;
       };
       union {
           f32 w;
           f32 a;
           f32 q;
           f32 height;
       };
    };
    f32 data[4];
} vec4_u;

// 2D rectangle
typedef vec4_u rect_2d;

typedef vec4_u colour;