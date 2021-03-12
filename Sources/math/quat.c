#include "quat.h"

#include "mat4.h"
#include "vec4.h"

#include <kinc/math/core.h>

void quat_set(quat_t* self,float x,float y, float z, float w){
    self->x = x;
    self->y = y;
    self->z = z;
    self->w = w;
}

void quat_add(quat_t* self,quat_t* other){
    self->x += other->x;
    self->y += other->y;
    self->z += other->z;
    self->w += other->w;
}

void quat_sub(quat_t* self,quat_t* other){
    self->x -= other->x;
    self->y -= other->y;
    self->z -= other->z;
    self->w -= other->w;
}

void fromAxisAngle(quat_t* self, vec4_t* axis, float angle) {
    float s = kinc_sin(angle * 0.5);
    self->x = axis->x * s;
    self->y = axis->y * s;
    self->z = axis->z * s;
    self->w = kinc_cos(angle * 0.5);
    quat_normalize(self);
}

float toAxisAngle(quat_t* self,vec4_t* axis) {
    quat_normalize(self);
    float angle = 2 * kinc_acos(self->w);
    float s = kinc_sqrt(1 - self->w * self->w);
    if (s < 0.001) {
        axis->x = self->x;
        axis->y = self->y;
        axis->z = self->z;
    }
    else {
        axis->x = self->x / s;
        axis->y = self->y / s;
        axis->z = self->z / s;
    }
    return angle;
}

void quat_fromMat(quat_t* self,mat4_t* m) {
    mat4_t helpMat = {0};
    mat4_setFrom(&helpMat,m);
    mat4_toRotation(&helpMat);
    quat_fromRotationMat(self,&helpMat);
}

void quat_fromRotationMat(quat_t* self,mat4_t* rotation) {
    mat4_t m = *rotation;
    // Assumes the upper 3x3 is a pure rotation matrix
    float m11 = m._00; float m12 = m._10; float m13 = m._20;
    float m21 = m._01; float m22 = m._11; float m23 = m._21;
    float m31 = m._02; float m32 = m._12; float m33 = m._22;
    float tr = m11 + m22 + m33;
    float s = 0.0;

    if (tr > 0) {
        s = 0.5 / kinc_sqrt(tr + 1.0);
        self->w = 0.25 / s;
        self->x = (m32 - m23) * s;
        self->y = (m13 - m31) * s;
        self->z = (m21 - m12) * s;
    }
    else if (m11 > m22 && m11 > m33) {
        s = 2.0 * kinc_sqrt(1.0 + m11 - m22 - m33);
        self->w = (m32 - m23) / s;
        self->x = 0.25 * s;
        self->y = (m12 + m21) / s;
        self->z = (m13 + m31) / s;
    }
    else if (m22 > m33) {
        s = 2.0 * kinc_sqrt(1.0 + m22 - m11 - m33);
        self->w = (m13 - m31) / s;
        self->x = (m12 + m21) / s;
        self->y = 0.25 * s;
        self->z = (m23 + m32) / s;
    }
    else {
        s = 2.0 * kinc_sqrt(1.0 + m33 - m11 - m22);
        self->w = (m21 - m12) / s;
        self->x = (m13 + m31) / s;
        self->y = (m23 + m32) / s;
        self->z = 0.25 * s;
    }
}

void quat_scale(quat_t* self,float s){
    self->x *= s;
    self->y *= s;
    self->z *= s;
    self->w *= s;
}

/**
    Multiply two quaternions and store the result in the first one.
    @param	q1 The first operand.
    @param	q2 The second operand.
**/
void quat_mult(quat_t* q1,quat_t* q2) {
    float q1x = q1->x; float q1y = q1->y; float q1z = q1->z; float q1w = q1->w;
    float q2x = q2->x; float q2y = q2->y; float q2z = q2->z; float q2w = q2->w;
    q1->x = q1x * q2w + q1w * q2x + q1y * q2z - q1z * q2y;
    q1->y = q1w * q2y - q1x * q2z + q1y * q2w + q1z * q2x;
    q1->z = q1w * q2z + q1x * q2y - q1y * q2x + q1z * q2w;
    q1->w = q1w * q2w - q1x * q2x - q1y * q2y - q1z * q2z;
}

// Module
float quat_module(quat_t* self) {
    return kinc_sqrt(self->x * self->x + self->y * self->y + self->z * self->z + self->w * self->w);
}

/**
    Scale this quaternion to have a magnitude of 1.
    @param self This quaternion
**/
void quat_normalize(quat_t* self) {
    float l = kinc_sqrt(self->x * self->x + self->y * self->y + self->z * self->z + self->w * self->w);
    if (l == 0.0) {
        self->x = 0;
        self->y = 0;
        self->z = 0;
        self->w = 0;
    }
    else {
        l = 1.0 / l;
        self->x *= l;
        self->y *= l;
        self->z *= l;
        self->w *= l;
    }
}

/**
    Copy the rotation of another quaternion to this one.
    @param self This quaternion
    @param	q A quaternion to copy.
**/
void quat_setFrom(quat_t* self,quat_t* q) {
    self->x = q->x;
    self->y = q->y;
    self->z = q->z;
    self->w = q->w;
}

/**
     Convert this quaternion to a YZX Euler.
    @return	A new YZX Euler that represents the same rotation as this
            quaternion.
**/
void quat_getEuler(quat_t* self,vec4_t* out) {
    float a = -2 * (self->x * self->z - self->w * self->y);
    float b =  self->w *  self->w + self->x * self->x - self->y * self->y - self->z * self->z;
    float c =  2 * (self->x * self->y + self->w * self->z);
    float d = -2 * (self->y * self->z - self->w * self->x);
    float e =  self->w *  self->w - self->x * self->x + self->y * self->y - self->z * self->z;
    vec4_set3(out,kinc_atan2(d, e), kinc_atan2(a, b), kinc_asin(c));
}

/**
     Set this quaternion to the rotation represented by a YZX Euler.
    @param	x The Euler's x component.
    @param	y The Euler's y component.
    @param	z The Euler's z component.
    @return	This quaternion.
**/
void quat_fromEuler(quat_t* self,float x,float y,float z) {
    float f = x / 2;
    float c1 = kinc_cos(f);
    float s1 = kinc_sin(f);
    f = y / 2;
    float c2 = kinc_cos(f);
    float s2 = kinc_sin(f);
    f = z / 2;
    float c3 = kinc_cos(f);
    float s3 = kinc_sin(f);
    // YZX
    self->x = s1 * c2 * c3 + c1 * s2 * s3;
    self->y = c1 * s2 * c3 + s1 * c2 * s3;
    self->z = c1 * c2 * s3 - s1 * s2 * c3;
    self->w = c1 * c2 * c3 - s1 * s2 * s3;
}

/**
     Linearly interpolate between two other quaterions, and store the
    result in this one. This is not a so-called slerp operation.
    @param	from The quaterion to interpolate from.
    @param	to The quaterion to interpolate to.
    @param	s The amount to interpolate, with 0 being `from` and 1 being
            `to`, and 0.5 being half way between the two.
    @return	This quaternion.
**/
void quat_lerp(quat_t* self,quat_t* from,quat_t* to, float s) {
    float fromx = from->x;
    float fromy = from->y;
    float fromz = from->z;
    float fromw = from->w;
    float dot = quat_dot(from,to);
    if (dot < 0.0) {
        fromx = -fromx;
        fromy = -fromy;
        fromz = -fromz;
        fromw = -fromw;
    }
    self->x = fromx + (to->x - fromx) * s;
    self->y = fromy + (to->y - fromy) * s;
    self->z = fromz + (to->z - fromz) * s;
    self->w = fromw + (to->w - fromw) * s;
    quat_normalize(self);
}

// Slerp is shorthand for spherical linear interpolation
void quat_slerp(quat_t* self,quat_t* from, quat_t* to, float t) {
    float epsilon = 0.0005;
    
    float dot = quat_dot(from,to);
    if (dot > 1 - epsilon) {
        quat_t result = {0};
        memcpy(&result,from,sizeof(quat_t));
        quat_sub(&result,to);
        quat_scale(&result,t);
        quat_add(&result,to);
        quat_normalize(&result);
        memcpy(self,&result,sizeof(quat_t));
        return;
    }
    if (dot < 0) dot = 0;
    if (dot > 1) dot = 1;

    float theta0 = kinc_acos(dot);
    float theta = theta0 * t;
    quat_scale(self,dot);
    quat_t q2 = {0};
    memcpy(&q2,to,sizeof(quat_t));
    quat_sub(&q2,self);
    quat_normalize(&q2);
    quat_scale(self,kinc_cos(theta));
    quat_scale(&q2,kinc_sin(theta));
    quat_add(self,&q2);
    quat_normalize(self);
}

/**
     Find the dot product of this quaternion with another.
    @param	q The other quaternion.
    @return	The dot product.
**/
float quat_dot(quat_t* self, quat_t* q) {
    return (self->x * q->x) + (self->y * q->y) + (self->z * q->z) + (self->w * q->w);
}


void quat_fromTo(quat_t* self, vec4_t* v1,vec4_t* v2) {
    // Rotation formed by direction vectors
    // v1 and v2 should be normalized first
    vec4_t a = {0};
    a.w = 1.0;
    float dot = vec4_dot(v1,v2);
    if (dot < -0.999999) {
        a.x = 1.0;
        vec4_cross(&a,v1);
        float len = vec4_length(&a);
        if (len < 0.000001){
            a.x = a.z = 0.0f; a.w = a.y = 1.0f;
            vec4_cross(&a,v1);
        } 
        vec4_normalize(&a);
        fromAxisAngle(self,&a, KINC_PI);
    }
    else if (dot > 0.999999) {
        quat_set(self,0, 0, 0, 1);
    }
    else {
        memcpy(&a, v1,sizeof(vec4_t));
        vec4_cross(&a,v2);
        quat_set(self,a.x, a.y, a.z, 1 + dot);
        quat_normalize(self);
    }
}

