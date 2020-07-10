#include "prod-cons.h"

void *producer(void *q)
{
  // Pointer to the queue
  queue *fifo;
  int i;

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

  // Delay the required amount of seconds
  sleep(t->StartDelay);
  for (i = 0; i < t->TasksToExecute; i++) {
    usleep(t->Period);
    pthread_mutex_lock(fifo->mut);
    while(fifo->full) {
      printf("producer: queue FULL.\n");
      ErrorFcn();
      pthread_cond_wait(fifo->notFull, fifo->mut);
    }

    //add a random function in the queue and a random argument
    // workFunction  work;
    // work.work = functions_array[rand()%8];
    // pointer = &random_arguments[rand()%10];
    // work.arg = pointer;
    /*This part of the code is added for testing purposes*/

    // gettimeofday(&work.start_time,NULL);

    queueAdd(fifo, t->TimerFcn);
    printf("Delay %d.\n",t->TimerFcn->TasksToExecute);
    pthread_mutex_unlock(fifo->mut);
    pthread_cond_signal(fifo->notEmpty);
  }
  // Lock the mutex to check the done variable
  pthread_mutex_lock(t->TimerFcn->work_mutex);
  // While dont is 0
  while(!*(t->TimerFcn->done)){

    // Wait for execution complete signal and unlock the variable
    pthread_cond_wait(t->TimerFcn->execution_complete,t->TimerFcn->work_mutex);
  }
  /* When the signal arrives (it means the function executed the required amount
    of times ) unlock the mutex variable
  */

  pthread_mutex_unlock(t->TimerFcn->work_mutex);
  printf("signal received!\n");
  StopFcn(t);
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
      printf("consumer: queue EMPTY.\n");
      pthread_cond_wait(fifo->notEmpty, fifo->mut);
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
    //*d.delay_time += 1;
    pthread_mutex_lock(d.work_mutex);
    *d.times_executed += 1;
    pthread_mutex_unlock(d.work_mutex);
    printf("alrigt %d!\n",*d.times_executed);
    if(*d.times_executed == d.TasksToExecute){
      *d.done = 1;
      pthread_cond_signal(d.execution_complete);
      printf("signal send\n");
    }
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
  for(int i =0; i < 9; i++){
    sin(rand() % 361);
  }
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
