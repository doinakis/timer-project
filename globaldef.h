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

/*
    Define the queuesize and the number of consumers 
*/
#define QUEUESIZE 10
#define   c   2

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



#endif
