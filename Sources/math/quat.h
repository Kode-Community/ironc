#pragma once

#include "../pch.h"

typedef struct quat {
    float x;
    float y;
    float z;
    float w;
} quat_t;

struct mat4_t;
struct vec4_t;

KINC_FUNC void quat_set(quat_t* self,float x,float y, float z, float w);
KINC_FUNC void quat_add(quat_t* self,quat_t* other);
KINC_FUNC void quat_sub(quat_t* self,quat_t* other);
KINC_FUNC void fromAxisAngle(quat_t* self, struct vec4_t* axis, float angle);
KINC_FUNC float toAxisAngle(quat_t* self,struct vec4_t* axis);
KINC_FUNC void quat_fromMat(quat_t* self,struct mat4_t* m);
KINC_FUNC void quat_fromRotationMat(quat_t* self,struct mat4_t* rotation);
// Multiply this quaternion by float.
KINC_FUNC void quat_scale(quat_t* self,float s);
/**
    Multiply two quaternions and store the result in the first one.
    @param	q1 The first operand.
    @param	q2 The second operand.
**/
KINC_FUNC void quat_mult(quat_t* q1,quat_t* q2);
// Module
KINC_FUNC float quat_module(quat_t* self);
/**
    Scale this quaternion to have a magnitude of 1.
    @param self This quaternion
**/
KINC_FUNC void quat_normalize(quat_t* self);
/**
    Copy the rotation of another quaternion to this one.
    @param self This quaternion
    @param	q A quaternion to copy.
**/
KINC_FUNC void quat_setFrom(quat_t* self,quat_t* q);
/**
     Convert this quaternion to a YZX Euler.
    @return	A new YZX Euler that represents the same rotation as this
            quaternion.
**/
KINC_FUNC void quat_getEuler(quat_t* self,struct vec4_t* out);
/**
     Set this quaternion to the rotation represented by a YZX Euler.
    @param	x The Euler's x component.
    @param	y The Euler's y component.
    @param	z The Euler's z component.
    @return	This quaternion.
**/
KINC_FUNC void quat_fromEuler(quat_t* self,float x,float y,float z);
/**
     Linearly interpolate between two other quaterions, and store the
    result in this one. This is not a so-called slerp operation.
    @param	from The quaterion to interpolate from.
    @param	to The quaterion to interpolate to.
    @param	s The amount to interpolate, with 0 being `from` and 1 being
            `to`, and 0.5 being half way between the two.
    @return	This quaternion.
**/
KINC_FUNC void quat_lerp(quat_t* self,quat_t* from,quat_t* to, float s);
// Slerp is shorthand for spherical linear interpolation
KINC_FUNC void quat_slerp(quat_t* self,quat_t* from, quat_t* to, float t);
/**
     Find the dot product of this quaternion with another.
    @param self This quaternion
    @param	q The other quaternion.
    @return	The dot product.
**/
KINC_FUNC float quat_dot(quat_t* self, quat_t* q);
void quat_fromTo(quat_t* self, struct vec4_t* v1,struct vec4_t* v2);