#pragma once

#include "../math/vec4.h"
#include "../math/quat.h"
#include "../math/mat4.h"

typedef struct transform {
    mat4_t world;
    bool localOnly;
    mat4_t local;
} transform_t;
//TODO: Fill me up