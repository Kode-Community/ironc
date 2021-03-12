#pragma once

//#include "trait.h"

struct trait_t;

typedef struct object {
    /*
    * Array of Traits.
    * Cast me maybe to trait_t*
    */
    struct trait_t* traits;
    //int value;
} object_t;