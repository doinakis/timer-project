#ifndef TIMER_H
#define TIMER_H

#include "queue.h"

typedef struct {
  int Period;
  int TasksToExecute;
  int StartDelay;
  workFunction TimerFcn;
  int *UserData;
} timer;

void start(timer t);
void startat(timer t,int y,int m,int d,int h,int min,int sec);
void StartFcn(void);
void ErrorFCN(void);


#endif
