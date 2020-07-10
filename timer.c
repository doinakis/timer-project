#include "timer.h"

/*
  Initializes the timer parameters
*/
void timerInit(timer *t, int Period,int TasksToExecute,int StartDelay, workFunction *TimerFcn,void *UserData,queue *q){

  StartFcn(t);
  // Assingments regarding the timer
  t->Period = Period*1000000;
  t->TasksToExecute = TasksToExecute;
  t->StartDelay = StartDelay;
  t->TimerFcn = TimerFcn;
  t->UserData = UserData;
  t->q = q;

  // Assingments regarding the TimerFcn
  t->TimerFcn->arg = t->UserData;
  t->TimerFcn->TasksToExecute = TasksToExecute;
  t->TimerFcn->times_executed = (int *)malloc(sizeof(int));
  if(t->TimerFcn->times_executed == NULL){
    fprintf(stderr, "Unable to allocate producer.\n");
    exit (1);
  }
  *t->TimerFcn->times_executed = 0;
  t->TimerFcn->done = (bool *)malloc(sizeof(bool));
  *t->TimerFcn->done = 0;
  if(t->TimerFcn->done == NULL){
    fprintf(stderr, "Unable to allocate producer.\n");
    exit (1);
  }
  *t->TimerFcn->done = 0;
  t->TimerFcn->work_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
  if(t->TimerFcn->work_mutex == NULL){
    fprintf(stderr, "Unable to allocate producer.\n");
    exit (1);
  }
  pthread_mutex_init(t->TimerFcn->work_mutex,NULL);
  t->TimerFcn->execution_complete = (pthread_cond_t *)malloc(sizeof (pthread_cond_t));
  if(t->TimerFcn->execution_complete == NULL){
    fprintf(stderr, "Unable to allocate producer.\n");
    exit (1);
  }
  pthread_cond_init(t->TimerFcn->execution_complete, NULL);

  // Producer thread allocation for the timer
  t->pro = (pthread_t *)malloc(sizeof(pthread_t));
  if(t->pro == NULL){
    fprintf(stderr, "Unable to allocate producer.\n");
    exit (1);
  }

}

void start(timer *t){
  // Starts the timer thread
  pthread_create(t->pro, NULL, producer,(void *)t);
}

void startat(timer *t,int y,int m,int d,int h,int min,int sec){

  // The amount of seconds that need to be delayed
  int seconds;

  // get the local time and convert it into seconds
  time_t local_time = time(NULL);
  struct tm date = *localtime(&local_time);
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

  if(seconds < 0) seconds = 0;
  seconds = seconds;
  // Add the additional delay to the StartDelay variable
  t->StartDelay += seconds;

  // Create a producer thread for the given timer
  pthread_create(t->pro, NULL, producer,(void *)t);

}

void StartFcn(timer *t){
  // This function can be changed by the user depending on the application
  fprintf(stderr, "Start function for initialization.\n");
}

/*
  Mutex/Cond destroy and memory deallocation
*/
void StopFcn(timer *t){
  fprintf(stderr, "lock...\n");
  pthread_mutex_lock(t->q->all_done);
  t->q->global_done++;
  printf("%d\n",t->q->global_done );
  pthread_mutex_unlock(t->q->all_done);
  fprintf(stderr, "unlock...\n");
  if(t->q->global_done == p){
    t->q->flag = 1;
    fprintf(stderr, "signal...\n");
    pthread_cond_signal(t->q->done);
  }
  fprintf(stderr, "Stop function deallocating space...\n");
  free(t->pro);
  // free(t->TimerFcn->delay_time);
  free(t->TimerFcn->times_executed);
  pthread_mutex_destroy(t->TimerFcn->work_mutex);
  free(t->TimerFcn->work_mutex);
  pthread_cond_destroy(t->TimerFcn->execution_complete);
  free(t->TimerFcn->execution_complete);
  free(t->TimerFcn->done);
  free(t);

}

void ErrorFcn(void){
  fprintf(stderr, "Buffer overflow!.\n");
}
