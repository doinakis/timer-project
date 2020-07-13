#ifndef QUEUE_H
#define QUEUE_H

#define QUEUESIZE 2

#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
/*
    ----------workFunction Stracture----------
  work: void pointer to function
  arg: void pointer to function arguments
  TasksToExecute: Times this function must be executed
  work_mutex: Mutex variable to update times_executed variable
  times_executed: How many times the function executed
  execution_complete: Condition variable to show that the function completed its
    executions
  done: Variable that show if the execution completed
*/
typedef struct{

  void *(*work)(void*);
  void *arg;
  /*This part is added for testing purposes
    variables that hold when a function is added to the queue, when it arrives
    at the consumer and the substraction is the delay_time*/
   struct timeval start_time,end_time;
   unsigned int delay_time;
   int TasksToExecute;
   int *times_executed;
   bool *done;
   pthread_mutex_t *work_mutex;
   pthread_cond_t *execution_complete;

}workFunction;

typedef struct {
  workFunction *buf[QUEUESIZE];
  long head, tail;
  int full, empty;
  int global_done;
  bool flag;
  /*This part is added for testing purposes
  added mutex variable im_done to update counter whenever a producer is done*/

  pthread_mutex_t *all_done;
  pthread_mutex_t *mut;
  pthread_cond_t *notFull, *notEmpty, *done;

} queue;

queue *queueInit(void);
void queueDelete(queue *q);
//change the in and out to workFunction type
void queueAdd(queue *q, workFunction *in);
void queueDel(queue *q, workFunction *out);


#endif
