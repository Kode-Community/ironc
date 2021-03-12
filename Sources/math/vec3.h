
#pragma once

#include "../pch.h"
#include "kinc/math/core.h"

typedef struct vec3 {
    float x;
    float y;
    float z;
    float w;
} vec3_t;

KINC_FUNC void vec3_cross(vec3_t* self,vec3_t* other);
KINC_FUNC void vec3_set(vec3_t* self,float x,float y, float z);
KINC_FUNC void vec3_add(vec3_t* self,vec3_t* other);
KINC_FUNC void vec3_addf(vec3_t* self,float x,float y, float z);
KINC_FUNC void vec3_sub(vec3_t* self,vec3_t* other);
KINC_FUNC void vec3_normalize(vec3_t* self);
KINC_FUNC void vec3_mult(vec3_t* self, float value);
KINC_FUNC float vec3_dot(vec3_t* self, vec3_t* v);
KINC_FUNC void vec3_clone(vec3_t* from,vec3_t* to);
KINC_FUNC void vec3_lerp(vec3_t* self,vec3_t* from,vec3_t* to,float s);
KINC_FUNC float vec3_length(vec3_t* self);