#ifndef QUEUE_H
#define QUEUE_H

#define QUEUESIZE 10

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
  int TasksToExecute;
  int *cons_delay;
  // counts executed counts how many times the function has been executed
  int *times_executed;
  bool *done;
  // mutex that for variable times_executed
  pthread_mutex_t *work_mutex;
  pthread_cond_t *execution_complete;

}workFunction;

typedef struct {
  workFunction *buf[QUEUESIZE];
  long head, tail;
  int full, empty;
  // global_done: counts how many producers finished their work
  int global_done;
  bool flag;
  /*This part is added for testing purposes
  added mutex variable im_done to update counter whenever a producer is done*/

  // mutex that is used when the global_done variable needs to be changed
  pthread_mutex_t *all_done;
  pthread_mutex_t *mut;
  /* condition done added to let know the main thread that all the executions
  where completed */
  pthread_cond_t *notFull, *notEmpty, *done;

} queue;

queue *queueInit(void);
void queueDelete(queue *q);
//change the in and out to workFunction type
void queueAdd(queue *q, workFunction *in);
void queueDel(queue *q, workFunction *out);


#endif
