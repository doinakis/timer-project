#include "globaldef.h"

void globalInit(void){

    /*
        Global variable/mutex/cond initialization 
    */
    kill_flag = 0;
    global_done = 0;
    flag =0;
    producers = 0;
    all_done = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if(all_done == NULL){
        fprintf (stderr, "globaldef.c: Unable to allocate all done mutex.\n");
        exit(1);
    }
    pthread_mutex_init (all_done, NULL);
    done = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    if(done == NULL){
        fprintf (stderr, "globaldef.c: Unable to allocate done condition.\n");
        exit(1);
    }
    pthread_cond_init(done, NULL);

}

void *killing(void){

    pthread_mutex_lock(all_done);
    while(!flag){
        pthread_cond_wait(done,all_done);
    }
    kill_flag = 1;
    pthread_mutex_unlock(all_done);

    return(NULL);

}