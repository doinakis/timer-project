#include "timer.h"

void timerInit(timer *t, int Period,int TasksToExecute,int StartDelay, workFunction TimerFcn,queue *q){

  t->Period = Period*1000000; //Period in us
  t->TasksToExecute = TasksToExecute;
  t->StartDelay = StartDelay*1000000; // Delay in us
  t->TimerFcn = TimerFcn;
//  t->UserData = UserData;
  t->q = q;

  //StartFcn();
}
void start(timer *t,pthread_t *pro){
  // for(int i=0; i < p; i++){
  //   pthread_create (&(pro[i]), NULL, producer,(void *)&(t[i]));
  // }
  pthread_create (pro, NULL, producer,(void *)t);
}
void StopFcn(timer *t){
    free(t);
}
