#ifndef QUEUE_H
#define QUEUE_H

#define QUEUESIZE 1500

#include <stdlib.h>
#include <pthread.h>
typedef struct{

  void *(*work)(void*);
  void *arg;
  /*This part is added for testing purposes
    variables that hold when a function is added to the queue, when it arrives
    at the consumer and the substraction is the delay_time*/
  // struct timeval start_time,end_time;
   int *delay_time;

}workFunction;

typedef struct {
  workFunction buf[QUEUESIZE];
  long head, tail;
  int full, empty;

  /*This part is added for testing purposes
  added mutex variable im_done to update counter whenever a producer is done*/

  // pthread_mutex_t *im_done;

  pthread_mutex_t *mut;
  pthread_cond_t *notFull, *notEmpty;

  //variable thst holds how many producer are done
  int done;

} queue;

queue *queueInit (void);
void queueDelete (queue *q);
//change the in and out to workFunction type
void queueAdd (queue *q, workFunction in);
void queueDel (queue *q, workFunction *out);


#endif
