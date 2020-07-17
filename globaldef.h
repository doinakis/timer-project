#ifndef GLOBALDEF
#define GLOBALDEF

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <limits.h>

/*
    Define the queuesize and the number of consumers 
*/
#define QUEUESIZE 10
#define c 2

/*
    con: consumer threads
    producers: the number of timers initialized during execution
    kill_flag: flag that causes the consumers to terminate
    global_done: the number of timers(producers) that finished their execution
    flag: flag that indicates all the producers are done executing
    all_done: mutex variable to change the all global done variable 
    done: signal that idnicates all the producers are done
*/

pthread_t *con;
int producers;
int kill_flag;
int global_done; 
bool flag;
pthread_mutex_t *all_done;
pthread_cond_t *done;

/*
    Initializes all the gloal variables to their default values
*/
void globalInit(void);
/*
  Killing: its a function that takes as an argument a queue and waits until the
  flag bool is true. If all the consumers are done and the dead flag is true then
  the kill flag is set to 1 and all the consumers can now terminate. It waits a
  done signal that is sent by the last active timer.
*/
void *killing(void);
#endif
