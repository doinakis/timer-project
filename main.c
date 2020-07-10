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
   timer *t1 = (timer *)malloc(sizeof(timer));
   timer *t2 = (timer *) malloc(sizeof(timer));
  // t->Period = 5;
  // k = queueInit();
  // return 0;
  void *pointer;
  workFunction *work1 = (workFunction *)malloc(sizeof(workFunction));
  workFunction *work2 = (workFunction *)malloc(sizeof(workFunction));
  work1->work = functions_array[0];
  pointer = &random_arguments[0];
  //work1->arg = pointer;
  queue *fifo;

  pthread_t *con = (pthread_t *)malloc(c * sizeof(pthread_t));
  if(con == NULL){
    fprintf (stderr, "Unable to allocate consumer.\n");
    exit (1);
  }

  fifo = queueInit ();

  if (fifo ==  NULL) {
    fprintf(stderr, "Main: Queue Init failed.\n");
    exit (1);
  }
  timerInit(t1,1,10,0,work1,&random_arguments[0],fifo);
  work2->work = functions_array[1];
  pointer = &random_arguments[1];
  //work2->arg = pointer;
  timerInit(t2,2,10,0,work2,&random_arguments[1],fifo);
  //timerInit(&t[1],5,3,0,work,&random_arguments[0],fifo);
  //start(t);
  // work.work = functions_array[1];
  // pointer = &random_arguments[1];
  // work.arg = pointer;
  // timerInit(&t[1],3,6,0,work,fifo);
  startat(t1,2020,7,10,12,39,0);
  startat(t2,2020,7,10,12,39,0);
  // for(int i =0; i<p;i++){
  //   pthread_create(&pro[i], NULL, producer,(void *)t);
  // }
  //
  // //create c consumer threads
  for(int j=0;j<c;j++){
    pthread_create(&con[j], NULL, consumer,(void *)fifo);
  }
  //
  // //wait for all the producers/consumers to finish their work
  // for(int i=0;i<p;i++){
  //   pthread_join (pro[i], NULL);
  // }
  //
  for(int j=0;j<c;j++){
    pthread_join(con[j], NULL);
  }

  //fclose(f);

  queueDelete (fifo);

  return 0;
}
