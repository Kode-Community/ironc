
#pragma once

#include "kinc/pch.h"
#include "stb_ds.h"
#define DBG
#if defined(ARR_FUNC) || defined(ARR_VAR)
#undef ARR_FUNC
#undef ARR_VAR
#endif
/**
 * Dynamic array of functions to be used with stb_ds 
**/
#define FUNC_DEF(type,name, param) typedef type name(param)
/**
 * Dynamic array variable to be used with stb_ds 
**/
#define ARR_VAR(varname) (*(varname))

#ifdef DBG
#include "kinc/log.h"
inline void check_alloc(void* data, const char *file, int line){
    if(data == NULL){
        kinc_log(KINC_LOG_LEVEL_ERROR,"%s (%4d) Failed to allocate data, not enough space available.",file,line);
    }
}
#define CHECK_ALLOC(vptr) check_alloc(vptr,__FILE__, __LINE__);
#endif
