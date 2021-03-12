#pragma once 

#include "../pch.h"

#include "kinc/math/matrix.h"

#ifdef __cplusplus
extern "C" {
#endif

struct vec4_t;
struct vec3_t;
struct quat_t;

typedef struct mat4 {
   float _00;
   float _10;
   float _20;
   float _30;
   float _01;
   float _11;
   float _21;
   float _31;
   float _02;
   float _12;
   float _22;
   float _32;
   float _03;
   float _13;
   float _23;
   float _33;
} mat4_t;

//Kha Matrix4

/**
    Reset this matrix to the identity and set its location.
    @param self The matrix to set
    @param	x The x location.
    @param	y The y location.
    @param	z The z location.
**/
KINC_FUNC void mat4_translation(mat4_t* self,float x,float y,float z);
/**
    Sets the matrix to one that represents no transform - located at the origin,
    zero rotation, and a uniform scale of 1.
    @param self The matrix to set
**/
KINC_FUNC void mat4_identity(mat4_t* self);
KINC_FUNC void mat4_scale(mat4_t* self,float x,float y,float z);
KINC_FUNC void mat4_rotationX(mat4_t* self,float alpha);
KINC_FUNC void mat4_rotationY(mat4_t* self,float alpha);
KINC_FUNC void mat4_rotationZ(mat4_t* self,float alpha);
KINC_FUNC void mat4_rotation(mat4_t* self,float yaw, float pitch, float roll);

/**
    Sets the matrix to a orthographic projection matrix.
    @param	left The left of the box.
    @param	right The right of the box.
    @param	bottom The bottom of the box.
    @param	top The top of the box.
    @param	near The depth of the near floor of the box.
    @param	far The depth of the far floor of the box.
**/
KINC_FUNC void mat4_ortho(mat4_t* self,float left,float right,float bottom,float top,float zn,float zf);

/**
    Sets the matrix to a perspective projection matrix.
    @param	fovY The vertical field of view.
    @param	aspect The aspect ratio.
    @param	zn The depth of the near floor of the frustum.
    @param	zf The depth of the far floor of the frustum.
**/
KINC_FUNC void mat4_persp(mat4_t* self,float fovY,float aspect,float zn,float zf);
KINC_FUNC void setLookAt(mat4_t* self,struct vec3_t* eye,struct vec3_t* at,struct vec3_t* up);
KINC_FUNC void mat4_add(mat4_t* self,mat4_t* other);
KINC_FUNC void mat4_sub(mat4_t* self,mat4_t* other);
KINC_FUNC void mat4_mult(mat4_t* self,mat4_t* other);
/**
 * Multiply the matrix by the vector. 
 * @param self The matrix
 * @param value The vector to multiply by.
 * @param product The product of the multiply
 **/
KINC_FUNC void mat4_multvec(mat4_t* self,struct vec4_t* value, struct vec4_t* product);
/**
    Multiply this vector by a scalar.
    * @param self The matrix
    * @param	value The scalar to multiply by.
**/
KINC_FUNC void mat4_multf(mat4_t* self,float value);
KINC_FUNC float mat4_cofactor(float m0, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8);
KINC_FUNC float mat4_determinant(mat4_t* self);
/**
    Invert a this matrix and store the result in another.
    @param self This matrix.
    @param	result The matrix to store the inverse.
**/
KINC_FUNC void mat4_inverse(mat4_t* self, mat4_t* result);
//Iron mat4

KINC_FUNC void mat4_multmats3x4(mat4_t* self,mat4_t* a,mat4_t* b);
/**
     Set the transform from a location, rotation and scale.
    @param self This matrix.
    @param loc The location to use.
    @param quat The rotation to use.
    @param sc The scale to use.
**/
KINC_FUNC void mat4_compose(mat4_t* self, struct vec4_t* loc, struct quat_t* quat,struct vec4_t* sc);
/**
     Decompose this matrix into its location, rotation and scale components.
    Additional transforms (skew, projection) will be ignored.
    @param self This matrix.
    @param loc A vector to write the location to.
    @param quat A quaternion to write the rotation to.
    @param scale A vector to write the scale to.
**/
KINC_FUNC void mat4_decompose(mat4_t* self, struct vec4_t* loc, struct quat_t* quat,struct vec4_t* scale);
/**
     Set the location component of this matrix.
    @param self This matrix
    @param	v The location to use.
**/
KINC_FUNC void mat4_setLoc(mat4_t* self, struct vec4_t* v);
/**
     Set the transform to a rotation from a quaternion. Other existing
    transforms will be removed.
    @param self This matrix
    @param q The rotation to use.
**/
KINC_FUNC void mat4_fromQuat( mat4_t* self, struct quat_t* q);
/**
    Apply an additional translation to this matrix.
    @param	x The distance to move in the x direction.
    @param	y The distance to move in the x direction.
    @param	z The distance to move in the x direction.
**/
 KINC_FUNC void mat4_translate(mat4_t* self, float x, float y, float z);
/**
    Apply an additional scale to this matrix.
    @param self This matrix.
    @param v The vector to scale by.
**/
KINC_FUNC void mat4_applyScale(mat4_t* self,struct vec4_t* v);
/**
    Transpose this matrix.
    @return	This matrix.
**/
KINC_FUNC void mat4_transpose(mat4_t* self);
KINC_FUNC void mat4_transpose3x3(mat4_t* self);
KINC_FUNC void mat4_setF32(mat4_t* self,float* a,int offset);
KINC_FUNC void mat4_setFrom(mat4_t* self, mat4_t* other);
/**
    Get the location component.
    @param self This matrix
    @param v The vector to put the location data
**/
KINC_FUNC void mat4_getLoc(mat4_t* self,struct vec4_t* v);
/**
    Get the scale component.
    @param self This matrix
    @param v The vector to put the scale data
**/
KINC_FUNC void mat4_getScale(mat4_t* self,struct vec4_t* v);
/**
    Convert this matrix to a rotation matrix, and discard location and
    scale information.
    @param self This matrix.
**/
KINC_FUNC void mat4_toRotation(mat4_t* self);
/**
    Apply an additional rotation to this matrix.
    @param self This matrix
    @param	q The quaternion to rotate by.
**/
KINC_FUNC void applyQuat(mat4_t* self,struct quat_t* q);
/**
 * Sets the vector to the positive x axis of the space defined by this matrix.
 * @param self This matrix
 * @param v The vector to be set to the right vector
**/
KINC_FUNC void  mat4_right(mat4_t* self,struct vec4_t* v);
/**
 * Sets the vector to the positive y axis of the space defined by this matrix.
 * @param self This matrix
 * @param v The vector to be set to the look vector
**/
KINC_FUNC void  mat4_look(mat4_t* self,struct vec4_t* v);
/**
 * Sets the vector to the positive z axis of the space defined by this matrix.
 * @param self This matrix
 * @param v The vector to be set to the up vector
**/
KINC_FUNC void mat4_up(mat4_t* self,struct vec4_t* v);
KINC_FUNC void mat4_to_kinc_matrix4x4(mat4_t* self,kinc_matrix4x4_t* out);

#ifdef __cplusplus
}
#endif