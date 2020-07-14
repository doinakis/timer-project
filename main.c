#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

// Include costum libraries
#include "timer.h"
#include "queue.h"
#include "prod-cons.h"


int *kill_flag;

/*
  Killing: its a function that takes as an argument a queue and waits until the
  flag bool is true. If all the consumers are done and the dead flag is true then
  the kill flag is set to 1 and all the consumers can now terminate. It waits a
  done signal that is sent by the last active timer.
*/
void *killing(void *dead_queue){
  queue *dead = (queue *) dead_queue;
  pthread_mutex_lock(dead->all_done);
  while(!dead->flag){
    pthread_cond_wait(dead->done,dead->all_done);
  }
  *kill_flag = 1;
  printf("DO WHATEVER \n");
  return(NULL);

}


int main(void){

  /*
    Flag that indicates that all the consumers are done executing and are ready
      to terminate
  */
  kill_flag = (int *)malloc(sizeof(int));
  *kill_flag = 0;
  /*
    Initialization of the fifo queue where the producer add the functions and the
      consumers grabs them
  */
  queue *fifo;
  fifo = queueInit();
  if (fifo ==  NULL){
    fprintf(stderr, "Main: Queue Init failed.\n");
    exit (1);
  }

  /*
    Initialization of the consumer threads
  */
  pthread_t *con;
  con = (pthread_t *)malloc(c * sizeof(pthread_t));
  if(con == NULL){
    fprintf (stderr, "Unable to allocate consumer.\n");
    exit (1);
  }
  // Create c consumer threads
  for(int j=0;j<c;j++){
    pthread_create(&con[j], NULL, consumer,(void *)fifo);
  }

  /*
    Timer structure and funciton pointers allocations
  */
  timer *t1 = (timer *)malloc(sizeof(timer));
  timer *t2 = (timer *)malloc(sizeof(timer));
  timer *t3 = (timer *)malloc(sizeof(timer));

  workFunction *work1 = (workFunction *)malloc(sizeof(workFunction));
  workFunction *work2 = (workFunction *)malloc(sizeof(workFunction));
  workFunction *work3 = (workFunction *)malloc(sizeof(workFunction));
  work1->work = functions_array[0];
  work2->work = functions_array[1];
  work3->work = functions_array[2];
  /*
    Timer initialization and starting point
  */
  timerInit(t1,1000,10,0,work1,&random_arguments[0],fifo);
  timerInit(t2,100,10,0,work2,&random_arguments[1],fifo);
  timerInit(t3,10,10,0,work3,&random_arguments[2],fifo);
  start(t1);
  start(t2);
  start(t3);

  /*
    Functions that waits all the consumers to finish their work and makes sure that
    all the timers are also done adding their functions in the fifo
  */
  killing((void*)fifo);

  /*
    This part is for terminating all the consumer threads properly
  */
  fifo->empty = 0;
  for(int j=0;j<c;j++){
    pthread_mutex_lock(fifo->mut);
    pthread_mutex_unlock(fifo->mut);
    pthread_cond_signal(fifo->notEmpty);
  }

  /*
    This part is to make sure that all the consumers terminated
  */
  for(int j=0;j<c;j++){
    pthread_join (con[j], NULL);
  }
  printf("JOINED CONS\n");

  /*
    Free all the allocated space
  */
  free(work1);
  free(work2);
  free(work3);
  free(con);
  queueDelete(fifo);
  printf("THE END...\n");
  return 0;
}
