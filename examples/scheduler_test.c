#include <kinc/pch.h>
#include "system/scheduler.h"
#include "system/time.h"

#define MAX_TASKS 256

void update(){
	time_update();
	kinc_log(KINC_LOG_LEVEL_INFO, "%f",realDelta);
}
scheduler_t* s;
int kickstart(int argc, char** argv) {
	kinc_init("Shader", 1024, 768, NULL, NULL);
	kinc_set_update_callback(update);
	s = (scheduler_t*)malloc(sizeof(scheduler_t));
	time_task_t* timeTasks = NULL;
	time_task_t* pausedTimeTasks = NULL;
	time_task_t* outdatedTimeTasks = NULL;
	time_task_t* timeTasksScratchpad = NULL;
	arrsetcap(timeTasks,MAX_TASKS);
	arrsetcap(pausedTimeTasks,MAX_TASKS);
	arrsetcap(outdatedTimeTasks,MAX_TASKS);
	arrsetcap(timeTasksScratchpad,MAX_TASKS);
	scheduler_init(s,timeTasks,pausedTimeTasks,outdatedTimeTasks,timeTasksScratchpad);
	scheduler_start(s,false);
	kinc_start();
	return 0;
}
