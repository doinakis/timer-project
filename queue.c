#include "queue.h"


queue *queueInit (void)
{
  queue *q;

  q = (queue *)malloc(sizeof (queue));
  if (q == NULL) return (NULL);

  q->done = 0;
  q->empty = 1;
  q->full = 0;
  q->head = 0;
  q->tail = 0;
  q->mut = (pthread_mutex_t *)malloc(sizeof (pthread_mutex_t));
  pthread_mutex_init(q->mut, NULL);

  /*This part of the code is added for testing purposes*/

  // q->im_done = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
  // pthread_mutex_init (q->im_done, NULL);

  q->notFull = (pthread_cond_t *)malloc(sizeof (pthread_cond_t));
  pthread_cond_init(q->notFull, NULL);
  q->notEmpty = (pthread_cond_t *)malloc(sizeof (pthread_cond_t));
  pthread_cond_init(q->notEmpty, NULL);

  return (q);
}

void queueDelete(queue *q)
{
  pthread_mutex_destroy(q->mut);
  free(q->mut);
  /*This part of the code is added for testing purposes*/

  // pthread_mutex_destroy (q->im_done);
  // free (q->im_done);

  pthread_cond_destroy(q->notFull);
  free(q->notFull);
  pthread_cond_destroy(q->notEmpty);
  free(q->notEmpty);
  free(q);
}

//changed the in variable to be type workFunction
void queueAdd(queue *q, workFunction *in)
{
  q->buf[q->tail] = in;
  q->tail++;
  if (q->tail == QUEUESIZE)
    q->tail = 0;
  if (q->tail == q->head)
    q->full = 1;
  q->empty = 0;

  return;
}

//changed the out variable to be type workFunction
void queueDel(queue *q, workFunction *out)
{
  *out = *q->buf[q->head];

  q->head++;
  if (q->head == QUEUESIZE)
    q->head = 0;
  if (q->head == q->tail)
    q->empty = 1;
  q->full = 0;

  return;
}
