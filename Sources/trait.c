#include "trait.h"
#include <string.h>

#include "kinc/log.h" 

trait_t* create_trait(const char* name){
    trait_t* t = malloc(sizeof(trait_t));
    #ifdef DBG
    if(t == NULL){
        kinc_log(KINC_LOG_LEVEL_ERROR,"Couldn't create the Trait.");
    }
    #endif
    t->name = malloc(sizeof(char) * strlen(name));
    memcpy(t->name,name,strlen(name));
    //t->object = NULL;
    t->_add = NULL;
    t->_init = NULL;
    t->_remove = NULL;
    t->_update = NULL;
    t->_lateUpdate = NULL;
    //t->_render= NULL;
    //t->_render2D = NULL;
    return t;
}
void notifyOnAdd(trait_t* t, void (*value)(void)){
    arrpush(t->_add,value);
}
void notifyOnInit(trait_t* t, void (*value)(void)){
    arrpush(t->_init, value);
}
void notifyOnRemove(trait_t* t, void (*value)(void)){
    arrpush(t->_remove, value);
}
void notifyOnUpdate(trait_t* t, void (*value)(void)){
    arrpush(t->_update, value);
}
void removeOnUpdate(trait_t* t, void (*value)(void)){
    for (int i = 0; i < arrlen(t->_update); ++i) {
        if (t->_update[i] == value) {
            arrdel(t->_update, i);
        }
    }
}
void notifyOnLateUpdate(trait_t* t, void (*value)(void)){
    arrpush(t->_lateUpdate, value);
}
void removeOnLateUpdate(trait_t* t, void (*value)(void)){
    for (int i = 0; i < arrlen(t->_lateUpdate); ++i) {
        if (t->_lateUpdate[i] == value) {
            arrdel(t->_lateUpdate, i);
        }
    }
}
//void notifyOnRender(trait_t* t, void (*value)(void)){
//
//}
//void removeOnRender(trait_t* t, void (*value)(void)){
//
//}
//void notifyOnRender2D(trait_t* t, void (*value)(void)){
//
//}
//void removeOnRender2D(trait_t* t, void (*value)(void)){
//
//}