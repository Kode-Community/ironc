#include "vec3.h"

void vec3_cross(vec3_t* self,vec3_t* other){
    float ax = self->x; float ay = self->y; float az = self->z;
	float bx = other->x; float by = other->y; float bz = other->z;

    self->x = ay * bz - az * by;
    self->y = az * bx - ax * bz;
    self->z = ax * by - ay * bx;
}

void vec3_set(vec3_t* self,float x,float y, float z){
    self->x = x;
    self->y = y;
    self->z = z;
}

void vec3_add(vec3_t* self,vec3_t* other){
    self->x += other->x;
    self->y += other->y;
    self->z += other->z;
}

void vec3_addf(vec3_t* self,float x,float y, float z){
    self->x += x;
    self->y += y;
    self->z += z;
}

void vec3_sub(vec3_t* self,vec3_t* other){
    self->x -= other->x;
    self->y -= other->y;
    self->z -= other->z;
}

void vec3_normalize(vec3_t* self){
    float n = vec3_length(self);
    if(n > 0.0){
        float invN = 1.0f / n;
        self->x *= invN;
        self->y *= invN;
        self->z *= invN;
    }
}

void vec3_mult(vec3_t* self, float value){
    self->x *= value;
    self->y *= value;
    self->z *= value;
}

float vec3_dot(vec3_t* self, vec3_t* v){
    return self->x * v->x + self->y * v->y + self->z * v->z;
}

void vec3_clone(vec3_t* from,vec3_t* to){
    memcpy(to,from,sizeof(vec3_t));
}

void vec3_lerp(vec3_t* self,vec3_t* from,vec3_t* to,float s){
    self->x = from->x + (to->x - from->x) * s;
    self->y = from->y + (to->y - from->y) * s;
    self->z = from->z + (to->z - from->z) * s;
}

float vec3_length(vec3_t* self){
    return kinc_sqrt(self->x * self->x + self->y * self->y + self->z * self->z);
}