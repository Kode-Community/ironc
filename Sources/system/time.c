#include "time.h"
#include "kinc/display.h"

void init_frequency(){
    kinc_display_mode_t m = kinc_display_current_mode(kinc_primary_display());
    frequency = m.frequency;
}

double get_step(){
    if(frequency < 0){
        init_frequency();
    }
    return 1/frequency;
}