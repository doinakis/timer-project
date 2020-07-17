#ifndef QUEUE_H
#define QUEUE_H

#include "globaldef.h"

/*
    ----------workFunction Stracture----------
  work: void pointer to function
  arg: void pointer to function arguments
  TasksToExecute: Times this function must be executed
  done: Variable that shows if the execution completed
  times_executed: How many times the function executed
  work_mutex: Mutex variable to update times_executed variable
  execution_complete: Condition variable to show that the function completed its
    executions
  cons_delay: Array that holds the time needed to get the function from the queue
    (its added for testing purposes)
  
*/
typedef struct{

  void *(*work)(void*);
  void *arg;
  int TasksToExecute;
  bool *done;
  int *times_executed;
  pthread_mutex_t *work_mutex;
  pthread_cond_t *execution_complete;
  struct timeval queue_in;
  int *cons_delay,*queue_lag;


}workFunction;

typedef struct {
  workFunction *buf[QUEUESIZE];
  long head, tail;
  int full, empty;
  pthread_mutex_t *mut;
  pthread_cond_t *notFull, *notEmpty;

} queue;

queue *queueInit(void);
void queueDelete(queue *q);
//change the in and out to workFunction type
void queueAdd(queue *q, workFunction *in);
void queueDel(queue *q, workFunction *out);


#endif
