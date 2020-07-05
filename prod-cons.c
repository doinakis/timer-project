#include "prod-cons.h"

void *producer (void *q)
{
  queue *fifo;
  int i;

  //add a void variable to pass the integer as void * type to the function
  void * pointer;
  //srand to pick a function at random
  srand(time(NULL));

  fifo = (queue *)q;
  for (i = 0; i < LOOP; i++) {
    pthread_mutex_lock (fifo->mut);
    while (fifo->full) {
      printf ("producer: queue FULL.\n");
      pthread_cond_wait (fifo->notFull, fifo->mut);
    }

    //add a random function in the queue and a random argument
    workFunction  work;
    work.work = functions_array[rand()%8];
    pointer = &random_arguments[rand()%10];
    work.arg = pointer;
    /*This part of the code is added for testing purposes*/

    // gettimeofday(&work.start_time,NULL);

    queueAdd (fifo, work);
    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notEmpty);

  }
  /*This part of the code is added for testing purposes
    producer done update variable
  pthread_mutex_lock (fifo->im_done);
  fifo->done++;
  pthread_mutex_unlock (fifo->im_done);
  */

  return (NULL);
}

void *consumer (void *q)
{
  queue *fifo;
  //make d a workFunction variable
  workFunction d;
  fifo = (queue *)q;

  // change the loop to while 1
  while(1){
    pthread_mutex_lock (fifo->mut);
    while (fifo->empty) {
      printf ("consumer: queue EMPTY.\n");
      /*This part of the code is added for testing purposes*/
      //close the file if the queue is empty and all the producers are done
      // if(fifo->done == p){
      //   fclose(f);
      // }

      pthread_cond_wait (fifo->notEmpty, fifo->mut);
    }
    //d is a workFunction variable produced by the producer
    queueDel (fifo, &d);

    /*This part of the code is added for testing purposes*/
    // gettimeofday(&d.end_time,NULL);
    // d.delay_time = (double)((d.end_time.tv_usec-d.start_time.tv_usec)/1.0e6+d.end_time.tv_sec-d.start_time.tv_sec);
    // fprintf(f,"%f \n",d.delay_time);
    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notFull);


    /*keep in mind that with the calculation inside the critical part we may add
    some overhead to the next delay_times */
    //the execution of the function takes place outside the mutex assuming that its execution doesnt interfere with other executions(e.g write at the same files etc.)
    (*d.work)(d.arg);
  }
  return (NULL);
}

//series of  functions that just print something
// compile like that to work gcc func_pointer.c -o func_pointer -lm
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
