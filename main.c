#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
// Include costum libraries
#include "timer.h"
#include "queue.h"
#include "prod-cons.h"




int main(void){
  // queue *k = (queue *) malloc(sizeof(queue));
   timer *t = (timer *) malloc(sizeof(timer));
   // t->Period = 5;
  // k = queueInit();
  // printf("hello motherfucker %d\n",t->Period );
  // return 0;
  void *pointer;
  workFunction work;
  work.work = functions_array[0];
  pointer = &random_arguments[0];
  work.arg = pointer;
  queue *fifo;

   pthread_t *pro = (pthread_t *) malloc(p * sizeof(pthread_t));
   pthread_t *con = (pthread_t *) malloc(c * sizeof(pthread_t));

  fifo = queueInit ();

  if (fifo ==  NULL) {
    fprintf (stderr, "main: Queue Init failed.\n");
    exit (1);
  }
  timerInit(&t[0],5,10,3,work,fifo);
  work.work = functions_array[1];
  pointer = &random_arguments[1];
  work.arg = pointer;
//  timerInit(&t[1],3,6,0,work,fifo);
  start(t,pro);
  // for(int i =0; i<p;i++){
  //   pthread_create (&pro[i], NULL, producer,(void *)t);
  // }
  //
  // //create c consumer threads
  for(int j=0;j<c;j++){
    pthread_create (&con[j], NULL, consumer,(void *)fifo);
  }
  //
  // //wait for all the producers/consumers to finish their work
  // for(int i=0;i<p;i++){
  //   pthread_join (pro[i], NULL);
  // }
  //
  for(int j=0;j<c;j++){
    pthread_join (con[j], NULL);
  }

  //fclose(f);

  queueDelete (fifo);

  return 0;
}
