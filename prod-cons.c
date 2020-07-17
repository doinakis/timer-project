#include "prod-cons.h"

void *producer(void *q)
{
  queue *fifo;
  // Drift due to the consumer delay  
  int delay_time;
  struct timeval start_time,end_time;
  // Pointer to the timer
  timer *t;

  // Srand to pick a function at random
  srand(time(NULL));

  // Typecast the void* to a timer. t now points to the timer
  t = (timer *)q;
  // Fifo now points at q
  fifo = t->q;

  int drift_calc[t->TasksToExecute];
  int adjust = t->Period;
  // Delay the required amount of seconds
  sleep(t->StartDelay);
  for(int i = 0; i < t->TasksToExecute; i++) {
    gettimeofday(&start_time,NULL);
    pthread_mutex_lock(fifo->mut);
    /*
      If the fifo is full then the function misses its execution waits the Period
    and tries at the next period
    */
    if(fifo->full){

      // Handling of the buffer overflow by the user
      ErrorFcn();
      pthread_mutex_unlock(fifo->mut);

      // Althoug the function doesn't execute it misses its execution
      pthread_mutex_lock(t->TimerFcn->work_mutex);
      t->TimerFcn->cons_delay[*t->TimerFcn->times_executed] = -INT_MAX;
      *t->TimerFcn->times_executed += 1;
      if(*t->TimerFcn->times_executed == t->TimerFcn->TasksToExecute){
        *t->TimerFcn->done = 1;
        pthread_cond_signal(t->TimerFcn->execution_complete);
      }
      pthread_mutex_unlock(t->TimerFcn->work_mutex);
      if(i != t->TasksToExecute - 1){
        usleep(adjust);
        delay_time = -t->Period;
      }
      // Drift calculation and Period adjustment
      gettimeofday(&end_time,NULL);
      delay_time += (int)((end_time.tv_usec-start_time.tv_usec + (end_time.tv_sec-start_time.tv_sec)*1e06));
      adjust = adjust - delay_time;

      if(adjust < 0) adjust = 0;
      drift_calc[i] = delay_time;
      continue;
    }
    gettimeofday(&t->TimerFcn->queue_in,NULL);
    queueAdd(fifo, t->TimerFcn);
    pthread_mutex_unlock(fifo->mut);
    pthread_cond_signal(fifo->notEmpty);
    if(i != t->TasksToExecute - 1){
      usleep(adjust);
      delay_time = -t->Period;
    }
    // Drift calculation and Period adjustment
    gettimeofday(&end_time,NULL);
    delay_time += (int)((end_time.tv_usec-start_time.tv_usec + (end_time.tv_sec-start_time.tv_sec)*1e06));
    adjust = adjust - delay_time;

    if(adjust < 0) adjust = 0;
    drift_calc[i] = delay_time;
    
  }
  // Lock the mutex to check the done variable
  pthread_mutex_lock(t->TimerFcn->work_mutex);
  // While done is 0
  while(!*(t->TimerFcn->done)){

    // Wait for execution complete signal and unlock the variable
    pthread_cond_wait(t->TimerFcn->execution_complete,t->TimerFcn->work_mutex);
  }
  /* When the signal arrives (it means the function executed the required amount
    of times) unlock the mutex variable
  */
  pthread_mutex_unlock(t->TimerFcn->work_mutex);

  // Run the StopFcn
  StopFcn(t);

  // Files to write the results of the experiments 
  FILE *f1,*f2,*f3;
  char name1[15];
  char name2[15];
  char name3[20];
  sprintf(name1,"drift_%d.csv",((t->Period)/1000));
  sprintf(name2,"cons_%d.csv",((t->Period)/1000));
  sprintf(name3,"queuelag%d.csv",((t->Period)/1000));
  f1 = fopen(name1,"wb");
  f2 = fopen(name2,"wb");
  f3 = fopen(name3,"wb");
  // The producer is respnsible of writing the results to the files
  for(int i=0;i<t->TasksToExecute;i++){
    fprintf(f1,"%d\n",drift_calc[i]);
    fprintf(f2,"%d\n",t->TimerFcn->cons_delay[i]);
    fprintf(f3,"%d\n",t->TimerFcn->queue_lag[i]);
  }

  fclose(f1);
  fclose(f2);
  fclose(f3);
  TimerStop(t);
  return (NULL);
}

void *consumer(void *q)
{
  queue *fifo;
  // Make d a workFunction variable
  workFunction d;
  fifo = (queue *)q;
  int delay_time;
  struct timeval start_time,end_time;
  // Change the loop to while 1
  while(1){
    
    pthread_mutex_lock(fifo->mut);
    while (fifo->empty) {
      printf("consumer: queue EMPTY .\n");
      pthread_cond_wait(fifo->notEmpty, fifo->mut);
    }
    
    if(kill_flag){
      pthread_mutex_unlock(fifo->mut);
      return(NULL);
    }
    gettimeofday(&start_time,NULL);
    // d is a workFunction variable produced by the producer
    queueDel(fifo, &d);
    
    pthread_mutex_unlock(fifo->mut);
    pthread_cond_signal(fifo->notFull);

    /*
      The execution of the function takes place outside the mutex assuming that
        its execution doesnt interfere with other executions(e.g write at the same
        files etc.)
    */
    gettimeofday(&end_time,NULL);

    (*d.work)(d.arg);
    /* Right after the execution update the execution times for the corresponding
      function. If the execution times are equal to the tasks executed then it
      sends a signal to the producer thread that the executions are done so that
      it can run the stop function for that timer
    */
    pthread_mutex_lock(d.work_mutex);
    d.cons_delay[*d.times_executed] = (int)((end_time.tv_usec-start_time.tv_usec + (end_time.tv_sec-start_time.tv_sec)*1e06));
    d.queue_lag[*d.times_executed] = (int)((end_time.tv_usec-d.queue_in.tv_usec + (end_time.tv_sec-d.queue_in.tv_sec)*1e06));
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
