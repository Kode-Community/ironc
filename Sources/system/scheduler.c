
#include "scheduler.h"

#include "kinc/math/core.h"
#include "kinc/system.h"
#include "kinc/window.h"
#include "kinc/display.h"

void scheduler_init(scheduler_t* scheduler,time_task_t* timeTasks,time_task_t* pausedTimeTasks,time_task_t* outdatedTimeTasks,time_task_t* timeTasksScratchpad,frame_task_t* frameTasks,frame_task_t* toDeleteFrame){
    singleton_scheduler = scheduler;
    for(int i = 0; i < DIF_COUNT; i++){
        scheduler->deltas[i] = 0;
    }
    scheduler->stopped = true;
    scheduler->frame_tasks_sorted = true;
    scheduler->current = scheduler->lastTime = scheduler->lastFrameEnd = realTime();

    scheduler->currentFrameTaskId = 0;
    scheduler->currentTimeTaskId = 0;
    scheduler->currentGroupId = 0;

    scheduler->timeTasks = timeTasks;
    scheduler->pausedTimeTasks = pausedTimeTasks;
    scheduler->outdatedTimeTasks = outdatedTimeTasks;
    scheduler->timeTasksScratchPad = timeTasksScratchpad;
    scheduler->frameTasks = frameTasks;
    scheduler->toDeleteFrame = toDeleteFrame;

}

void scheduler_start(scheduler_t* s,bool restartTimes){
    s->vsync = kinc_window_vsynced(0);
    kinc_display_mode_t display = kinc_display_current_mode(kinc_primary_display());
    double hz =  display.frequency;
    if (hz >= 57 && hz <= 63)
			hz = 60;
	s->onedifhz = 1.0 / hz;

    s->stopped = false;

    scheduler_resetTime(s);
    s->lastTime = realTime() - s->startTime;
    for(int i = 0; i < DIF_COUNT; i++){
        s->deltas[i] = 0;
    }

    if(restartTimes){
        for( int i = 0; i < arrlen(s->timeTasks); i++){
            s->timeTasks[i].paused = false;
        }
        for( int i = 0; i < arrlen(s->frameTasks); i++){
            s->frameTasks[i].paused = false;
        }
    }
}

void warpTimeTasksBack(double time,time_task_t* tasks){
    for( int i = 0; i < arrlen(tasks); i++){
        time_task_t timeTask = tasks[i];
        if (timeTask.start >= time) {
            timeTask.next = timeTask.start;
        }
        else if (timeTask.period > 0) {
            double sinceStart = time - timeTask.start;
            float times = kinc_ceil(sinceStart / timeTask.period);
            timeTask.next = timeTask.start + times * timeTask.period;
        }
    }
}

void warp(scheduler_t* s,double time){
    if (time < s->lastTime) {
        s->current = time;
        s->lastTime = time;
        s->lastFrameEnd = time;

        warpTimeTasksBack(time, s->outdatedTimeTasks);
        warpTimeTasksBack(time, s->timeTasks);

        for( int i = 0; i < arrlen(s->outdatedTimeTasks); i++) {
            time_task_t task = s->outdatedTimeTasks[i];
            if (task.next >= time) {
                arrpush(s->timeTasksScratchPad,s->outdatedTimeTasks[i]);
                arrdelswap(s->outdatedTimeTasks,i);
                arrpop(s->outdatedTimeTasks);
                i--;
            }
        }
        for( int i = 0; i < arrlen(s->timeTasksScratchPad); i++){
            insertSorted(s->timeTasks, &s->timeTasksScratchPad[i]);
        }
        arrdeln(s->timeTasksScratchPad,0,arrlen(s->timeTasksScratchPad));
    }
    else if (time > s->lastTime) {
        // TODO: Changing startTime line prevents clients from falling into a
        // warp-forward-then-wait-for-systemtime-to-catch-up-loop that causes
        // choppy movement (e.g. every 3rd frame forward 3 times).
        // But it causes backwards jumps in originally constant movements.
        // And on HTML5 packets are received while no frames are executed,
        // which causes the client to overtakes the server and then move
        // farther away with each packet while being unable to synch back
        // (backwards warping is not allowed to change startTime).
        s->startTime -= (time - s->lastTime);

        s->current = time;
        s->lastTime = time;
        s->lastFrameEnd = time;

        executeTimeTasks(s,time);
    }
}

void scheduler_executeFrame(scheduler_t* s){
    double real = realTime();
    double now = real - s->startTime;
    double delta = now - s->lastTime;

    double frameEnd = s->lastFrameEnd;
    if(delta >= 0){
        if(true)//would be kha.netsync.Session.the() == null but we dont have netcode in Kinc
        {
            if (delta > maxframetime) {
                s->startTime += delta - maxframetime;
                now = real - s->startTime;
                delta = maxframetime;
                frameEnd += delta;
            }
            else {
                if (s->vsync) {
                    // var measured = delta;
                    // this is optimized not to run at exact speed
                    // but to run as fluid as possible
                    double frames = kinc_round(delta / s->onedifhz);
                    if (frames < 1) {
                        return;
                    }

                    double realdif = frames * s->onedifhz;

                    delta = realdif;
                    for (int i = 0; i < DIF_COUNT - 2;i++) {
                        delta += s->deltas[i];
                        s->deltas[i] = s->deltas[i + 1];
                    }
                    delta += s->deltas[DIF_COUNT - 2];
                    delta /= DIF_COUNT;
                    s->deltas[DIF_COUNT - 2] = realdif;

                    frameEnd += delta;

                    // trace("Measured: " + measured + " Frames: " + frames +  " Delta: " + delta + " ");
                }
                else {
                    for (int i = 0; i < DIF_COUNT - 1;i++) {
                        s->deltas[i] = s->deltas[i + 1];
                    }
                    s->deltas[DIF_COUNT - 1] = delta;

                    double next = 0;
                    for (int i = 0; i < DIF_COUNT;i++) {
                        next += s->deltas[i];
                    }
                    next /= DIF_COUNT;

                    // delta = interpolated_delta; // average the frame end estimation

                    // lastTime = now;
                    frameEnd += next;
                }
            }
        }
        else {
            frameEnd += delta;
        }
        s->lastTime = now;
        if (!s->stopped) { // Stop simulation time
            s->lastFrameEnd = frameEnd;
        }

        // Extend endpoint by paused time (individually paused tasks)
        for (int i = 0; i < arrlen(s->pausedTimeTasks); i++) {
            s->pausedTimeTasks[i].next += delta;
        }

        if (s->stopped) {
            // Extend endpoint by paused time (running tasks)
            for (int i = 0; i < arrlen(s->timeTasks); i++) {
                s->timeTasks[i].next += delta;
            }
        }

        executeTimeTasks(s,frameEnd);

        // Maintain outdated task list
        for (int i = 0; i < arrlen(s->outdatedTimeTasks); i++) {
            time_task_t task = s->outdatedTimeTasks[i];
            if (task.next < frameEnd - timeWarpSaveTime) {
                // arrpush(s->timeTasksScratchPad,task);
                arrdelswap(s->outdatedTimeTasks,i);
                arrpop(s->outdatedTimeTasks);
                i--;
            }
        }
        // while (arrlen(s->timeTasksScratchPad) > 0) {
        //     arrdelswap(s->timeTasksScratchPad,0);
        //     arrpop(s->timeTasksScratchPad);
        // }
    }
    s->current = frameEnd;

    sortFrameTasks(s);
    for (int i = 0; i < arrlen(s->frameTasks); i++) {
        frame_task_t frameTask = s->frameTasks[i];
        if (!s->stopped && !frameTask.paused && frameTask.active) {
            if (!frameTask.task())
                frameTask.active = false;
        }
    }

    for (int i = 0; i < arrlen(s->frameTasks); i++) {
        if (!s->frameTasks[i].active) {
            arrdelswap(s->frameTasks,i);
            arrpop(s->frameTasks);
            i--;
        }
    }
}

void executeTimeTasks(scheduler_t* s,double until){
    while(arrlen(s->timeTasks) > 0){
        s->activeTimeTask = &s->timeTasks[0];
        
        if(s->activeTimeTask->next <= until){
            s->current = s->activeTimeTask->next;
            s->activeTimeTask->next = s->activeTimeTask->period;
            arrdelswap(s->timeTasks,0);
            arrpop(s->timeTasks);
            if(s->activeTimeTask->active && s->activeTimeTask->task()){
                if(s->activeTimeTask->period > 0 && 
                (s->activeTimeTask->duration == 0 || s->activeTimeTask->duration >= s->activeTimeTask->start + s->activeTimeTask->next)){
                    insertSorted(s->timeTasks,s->activeTimeTask);
                }
                else {
                    archiveTimeTask(s,s->activeTimeTask, until);
                }
            }
            else {
                s->activeTimeTask->active = false;
				archiveTimeTask(s,s->activeTimeTask, until);
            }
        }
        else {
            break;
        }
    }
    s->activeTimeTask = NULL;
}
void archiveTimeTask(scheduler_t* s,time_task_t* time_task, double frameEnd){
    #ifdef KINC_SYS_SERVER
    if(time_task->next > frameEnd - timeWarpSaveTime){
        arrpush(s->outdatedTimeTasks,*time_task);
    }
    #endif
}

/**
 * An approximation of the amount of time (in fractional seconds) that elapsed while the game was active.
 * This value is optimized for achieving smooth framerates.
 */
double scheduler_time(scheduler_t* s){
    return s->current;
}

double scheduler_realTime(){
    return kinc_time();
}

void scheduler_resetTime(scheduler_t* s){
    double now = kinc_time();
    double dif = now - s->startTime;
    s->startTime = now;

    for( int i = 0; i < arrlen(s->timeTasks); i++){
        s->timeTasks[i].start -= dif;
        s->timeTasks[i].next -= dif;
    }
    for(int i = 0; i < DIF_COUNT; i++){
        s->deltas[i] = 0;
    }
    s->current = 0;
    s->lastTime = 0;
    s->lastFrameEnd = 0;
}

void scheduler_addBreakableFrameTask(scheduler_t* s,frame_task_t* f,task_func* task,int priority){
    
    f->task = task;
    f->priority = priority;
    f->id = ++s->currentFrameTaskId;
    f->active = true;
    f->paused = false;

    arrpush(s->frameTasks,*f);
    s->frame_tasks_sorted = false;
}

//@FIXME:
void scheduler_addFrameTask(scheduler_t* s,frame_task_t* f,task_func* task,int priority){
    scheduler_addBreakableFrameTask(s,f,task,priority);//Should be what is under here
    // return addBreakableFrameTask(function() {
    //     task();
    //     return true;
    // }, priority);
}

void scheduler_pauseFrameTask(scheduler_t* s,int id,bool paused){
    for( int i = 0; i < arrlen(s->frameTasks); i++){
        if(s->frameTasks[i].id == id){
            s->frameTasks[i].paused = paused;
            break;
        }
    }
}

void scheduler_removeFrameTask(scheduler_t* s,int id){
    for( int i = 0; i < arrlen(s->frameTasks); i++){
        if(s->frameTasks[i].id == id){
            s->frameTasks[i].active = false;
            break;
        }
    }
}

int scheduler_generateGroupId(scheduler_t* s){
    return ++s->currentGroupId;
}

void scheduler_addBreakableTimeTaskToGroup(scheduler_t* s,time_task_t* t,int groupId,task_func* task,double start, double period, double duration){
    
    t->active = true;
    t->task = task;
    t->id = ++s->currentTimeTaskId;
    t->groupId = groupId;

    t->start = s->current + start;
    t->period = 0;
    if (period != 0)
        t->period = period;
    t->duration = 0; // infinite
    if (duration != 0)
        t->duration = t->start + duration;

    t->next = t->start;
    insertSorted(s->timeTasks,t);
}

//@FIXME: 
void scheduler_addTimeTaskToGroup(scheduler_t* s,time_task_t* t,int groupId,task_func* task,double start, double period, double duration){
    scheduler_addBreakableTimeTaskToGroup(s,t,groupId, task, start, period, duration);// Should be what is under here
    // return addBreakableTimeTaskToGroup(groupId, function() {
    //     task();
    //     return true;
    // }, start, period, duration);
}

void scheduler_addBreakableTimeTask(scheduler_t* s,time_task_t* t,int groupId,task_func* task,double start, double period, double duration){
    scheduler_addBreakableTimeTaskToGroup(s,t,0, task, start, period, duration);
}

void scheduler_addTimeTask(scheduler_t* s,time_task_t* t,task_func* task,double start, double period, double duration){
    scheduler_addTimeTaskToGroup(s,t,0, task, start, period, duration);
}

time_task_t* getTimeTask(scheduler_t* s,int id){
    if (s->activeTimeTask != NULL && s->activeTimeTask->id == id)
        return s->activeTimeTask;
    for (int i = 0; i < arrlen(s->timeTasks); i++) {
        if (s->timeTasks[i].id == id) {
            return &s->timeTasks[i];
        }
    }
    for (int i = 0; i < arrlen(s->pausedTimeTasks); i++) {
        if (s->pausedTimeTasks[i].id == id) {
            return &s->pausedTimeTasks[i];
        }
    }
    return NULL;
}

void scheduler_pauseTimeTask(scheduler_t* s,int id,bool paused){
    time_task_t* timeTask = getTimeTask(s,id);
    if(timeTask != NULL){
        pauseRunningTimeTask(s,timeTask, paused);
    }
}

void pauseRunningTimeTask(scheduler_t* s,time_task_t* t,bool paused){
    t->paused = paused;
    if (paused) {
        for (int i = 0; i < arrlen(s->timeTasks); i++) {
            if (s->timeTasks[i].id == t->id) {
                time_task_t temp = s->timeTasks[i]; 
                arrdel(s->timeTasks,i);
                arrpush(s->pausedTimeTasks,temp);
            }
        }   
    }
    else {
        insertSorted(s->timeTasks, t);
        for (int i = 0; i < arrlen(s->pausedTimeTasks); i++) {
            if (s->pausedTimeTasks[i].id == t->id) { 
                arrdelswap(s->pausedTimeTasks,i);
            }
        }
    }
}

void scheduler_pauseTimeTasks(scheduler_t* s,int groupId,bool paused){
    for (int i = 0; i < arrlen(s->timeTasks); i++) {
        if (s->timeTasks[i].groupId == groupId) {
            pauseRunningTimeTask(s,&s->timeTasks[i],paused); 
            
        }
    }
    if(s->activeTimeTask != NULL && s->activeTimeTask->groupId == groupId){
        s->activeTimeTask->paused = paused;
    }
}

void scheduler_removeTimeTask(scheduler_t* s,int id)
{
    for (int i = 0; i < arrlen(s->timeTasks); i++) {
        if (s->timeTasks[i].id == id) {
            s->timeTasks[i].active = false;
            arrdel(s->timeTasks,i); 
            
        }
    }
}

void scheduler_removeTimeTasks(scheduler_t* s,int groupId){
    for (int i = 0; i < arrlen(s->timeTasks); i++) {
        if (s->timeTasks[i].groupId == groupId) {
            s->timeTasks[i].active = false;
            arrdel(s->timeTasks,i);
            i--;
        }
    }
    if(s->activeTimeTask != NULL && s->activeTimeTask->groupId == groupId){
        s->activeTimeTask->active = false;
    }
}

int numTasksInSchedule(scheduler_t* s){
    return arrlen(s->timeTasks) + arrlen(s->frameTasks);
}
void insertSorted(time_task_t* list,time_task_t* task){
    for( int i = 0; i < arrlen(list); i++) {
        if (list[i].next > task->next) {
            arrins(list,i,*task);
            return;
        }
    }
    arrpush(list,*task);
}

void sortFrameTasks(scheduler_t* s){
    if(s->frame_tasks_sorted)
        return;
    
    //sort
    for (int i = 0; i < arrlen(s->frameTasks); i++){
        int equals = 0;
        for (int y = 1; i + y < arrlen(s->frameTasks); y++){
            int8_t res = compare_frame_tasks(&s->frameTasks[i],&s->frameTasks[i+y]);
            if(res > 0){
                continue;
            }
            else if(res < 0){
                switch_elements(s->frameTasks,i,i+y);
            }
            else if(y > 1 && res  == 0){
                equals++;
                switch_elements(s->frameTasks,i+equals,i+y);
            }
        }
    }
    s->frame_tasks_sorted = true;
}

void switch_elements(frame_task_t* list,int from,int to){
    arrpush(list,list[to]);
    frame_task_t temp = list[from];
    arrdelswap(list,from);
    arrpop(list);

    arrpush(list,temp);
    arrdelswap(list,to);
    arrpop(list,from);
}
