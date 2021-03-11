#pragma once

#include "../pch.h"
#include "kinc/math/core.h"

typedef struct vec4 {
    float x;
    float y;
    float z;
    float w;
} vec4_t;

KINC_FUNC inline void vec4_cross(vec4_t* self,vec4_t* other){
    float ax = self->x; float ay = self->y; float az = self->z;
	float bx = other->x; float by = other->y; float bz = other->z;

    self->x = ay * bz - az * by;
    self->y = az * bx - ax * bz;
    self->z = ax * by - ay * bx;
}

KINC_FUNC inline void vec4_set(vec4_t* self,float x,float y, float z, float w){
    self->x = x;
    self->y = y;
    self->z = z;
    self->w = w;
}

KINC_FUNC inline void vec4_set(vec4_t* self,float x,float y, float z){
    self->x = x;
    self->y = y;
    self->z = z;
    self->w = 1.0;
}

KINC_FUNC inline void vec4_add(vec4_t* self,vec4_t* other){
    self->x += other->x;
    self->y += other->y;
    self->z += other->z;
}

KINC_FUNC inline void vec4_addf(vec4_t* self,float x,float y, float z){
    self->x += x;
    self->y += y;
    self->z += z;
}

KINC_FUNC inline void vec4_sub(vec4_t* self,vec4_t* other){
    self->x -= other->x;
    self->y -= other->y;
    self->z -= other->z;
}

KINC_FUNC inline void vec4_normalize(vec4_t* self){
    float n = vec4_length(self);
    if(n > 0.0){
        float invN = 1.0 / n;
        self->x *= invN;
        self->y *= invN;
        self->z *= invN;
    }
}

KINC_FUNC inline void vec4_mult(vec4_t* self, float value){
    self->x *= value;
    self->y *= value;
    self->z *= value;
}

KINC_FUNC inline float vec4_dot(vec4_t* self, vec4_t* v){
    return self->x * v->x + self->y * v->y + self->z * v->z;
}

KINC_FUNC inline void vec4_clone(vec4_t* from,vec4_t* to){
    memcpy(to,from,sizeof(vec4_t));
}

KINC_FUNC inline void vec4_lerp(vec4_t* self,vec4_t* from,vec4_t* to,float s){
    self->x = from->x + (to->x - from->x) * s;
    self->y = from->y + (to->y - from->y) * s;
    self->z = from->z + (to->z - from->z) * s;
}

KINC_FUNC inline float vec4_length(vec4_t* self){
    return kinc_sqrt(self->x * self->x + self->y * self->y + self->z * self->z);
}