#include "ray.h"

void ray_set(ray_t* self,vec4_t* origin,vec4_t* direction){
    self->origin.x = origin->x;
    self->origin.y = origin->y;
    self->origin.z = origin->z;
    self->origin.w = origin->w;

    self->direction.x = direction->x;
    self->direction.y = direction->y;
    self->direction.z = direction->z;
    self->direction.w = direction->w;
}

void ray_at(ray_t* self,vec4_t* result,float t){
    memcpy(result,&self->direction,sizeof(vec4_t));
    vec4_mult(result,t);
    vec4_add(result,&self->origin);
}

float ray_distanceToPoint(ray_t* self,vec4_t* point){
    vec4_t v1 = {0};
    memcpy(&v1,point,sizeof(vec4_t));
    vec4_sub(&v1,&self->origin);

    float directionDistance = vec4_dot(&v1,&self->direction);

    // Point behind the ray
    if (directionDistance < 0) {
        return vec4_distanceTo(&self->origin,point);
    }
    
    memcpy(&v1,&self->direction,sizeof(vec4_t));

    vec4_mult(&v1,directionDistance);
    vec4_add(&v1,&self->origin);
    return vec4_distanceTo(&v1,point);
}

bool ray_intersectsSphere(ray_t* self,vec4_t* sphereCenter,float sphereRadius){
    return ray_distanceToPoint(self,sphereCenter) <= sphereRadius;
}

bool ray_intersectsPlane(ray_t* self,plane_t* plane){
    // Check if the ray lies on the plane first
    float distToPoint = plane_distanceToPoint(plane,&self->origin);
    if (distToPoint == 0) return true;

    float denominator = vec4_dot(&plane->normal,&self->direction);
    if (denominator * distToPoint < 0) return true;

    // Ray origin is behind the plane (and is pointing behind it)
	return false;
}

float ray_distanceToPlane(ray_t* self,plane_t* plane){
    float denominator = vec4_dot(&plane->normal,&self->direction);
    if(denominator == 0.0f){
        // Line is coplanar, return origin
        if(plane_distanceToPoint(plane,&self->origin) == 0.0f){
            return 0.0f;
        }
        return -1.0f;
    }

    float t = -(vec4_dot(&self->origin,&plane->normal) + plane->constant) / denominator;

    // Return if the ray never intersects the plane
	return t >= 0 ? t : -1.0f;
}

void ray_intersectPlane(ray_t* self,plane_t* plane,vec4_t* result){
    float t = ray_distanceToPlane(self,plane);
    if(t == -1.0f) return;
    ray_at(self,result,t);
}

bool ray_intersectsBox(ray_t* self,vec4_t* center,vec4_t* dim){
    vec4_t result = {0};
    ray_intersectBox(self,center,dim,&result);
    return result.w > 0;
}

/**
    Checks if this ray intersects the box and returns the position.
    If it doesn't intersect it sets the result.w component to -1. If it does we set the value to 1. 
    @param self This ray.
    @param center Center vector of the box.
    @param dim Dimension vector of the box.
    @param result The vector that will hold the resulting position.
**/
void ray_intersectBox(ray_t* self,vec4_t* center,vec4_t* dim,vec4_t* result){
    result->w = -1.0f;
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    float halfX = dim->x / 2;
    float halfY = dim->y / 2;
    float halfZ = dim->z / 2;
    float boxMinX = center->x - halfX;
    float boxMinY = center->y - halfY;
    float boxMinZ = center->z - halfZ;
    float boxMaxX = center->x + halfX;
    float boxMaxY = center->y + halfY;
    float boxMaxZ = center->z + halfZ;

    float invdirx = 1 / self->direction.x;
    float	invdiry = 1 / self->direction.y;
    float invdirz = 1 / self->direction.z;

    vec4_t origin = self->origin;

    if (invdirx >= 0) {
        tmin = (boxMinX - origin.x) * invdirx;
        tmax = (boxMaxX - origin.x) * invdirx;
    }
    else {
        tmin = (boxMaxX - origin.x) * invdirx;
        tmax = (boxMinX - origin.x) * invdirx;
    }

    if (invdiry >= 0) {
        tymin = (boxMinY - origin.y) * invdiry;
        tymax = (boxMaxY - origin.y) * invdiry;
    }
    else {
        tymin = (boxMaxY - origin.y) * invdiry;
        tymax = (boxMinY - origin.y) * invdiry;
    }

    if ((tmin > tymax) || (tymin > tmax)) return;

    // These lines also handle the case where tmin or tmax is NaN
    // (result of 0 * Infinity). x !== x returns true if x is NaN
    if (tymin > tmin || tmin != tmin) tmin = tymin;
    if (tymax < tmax || tmax != tmax) tmax = tymax;

    if (invdirz >= 0) {
        tzmin = (boxMinZ - origin.z) * invdirz;
        tzmax = (boxMaxZ - origin.z) * invdirz;
    }
    else {
        tzmin = (boxMaxZ - origin.z) * invdirz;
        tzmax = (boxMinZ - origin.z) * invdirz;
    }

    if ((tmin > tzmax) || (tzmin > tmax)) return;
    if (tzmin > tmin || tmin != tmin ) tmin = tzmin;
    if (tzmax < tmax || tmax != tmax ) tmax = tzmax;

    // Return point closest to the ray (positive side)
    if (tmax < 0) return;

    result->w = 1.0f;
    ray_at(self,result,tmin >= 0 ? tmin : tmax);
}

/**
    Checks if this ray intersects the triangle and returns the position.
    If it doesn't intersect it sets the result.w component to -1. If it does we set the value to 1. 
    @param self This ray.
    @param a First point of triangle.
    @param b Second point of triangle.
    @param c Third point of triangle.
    @param backfaceCulling If back face culling is enabled
    @param result The vector that will hold the resulting position.
**/
void ray_intersectTriangle(ray_t* self,vec4_t* a,vec4_t* b,vec4_t* c,bool backfaceCulling, vec4_t* result){
    result->w = -1.0f;
    // Compute the offset origin, edges, and normal
    vec4_t diff = self->origin;
    vec4_t edge1 = {0};
    vec4_t edge2 = {0};
    vec4_t normal = {0};

    // from http://www.geometrictools.com/LibMathematics/Intersection/Wm5IntrRay3Triangle3.cpp
    memcpy(&edge1,b,sizeof(vec4_t));
    vec4_sub(&edge1, a);
    memcpy(&edge2,c,sizeof(vec4_t));
    vec4_sub(&edge2, a);
    memcpy(&normal,&edge1,sizeof(vec4_t));
    vec4_cross(&normal,&edge2);

    // Solve Q + t*D = b1*E1 + b2*E2 (Q = kDiff, D = ray direction,
    // E1 = kEdge1, E2 = kEdge2, N = Cross(E1,E2)) by
    //   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
    //   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
    //   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
    float DdN = vec4_dot(&self->direction,&normal);
    int sign;

    if (DdN > 0) {
        if (backfaceCulling) return;
        sign = 1;
    } else if (DdN < 0) {
        sign = -1;
        DdN = -DdN;
    } else {
        return;
    }

    vec4_sub(&diff, a);
    vec4_t diff2 = diff;
    vec4_cross(&diff,&edge2);
    float DdQxE2 = sign * vec4_dot(&self->direction,&diff);

    // b1 < 0, no intersection
    if (DdQxE2 < 0) {
        return;
    }

    vec4_cross(&edge1,&diff2);
    float DdE1xQ = sign * vec4_dot(&self->direction,&edge1);

    // b2 < 0, no intersection
    if (DdE1xQ < 0) {
        return;
    }

    // b1+b2 > 1, no intersection
    if (DdQxE2 + DdE1xQ > DdN) {
        return;
    }

    // Line intersects triangle, check if ray does.
    float QdN = -sign * vec4_dot(&diff2,&normal);

    // t < 0, no intersection
    if (QdN < 0) {
        return;
    }

    result->w = 1.0f;
    // Ray intersects triangle.
    return ray_at(self,result,QdN / DdN);
}

float plane_distanceToPoint(plane_t* self,vec4_t* point){
    return vec4_dot(&self->normal,point) + self->constant;
}

void plane_set(plane_t* self,vec4_t* normal,vec4_t* point){
    vec4_set(&self->normal,normal->x,normal->y,normal->z,normal->w);
    self->constant = -vec4_dot(point,normal);
}
