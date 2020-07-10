#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
// Include costum libraries
#include "timer.h"
#include "queue.h"
#include "prod-cons.h"
#include <signal.h>

pthread_t *con;

void *killing(void *dead_queue){
  queue *dead = (queue *) dead_queue;
  pthread_mutex_lock(dead->all_done);
  while(!dead->flag){
    pthread_cond_wait(dead->done,dead->all_done);
  }
  for(int i = 0; i < c; i++){
    pthread_cancel(con[i]);
    printf("killing!\n");
  }
  //queueDelete(dead);
}


int main(void){
  // queue *k = (queue *) malloc(sizeof(queue));
  con = (pthread_t *)malloc(c * sizeof(pthread_t));
  timer *t1 = (timer *)malloc(sizeof(timer));
  // timer *t2 = (timer *) malloc(sizeof(timer));
  // timer *t3 = (timer *) malloc(sizeof(timer));
  pthread_t *killer = (pthread_t *)malloc(sizeof(pthread_t));
  // t->Period = 5;
  // k = queueInit();
  // return 0;
  void *pointer;
  workFunction *work1 = (workFunction *)malloc(sizeof(workFunction));
  // workFunction *work2 = (workFunction *)malloc(sizeof(workFunction));
  // workFunction *work3 = (workFunction *)malloc(sizeof(workFunction));
  work1->work = functions_array[0];
  pointer = &random_arguments[0];
  //work1->arg = pointer;
  queue *fifo;

  if(con == NULL){
    fprintf (stderr, "Unable to allocate consumer.\n");
    exit (1);
  }

  fifo = queueInit();

  if (fifo ==  NULL){
    fprintf(stderr, "Main: Queue Init failed.\n");
    exit (1);
  }
  timerInit(t1,1,10,0,work1,&random_arguments[0],fifo);
  // work2->work = functions_array[1];
  // pointer = &random_arguments[1];
  //work2->arg = pointer;
  // timerInit(t2,2,10,0,work2,&random_arguments[1],fifo);
  // work3->work = functions_array[2];
  // pointer = &random_arguments[2];
  // timerInit(t3,1,10,0,work3,&random_arguments[2],fifo);
  // //start(t);
  // // work.work = functions_array[1];
  // // pointer = &random_arguments[1];
  // // work.arg = pointer;
  // // timerInit(&t[1],3,6,0,work,fifo);
  startat(t1,2020,7,10,12,39,0);
  // startat(t2,2020,7,10,12,39,0);
  // startat(t3,2020,7,10,12,39,0);
  // for(int i =0; i<p;i++){
  //   pthread_create(&pro[i], NULL, producer,(void *)t);
  // }
  //
  // //create c consumer threads

  for(int j=0;j<c;j++){
    pthread_create(&con[j], NULL, consumer,(void *)fifo);
  }
  pthread_create(killer,NULL,killing,(void *)fifo);
  // //wait for all the producers/consumers to finish their work
  // for(int i=0;i<p;i++){
  //   pthread_join (pro[i], NULL);
  // }
  //
  //for(int j=0;j<c;j++){
  pthread_join(*killer, NULL);
  //}
  // free(t1);
  // free(t2);
  // free(t3);
  free(work1);
  // free(work2);
  // free(work3);
  free(con);
  free(killer);
  queueDelete(fifo);
  printf("THE END...\n");
  return 0;
}
