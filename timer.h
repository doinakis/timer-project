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
  queue : The fifo where all the functions await their execution
*/
typedef struct {
  int Period;
  int TasksToExecute;
  int StartDelay;
  workFunction TimerFcn;
  void *UserData;
  queue *q;
} timer;

void timerInit(timer *t, int Period,int TasksToExecute,int StartDelay, workFunction TimerFcn,void *UserData,queue *q);
void start(timer *t, pthread_t *pro);
void startat(timer *t,int y,int m,int d,int h,int min,int sec,pthread_t *pro);
void StartFcn(timer *t);
void StopFcn(timer *t);
void ErrorFcn(timer *t);


#endif
