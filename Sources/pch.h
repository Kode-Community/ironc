
#pragma once

#include "kinc/pch.h"
#include "stb_ds.h"
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
