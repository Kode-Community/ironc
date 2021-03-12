#pragma once

#include "../pch.h"

typedef struct vec4 {
    float x;
    float y;
    float z;
    float w;
} vec4_t;

KINC_FUNC void vec4_cross(vec4_t* self,vec4_t* other);
KINC_FUNC void vec4_set(vec4_t* self,float x,float y, float z, float w);
KINC_FUNC void vec4_set3(vec4_t* self,float x,float y, float z);
KINC_FUNC void vec4_add(vec4_t* self,vec4_t* other);
KINC_FUNC void vec4_addf(vec4_t* self,float x,float y, float z);
KINC_FUNC void vec4_sub(vec4_t* self,vec4_t* other);
KINC_FUNC void vec4_normalize(vec4_t* self);
KINC_FUNC void vec4_mult(vec4_t* self, float value);
KINC_FUNC float vec4_dot(vec4_t* self, vec4_t* v);
KINC_FUNC void vec4_clone(vec4_t* from,vec4_t* to);
KINC_FUNC void vec4_lerp(vec4_t* self,vec4_t* from,vec4_t* to,float s);
KINC_FUNC float vec4_length(vec4_t* self);