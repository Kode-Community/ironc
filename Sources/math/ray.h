#pragma once

#include "../pch.h"

#include "vec4.h"

typedef struct ray {
    vec4_t origin;
    vec4_t direction;
} ray_t;

typedef struct plane {
    vec4_t normal;
    float constant;
} plane_t;

// Ray funcs
KINC_FUNC void ray_set(ray_t* self,vec4_t* origin,vec4_t* direction);
KINC_FUNC void ray_at(ray_t* self,vec4_t* result,float t);
KINC_FUNC float ray_distanceToPoint(ray_t* self,vec4_t* point);
KINC_FUNC bool ray_intersectsSphere(ray_t* self,vec4_t* sphereCenter,float sphereRadius);
KINC_FUNC bool ray_intersectsPlane(ray_t* self,plane_t* plane);
KINC_FUNC float ray_distanceToPlane(ray_t* self,plane_t* plane);
KINC_FUNC void ray_intersectPlane(ray_t* self,plane_t* plane,vec4_t* result);
KINC_FUNC bool ray_intersectsBox(ray_t* self,vec4_t* center,vec4_t* dim);
KINC_FUNC void ray_intersectBox(ray_t* self,vec4_t* center,vec4_t* dim,vec4_t* result);
KINC_FUNC void ray_intersectTriangle(ray_t* self,vec4_t* a,vec4_t* b,vec4_t* c,bool backfaceCulling, vec4_t* result);
//Plane funcs
KINC_FUNC float plane_distanceToPoint(plane_t* self,vec4_t* point);
KINC_FUNC void plane_set(plane_t* self,vec4_t* normal,vec4_t* point);