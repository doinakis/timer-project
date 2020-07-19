#include "globaldef.h"

// Include costum libraries
#include "timer.h"
#include "queue.h"
#include "prod-cons.h"

int main(void){

  /*
    Initialization of the global variables
  */
  globalInit();

  /*
    Initialization of the fifo queue where the producer add the functions and the
      consumers grabs them
  */
  queue *fifo;
  fifo = queueInit();
  if (fifo ==  NULL){
    fprintf(stderr, "Main: Queue Init failed.\n");
    exit(1);
  }

  /*
    Timer structure and funciton pointers allocations
  */
  timer *t1 = (timer *)malloc(sizeof(timer));
  timer *t2 = (timer *)malloc(sizeof(timer));
  timer *t3 = (timer *)malloc(sizeof(timer));

  /*
    Timer initialization and starting point
  */
  timerInit(t1,1000,10,0,functions_array[0],&random_arguments[0],fifo);
  timerInit(t2,100,100,0,functions_array[1],&random_arguments[1],fifo);
  timerInit(t3,10,1000,0,functions_array[2],&random_arguments[2],fifo);
  start(t1);
  start(t2);
  start(t3);

  // startat(t1,2020,7,16,19,47,0);
  // startat(t2,2020,7,16,19,47,0);
  // startat(t3,2020,7,16,19,47,0);

  /*
    Function that waits all the consumers to finish their work and makes sure that
    all the timers are also done adding their functions in the fifo
  */
  killing();

  /*
    This part is for terminating all the consumer threads properly
  */
  fifo->empty = 0;
  for(int j=0;j<c;j++){
    // Make sure that the fifo->mut is not locked by any other consumer 
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

  /*
    Free all the allocated space and destroy the mutexes and conditions 
  */
  pthread_mutex_destroy(all_done);
  free(all_done);
  pthread_cond_destroy(done);
  free(done);
  free(con);
  queueDelete(fifo);
  printf("THE END...\n");

  return 0;
  
}
