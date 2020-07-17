#include "timer.h"

/*
  Initializes the timer parameters
*/
void timerInit(timer *t, int Period,int TasksToExecute,int StartDelay,void *(*TimerFcn)(void *),void *UserData,queue *q){

  StartFcn(t);
  producers++;
  // If its the first producer then create the consumer threads 
  if(producers == 1){

    /*
      Initialization of the consumer threads
    */
    con = (pthread_t *)malloc(c*sizeof(pthread_t));
    if(con == NULL){
      fprintf (stderr, "Timer.c: Unable to allocate consumer.\n");
      exit(1);
    }
    // Create c consumer threads
    for(int j=0;j<c;j++){
      pthread_create(&con[j], NULL, consumer,(void *)q);
    }
    
  }
  // Assingments regarding the timer
  t->Period = Period*1000;
  t->TasksToExecute = TasksToExecute;
  t->StartDelay = StartDelay;
  t->TimerFcn = (workFunction *)malloc(sizeof(workFunction));
  if(t->TimerFcn == NULL){
    fprintf(stderr, "Timer.c: Unable to allocate TimerFcn.\n");
    exit(1);
  }
  t->TimerFcn->work = TimerFcn;
  t->UserData = UserData;
  t->q = q;

  // Assingments regarding the TimerFcn
  t->TimerFcn->arg = t->UserData;
  t->TimerFcn->TasksToExecute = TasksToExecute;
  t->TimerFcn->times_executed = (int *)malloc(sizeof(int));
  if(t->TimerFcn->times_executed == NULL){
    fprintf(stderr, "Timer.c: Unable to allocate times_executed.\n");
    exit(1);
  }
  *t->TimerFcn->times_executed = 0;
  t->TimerFcn->cons_delay = (int *)malloc(TasksToExecute*sizeof(int));
  if(t->TimerFcn->cons_delay == NULL){
    fprintf(stderr, "Timer.c: Unable to allocate cons_delay.\n");
    exit(1);
  }
  t->TimerFcn->queue_lag = (int *)malloc(TasksToExecute*sizeof(int));
  if(t->TimerFcn->queue_lag == NULL){
    fprintf(stderr, "Timer.c: Unable to allocate queue_lag.\n");
    exit(1);
  }
  t->TimerFcn->done = (bool *)malloc(sizeof(bool));
  *t->TimerFcn->done = 0;
  if(t->TimerFcn->done == NULL){
    fprintf(stderr, "Timer.c: Unable to allocate done.\n");
    exit(1);
  }
  *t->TimerFcn->done = 0;
  t->TimerFcn->work_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
  if(t->TimerFcn->work_mutex == NULL){
    fprintf(stderr, "Timer.c: Unable to allocate work_mutex.\n");
    exit(1);
  }
  pthread_mutex_init(t->TimerFcn->work_mutex,NULL);
  t->TimerFcn->execution_complete = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
  if(t->TimerFcn->execution_complete == NULL){
    fprintf(stderr, "Timer.c: Unable to allocate execution_complete\n");
    exit(1);
  }
  pthread_cond_init(t->TimerFcn->execution_complete, NULL);

  // Producer thread allocation for the timer
  t->pro = (pthread_t *)malloc(sizeof(pthread_t));
  if(t->pro == NULL){
    fprintf(stderr, "Timer.c: Unable to allocate producers.\n");
    exit(1);
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
*/
void StopFcn(timer *t){
  fprintf(stderr, "Stop Function.\n");
}

void ErrorFcn(void){
  fprintf(stderr, "Buffer overflow!.\n");
}

/*
  Mutex/Cond destroy and memory deallocation
*/
void TimerStop(timer *t){

  fprintf(stderr, "Deallocating space...\n");
  free(t->pro);
  free(t->TimerFcn->times_executed);
  pthread_mutex_destroy(t->TimerFcn->work_mutex);
  free(t->TimerFcn->work_mutex);
  pthread_cond_destroy(t->TimerFcn->execution_complete);
  free(t->TimerFcn->execution_complete);
  free(t->TimerFcn->done);
  free(t->TimerFcn->cons_delay);
  free(t->TimerFcn);
  free(t);

  /*
    Update the global_done variable and if all the producers are done 
      send signal to main to move on 
  */
  pthread_mutex_lock(all_done);
  global_done++;
  if(global_done == producers){
    flag = 1;
    pthread_cond_signal(done);
  }
  pthread_mutex_unlock(all_done);

}
