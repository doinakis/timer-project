#ifndef TIMER_H
#define TIMER_H

#include "queue.h"
#include "prod-cons.h"
typedef struct {
  int Period; // Time between 2 calls of the function
  int TasksToExecute; // How many times the function will be executed
  int StartDelay; // Delay on the first run of the function
  workFunction TimerFcn; // Function to be executed
  int *UserData;
  queue *q;
} timer;

void timerInit(timer *t, int Period,int TasksToExecute,int StartDelay, workFunction TimerFcn,queue *q);
void start(timer *t, pthread_t *pro);
void startat(timer *t,int y,int m,int d,int h,int min,int sec);
void StartFcn(timer *t);
void StopFcn(timer *t);
void ErrorFCN(timer *t); // If the queue is full this function is executed


#endif
