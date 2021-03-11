#pragma once

#include "pch.h"
#include "object.h"
// #include <kinc/graphics4/graphics.h>

FUNC_DEF(void, VoidFunc, void);
typedef struct trait {
    char* name;
    /**
	  Object this trait belongs to.
	**/
    object_t* object;

    VoidFunc** _add;
    VoidFunc** _init;
    VoidFunc** _remove;
    VoidFunc** _update;
    VoidFunc** _lateUpdate;
    ////@TODO: Revisite these when we have rendering figured out
    //VoidFunc** _render;
    //VoidFunc** _render2D;
    
} trait_t;

/**
 * Initializes the trait to the object
 * @param t The trait to initialize
 * @param o The object we will set on the Trait
 * @param name name of the Trait
**/
KINC_FUNC void init_trait(trait_t* t,object_t* o,const char* name);
/**
 * Add a function to be called when we add the Trait to an Object.
 * This can happen at anytime.
**/
KINC_FUNC void notifyOnAdd(trait_t* t, void (*value)(void));
/**
 * Add a function to be called when the scene is initialized. 
**/
KINC_FUNC void notifyOnInit(trait_t* t, void (*value)(void));
/**
 * Add a function to be called when we remove the Trait from an Object.
 * This can happen at anytime.
**/
KINC_FUNC void notifyOnRemove(trait_t* t, void (*value)(void));
KINC_FUNC void notifyOnUpdate(trait_t* t, void (*value)(void));
KINC_FUNC void removeOnUpdate(trait_t* t, void (*value)(void));
KINC_FUNC void notifyOnLateUpdate(trait_t* t, void (*value)(void));
KINC_FUNC void removeOnLateUpdate(trait_t* t, void (*value)(void));
//void notifyOnRender(trait_t* t, void (*value)(void));
//void removeOnRender(trait_t* t, void (*value)(void));
//void notifyOnRender2D(trait_t* t, void (*value)(void));
//void removeOnRender2D(trait_t* t, void (*value)(void));