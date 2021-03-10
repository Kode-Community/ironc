#pragma once

#include "../pch.h"

#define realTime kinc_time

typedef bool task_func(void);

typedef struct time_task {
    task_func* task;

    double start;
    double period;
    double duration;
    double next;

    int id;
    int groupId;
    bool active;
    bool paused;
} time_task_t;

typedef struct frame_task {
    task_func* task;
    int priority;
    int id;
    bool active;
    bool paused;

} frame_task_t;

static double timeWarpSaveTime = 10.0;
#define DIF_COUNT 3
static float maxframetime = 0.5;

typedef struct scheduler {
    time_task_t* timeTasks;
    time_task_t* pausedTimeTasks;
    time_task_t* outdatedTimeTasks;
    time_task_t* timeTasksScratchPad;

    frame_task_t* frameTasks;
    frame_task_t* toDeleteFrame;

    double current;
    double lastTime;
    double lastFrameEnd;

    bool frame_tasks_sorted;
    bool stopped;
    bool vsync;

    double onedifhz;

    int currentFrameTaskId;
    int currentTimeTaskId;
    int currentGroupId;

    float deltas[DIF_COUNT];

    double startTime;
    time_task_t* activeTimeTask; 

} scheduler_t;

scheduler_t* singleton_scheduler;
KINC_FUNC void scheduler_init(scheduler_t* scheduler,time_task_t* timeTasks,time_task_t* pausedTimeTasks,time_task_t* outdatedTimeTasks,time_task_t* timeTasksScratchpad);
KINC_FUNC void scheduler_start(scheduler_t* s,bool restartTimes);
KINC_FUNC inline void scheduler_stop(scheduler_t* s){
    s->stopped = true;
}
KINC_FUNC inline bool scheduler_isStopped(scheduler_t* s){
    return s->stopped;
}
void warpTimeTasksBack(double time,time_task_t* tasks);
KINC_FUNC void warp(scheduler_t* s,double time);
KINC_FUNC void scheduler_executeFrame(scheduler_t* s);
void executeTimeTasks(scheduler_t* s,double until);
void archiveTimeTask(scheduler_t* s,time_task_t* time_task, double frameEnd);
/**
 * An approximation of the amount of time (in fractional seconds) that elapsed while the game was active.
 * This value is optimized for achieving smooth framerates.
 */
KINC_FUNC double scheduler_time(scheduler_t* s);
/**
 * The amount of time (in fractional seconds) that elapsed since the game started.
 */
KINC_FUNC double scheduler_realTime();

void scheduler_resetTime(scheduler_t* s);



KINC_FUNC void scheduler_addBreakableFrameTask(scheduler_t* s,frame_task_t* f,task_func* task,int priority);
KINC_FUNC void scheduler_addFrameTask(scheduler_t* s,frame_task_t* f,task_func* task,int priority);
KINC_FUNC void scheduler_pauseFrameTask(scheduler_t* s,int id,bool paused);
KINC_FUNC void scheduler_removeFrameTask(scheduler_t* s,int id);
KINC_FUNC int scheduler_generateGroupId(scheduler_t* s);
KINC_FUNC void scheduler_addBreakableTimeTaskToGroup(scheduler_t* s,time_task_t* t,int groupId,task_func* task,double start, double period, double duration);
KINC_FUNC void scheduler_addTimeTaskToGroup(scheduler_t* s,time_task_t* t,int groupId,task_func* task,double start, double period, double duration);
KINC_FUNC void scheduler_addBreakableTimeTask(scheduler_t* s,time_task_t* t,int groupId,task_func* task,double start, double period, double duration);
KINC_FUNC void scheduler_addTimeTask(scheduler_t* s,time_task_t* t,task_func* task,double start, double period, double duration);
time_task_t* getTimeTask(scheduler_t* s,int id);
KINC_FUNC void scheduler_pauseTimeTask(scheduler_t* s,int id,bool paused);
void pauseRunningTimeTask(scheduler_t* s,time_task_t* t,bool paused);
KINC_FUNC void scheduler_pauseTimeTasks(scheduler_t* s,int groupId,bool paused);
KINC_FUNC void scheduler_removeTimeTask(scheduler_t* s,int id);
KINC_FUNC void scheduler_removeTimeTasks(scheduler_t* s,int groupId);
KINC_FUNC int numTasksInSchedule(scheduler_t* s);
void insertSorted(time_task_t* list,time_task_t* task);
void sortFrameTasks(scheduler_t* s);
inline int8_t compare_frame_tasks(frame_task_t* a,frame_task_t* b){
    return a->priority > b->priority ? 1 : ((a->priority < b->priority) ? -1 : 0);
}
void switch_elements(frame_task_t* list,int from,int to);
