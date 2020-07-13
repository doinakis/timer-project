#ifndef TIMER_H
#define TIMER_H

#include "queue.h"
#include "prod-cons.h"
#include <time.h>

/*
    ----------Timer Stracture----------
  Period: Time between 2 calls of the function (in useconds)
  TasksToExecute: How many times the function will be executed
  StartDelay: Delay on the first run of the function (in seconds)
  TimerFcn: Function to be executed
  UserData: User defined data
  queue: The fifo where all the functions await their execution
  pro: The producer assigned with the timer
*/
typedef struct {
  int Period;
  int TasksToExecute;
  int StartDelay;
  workFunction *TimerFcn;
  void *UserData;
  queue *q;
  pthread_t *pro;
} timer;

/*
  timerInit: Initializes the core variables of the timer and allocates the space
    for the mutexes of the TimerFcn and its condition variables the values are
    given in main by the user. StartFcn executes at the start. StartFcn may change
    by the user to initialize UserData for the TimerFcn
*/
void timerInit(timer *t, int Period,int TasksToExecute,int StartDelay, workFunction *TimerFcn,void *UserData,queue *q);

/*
  start: Creates a producer thread that runs the timer
*/
void start(timer *t);

/*
  startat: Creates a producer thread that runs the timer at the given date and
    time. If the date and time given is in the past then the producer starts
    without further waiting.
*/
void startat(timer *t,int y,int m,int d,int h,int min,int sec);

/*
  StartFcn: Executes when the timer is being constructed to initialize UserData.
*/
void StartFcn(timer *t);

/*
  StopFcn: Executes after the last instance of the function of the timer completes
    its execution.
*/
void StopFcn(timer *t);

/*
  ErrorFcn: Buffer overflow handler
*/
void ErrorFcn(void);

/*
  TimerStop: It deallocates the space that was allocated for the execution of
  the timer.
*/
void TimerStop(timer *t);


#endif
