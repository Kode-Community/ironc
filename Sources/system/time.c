#include "time.h"

#include "scheduler.h"
#include "kinc/display.h"

int frequency = -1;
double time_last = 0.0;
float scale = 1.0;
double realDelta = 0.0;

void init_frequency(void){
    kinc_display_mode_t m = kinc_display_current_mode(kinc_primary_display());
    frequency = m.frequency;
}

double get_step(void){
    if(frequency < 0){
        init_frequency();
    }
    return 1/frequency;
}

double time_time(void){
    #ifdef DBG
    if(singleton_scheduler == NULL){
        kinc_log(KINC_LOG_LEVEL_ERROR,"The scheduler isn't initialised. Call scheduler_init before using time.");
    }
    #endif
    scheduler_time(singleton_scheduler);
}

double time_realTime(void){
    return scheduler_realTime();
}

void time_update(void){
    realDelta = time_realTime() - time_last;
    time_last = time_realTime();
}