#pragma once

#include "../pch.h"
#include "scheduler.h"
#include "kinc/display.h"

extern int frequency;
extern double time_last;
void init_frequency(void);
double get_step(void);
extern float scale;
double get_delta();

KINC_FUNC extern double realDelta;

KINC_FUNC inline double time_time(void){
    #ifdef DBG
    if(singleton_scheduler == NULL){
        kinc_log(KINC_LOG_LEVEL_ERROR,"The scheduler isnt initialised. Call scheduler_init before using time.");
    }
    #endif
    scheduler_time(singleton_scheduler);
}

KINC_FUNC inline double time_realTime(void){
    return scheduler_realTime();
}

KINC_FUNC void time_update(void);