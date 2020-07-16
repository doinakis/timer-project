#ifndef PRODCONS_H
#define PRODCONS_H

#include "globaldef.h"
#include "queue.h"
#include "timer.h"



//functions that simulate jobs added by producers
void* function_print_1(void* x);
void* function_print_2(void* x);
void* function_sin(void* x);
void* function_cos(void* x);
void* function_acos(void* x);
void* function_asin(void* x);
void* function_tan(void* x);
void* function_atan(void* x);
static void* (*functions_array[8])(void *) = {&function_print_1,&function_print_2,
                                              &function_sin,&function_cos,
                                              &function_asin,&function_acos,
                                              &function_tan,&function_atan};
static int random_arguments[10] = {1,2,3,4,5,6,7,8,9,10};

void *producer(void *q);
void *consumer(void *q);

#endif
