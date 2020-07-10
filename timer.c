#include "timer.h"

/*
  Initializes the timer parameters
*/
void timerInit(timer *t, int Period,int TasksToExecute,int StartDelay, workFunction *TimerFcn,void *UserData,queue *q){
  StartFcn(t);
  t->Period = Period*1000000;
  t->TasksToExecute = TasksToExecute;
  t->StartDelay = StartDelay;
  t->TimerFcn = TimerFcn;
  t->TimerFcn->TasksToExecute = TasksToExecute;
  t->TimerFcn->work_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(t->TimerFcn->work_mutex,NULL);
  t->TimerFcn->times_executed = (int *)malloc(sizeof(int));
  t->TimerFcn->done = (int *)malloc(sizeof(int));
  *t->TimerFcn->done = 0;
  *t->TimerFcn->times_executed = 0;
  t->TimerFcn->execution_complete = (pthread_cond_t *)malloc(sizeof (pthread_cond_t));
  pthread_cond_init(t->TimerFcn->execution_complete, NULL);
  t->UserData = UserData;
  t->TimerFcn->arg = t->UserData;
  t->q = q;
  t->pro = (pthread_t *)malloc(sizeof(pthread_t));
  if(t->pro == NULL){
    fprintf(stderr, "Unable to allocate producer.\n");
    exit (1);
  }

}

/*
  Starts the array of timers set in main function and assigns them to the same
  number of consumers
*/
void start(timer *t){
  pthread_create(t->pro, NULL, producer,(void *)t);
}
/*
  Starts the timer at the given time
*/
void startat(timer *t,int y,int m,int d,int h,int min,int sec){

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
  pthread_create(t->pro, NULL, producer,(void *)t);

}
/*
  Function that executes at the first time of initialization of the timer
*/
void StartFcn(timer *t){
  fprintf(stderr, "Start function for initialization.\n");
}
/*
  Functions that executes right after the last execution of the TimerFcn
*/
void StopFcn(timer *t){

  fprintf (stderr, "Stop function dealocating space...\n");
  free(t->pro);
  // free(t->TimerFcn->delay_time);
  free(t->TimerFcn->times_executed);
  pthread_mutex_destroy(t->TimerFcn->work_mutex);
  free(t->TimerFcn->work_mutex);
  pthread_cond_destroy(t->TimerFcn->execution_complete);
  free(t->TimerFcn->execution_complete);
  free(t);

}
/*
  Function that executes in case of buffer overflow
*/
void ErrorFcn(void){
  fprintf(stderr, "Buffer overflow!.\n");
}
