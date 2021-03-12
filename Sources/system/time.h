#pragma once

#include "../pch.h"

extern int frequency;
extern double time_last;
void init_frequency(void);
double get_step(void);
extern float scale;
double get_delta();

KINC_FUNC extern double realDelta;

KINC_FUNC double time_time(void);

KINC_FUNC inline double time_realTime(void){
    return scheduler_realTime();
}

KINC_FUNC void time_update(void);