#include "timer.h"

/*
  Initializes the timer parameters
*/
void timerInit(timer *t, int Period,int TasksToExecute,int StartDelay, workFunction TimerFcn,void *UserData,queue *q){

  t->Period = Period*1000000;
  t->TasksToExecute = TasksToExecute;
  t->StartDelay = StartDelay;
  t->TimerFcn = TimerFcn;
  t->TimerFcn.delay_time = (int *)malloc(sizeof(int));
  t->UserData = UserData;
  t->TimerFcn.arg = t->UserData;
  t->q = q;

}

/*
  Starts the array of timers set in main function and assigns them to the same
  number of consumers
*/
void start(timer *t,pthread_t *pro){
  // for(int i=0; i < p; i++){
  //   pthread_create (&(pro[i]), NULL, producer,(void *)&(t[i]));
  // }
  pthread_create(pro, NULL, producer,(void *)t);
}
/*
  Starts the timer at the given time
*/
void startat(timer *t,int y,int m,int d,int h,int min,int sec,pthread_t *pro){
  // The amount of seconds that need to be delayed
  int seconds;
  // get the local time and convert it into seconds
  time_t local_time=time(NULL);
  struct tm date= *localtime(&local_time);
  date.tm_year += 1900;
  date.tm_mon += 1;
  y = (y - date.tm_year)*31556926;
  m = (m - date.tm_mon)*2629743.83;
  d = (d - date.tm_mday)*86400;
  h = (h - date.tm_hour)*3600;
  min = (min - date.tm_min)*60;
  sec = sec - date.tm_sec;
  seconds = (y+m+d+h+min+sec);

  /*
      If the total seconds < 0 that means that the time given was in the past so
      the total seconds to sleep are 0

  */

  if (seconds < 0) seconds = 0;
  seconds = seconds;
  // Add the additional delay to the StartDelay variable
  t->StartDelay += seconds;
  //testing
  //printf("waiting for %d %d %d %d %d %d seconds %d \n",y,m,d,h,min,sec,t->StartDelay);

  // Create a producer thread for the given timer
  pthread_create(pro, NULL, producer,(void *)t);

}
/*
  Function that executes at the first time of initialization of the timer
*/
void StartFcn(timer *t){
  t->TimerFcn.arg = t->UserData;
}
/*
  Functions that executes right after the last execution of the TimerFcn
*/
void StopFcn(timer *t){
    free(t);
}
/*
  Function that executes in case of buffer overflow
*/
void ErrorFcn(timer *t){

}
