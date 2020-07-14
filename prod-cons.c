#include "prod-cons.h"

extern int *kill_flag;
void *producer(void *q)
{
  // Pointer to the queue
  queue *fifo;

  // Pointer to the timer
  timer *t;
  // Add a void variable to pass the integer as void * type to the function
  void * pointer;
  // Srand to pick a function at random
  srand(time(NULL));

  // Typecast the void* to a timer. t now points to the timer
  t = (timer *)q;
  // Fifo now points at q
  fifo = t->q;
  FILE *f;

  char name[15];
  sprintf(name,"drift_%d.csv",((t->Period)/1000));
  f = fopen(name,"wb");
  int drift_calc[t->TasksToExecute];
  int adjust = t->Period; // Period*1000 to convert to microseconds
  printf("%d\n", adjust);
  // Delay the required amount of seconds
  sleep(t->StartDelay);
  for(int i = 0; i < t->TasksToExecute; i++) {
    gettimeofday(&t->TimerFcn->start_time,NULL);
    pthread_mutex_lock(fifo->mut);
    /*
      If the fifo is full then the function misses its execution waits the Period
    and tries at the next period
    */
    if(fifo->full){

      // Handling of the buffer overflow by the user
      ErrorFcn();
      pthread_mutex_unlock(fifo->mut);
      // Count that to the times times_executed although the function is dropped
      pthread_mutex_lock(t->TimerFcn->work_mutex);
      *t->TimerFcn->times_executed += 1;
      if(*t->TimerFcn->times_executed == t->TimerFcn->TasksToExecute){
        *t->TimerFcn->done = 1;
        pthread_cond_signal(t->TimerFcn->execution_complete);
      }
      pthread_mutex_unlock(t->TimerFcn->work_mutex);
      if(i != t->TasksToExecute - 1){
        usleep(adjust);
      }
      // Drift calculation and Period adjustment
      gettimeofday(&t->TimerFcn->end_time,NULL);
      t->TimerFcn->delay_time = (unsigned int)((t->TimerFcn->end_time.tv_usec-t->TimerFcn->start_time.tv_usec + (t->TimerFcn->end_time.tv_sec-t->TimerFcn->start_time.tv_sec)*1e06 - t->Period));
      adjust = adjust - t->TimerFcn->delay_time;
      if(adjust < 0) adjust = 0;
      drift_calc[i] = t->TimerFcn->delay_time;
      continue;
    }

    queueAdd(fifo, t->TimerFcn);
    pthread_mutex_unlock(fifo->mut);
    pthread_cond_signal(fifo->notEmpty);
    if(i != t->TasksToExecute - 1){
      usleep(adjust);
    }
    // Drift calculation and Period adjustment
    gettimeofday(&t->TimerFcn->end_time,NULL);
    t->TimerFcn->delay_time = (unsigned int)((t->TimerFcn->end_time.tv_usec-t->TimerFcn->start_time.tv_usec + (t->TimerFcn->end_time.tv_sec-t->TimerFcn->start_time.tv_sec)*1e06 - t->Period));
    adjust = adjust - t->TimerFcn->delay_time;

    if(adjust < 0) adjust = 0;
    drift_calc[i] = t->TimerFcn->delay_time;

  }
  // Lock the mutex to check the done variable
  pthread_mutex_lock(t->TimerFcn->work_mutex);
  // While done is 0
  while(!*(t->TimerFcn->done)){

    // Wait for execution complete signal and unlock the variable
    pthread_cond_wait(t->TimerFcn->execution_complete,t->TimerFcn->work_mutex);
  }
  /* When the signal arrives (it means the function executed the required amount
    of times ) unlock the mutex variable
  */
  pthread_mutex_unlock(t->TimerFcn->work_mutex);
  for(int i=0;i<t->TasksToExecute;i++){
    fprintf(f,"%d\n",drift_calc[i] );
  }
  //fwrite(drift_calc, sizeof(int), t->TasksToExecute, f);
  fclose(f);
  StopFcn(t);
  TimerStop(t);
  return (NULL);
}

void *consumer(void *q)
{
  queue *fifo;
  // Make d a workFunction variable
  workFunction d;
  fifo = (queue *)q;

  // Change the loop to while 1
  while(1){
    pthread_mutex_lock(fifo->mut);
    while (fifo->empty) {
      printf("consumer: queue EMPTY .\n");
      pthread_cond_wait(fifo->notEmpty, fifo->mut);
    }
    if(*kill_flag){
      pthread_mutex_unlock(fifo->mut);
      return(NULL);
    }

    // d is a workFunction variable produced by the producer
    queueDel(fifo, &d);
    pthread_mutex_unlock(fifo->mut);
    pthread_cond_signal(fifo->notFull);

    /*
      The execution of the function takes place outside the mutex assuming that
        its execution doesnt interfere with other executions(e.g write at the same
        files etc.)
    */
    (*d.work)(d.arg);

    /* right after the execution update the execution times for the corresponding
      function. If the execution times are equal to the tasks executed then it
      sends a signal to the producer thread that the executions are done so that
      it can run the stop function for that timer
      */
    pthread_mutex_lock(d.work_mutex);
    *d.times_executed += 1;
    if(*d.times_executed == d.TasksToExecute){
      *d.done = 1;
      pthread_cond_signal(d.execution_complete);
    }
    pthread_mutex_unlock(d.work_mutex);
  }
  return (NULL);
}

//series of  functions that just print something
void* function_print_1(void* arg){
  //print a message
  printf("Function 1 called randomly, the random argument is : %d  \n",*((int *) arg));
  return (NULL);
}

void*  function_print_2(void* arg){
  //print a message
  printf("Function 2 called randomly, the random argument is : %d  \n",*((int *) arg));
  return (NULL);
}

void* function_sin(void* arg){
  srand(time(NULL));
  //compute 10 random integer sins
  // for(int i =0; i < 9; i++){
  //   sin(rand() % 361);
  // }
  printf("Function 3 called randomly, the random argument is : %d  \n",*((int *) arg));
  return (NULL);
}

void*  function_cos(void* arg){
  srand(time(NULL));
  //compute 10 random integer cos
  for(int i =0; i < 9; i++){
    cos(rand() % 361);
  }
  return (NULL);
}

void*  function_asin(void* arg){
  srand(time(NULL));
  //compute 10 random integer asin
  for(int i =0; i < 9; i++){
    asin(rand() % 361);
  }
  return (NULL);
}

void*  function_acos(void* arg){
  srand(time(NULL));
  //compute 10 random integer acos
  for(int i =0; i < 9; i++){
    acos(rand() % 361);
  }
  return (NULL);
}

void*  function_tan(void* arg){
  srand(time(NULL));
  //compute 10 random integer tan
  for(int i =0; i < 9; i++){
    tan(rand() % 361);
  }
  return (NULL);
}

void*  function_atan(void* arg){
  srand(time(NULL));
  //compute 10 random integer atan
  for(int i =0; i < 9; i++){
    atan(rand() % 361);
  }
  return (NULL);
}
