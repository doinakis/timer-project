# Timer Project
In addition to the revision of the [prods-cons](https://github.com/doinakis/Real-Time-Embedded-Systems) this is a follow up project. It is intended to be used with a raspberry pi and it is implementing a simple timer that periodically pushes functions to a fifo queue. It is implemented using pthreads, every timer is a producer thread and every consumer is a worker thread that executes the functions that the timers push to the fifo queue. The number of consumers and the length of the fifo queue can be assigned at the **globaldef.h** file. The **timer.h timer.c and main.c** files should have everything you need to understand the code and adjust it to your needs. The [cross-compiler](https://github.com/abhiTronix/raspberry-pi-cross-compilers) used can be found in the above link. Below are shown the results of the drifts, the time that the producer needs to push a function in the queue, the time the consumer needs to grab a function from the queue, and the time that the function stays in the queue. The results were produced from experiments containing three timers that pushes functions periodically with period 10,100,1000 milliseconds each. The metrics show the mean,median,max,min and standart deviation of the drift the time that the producer needs to add a fucntion to the queue, the time the consumer needs to grab one and the time inside the queue.
  
* # 2 Consumers 
## Drift 
|     Period(ms)    |    Mean  (μS)    |     Median   (μS)    |     Max   (μS)    |     Min   (μS)    |     Std (μS)    |
|---------------------|---------------------------|------------------------|-----------------------|------------------------|-------------------------------|
|     10              |     0.00065               |     0                  |     8273              |     -8241              |     97.903                    |
|     100             |     0.00603               |     0                  |     8088              |     -8116              |     117.331                   |
|     1000            |     0.05694               |     0                  |     5753              |     -5767              |     286.291                   |

## Producer add  
|     Period(ms)    |     Mean  (μS)    |     Median   (μS)    |     Max   (μS)    |     Min   (μS)    |     Std (μS)    |
|---------------------|---------------------------|------------------------|-----------------------|------------------------|-------------------------------|
|     10              |     31.967                |     32                 |     1659              |     11                 |     8.341                     |
|     100             |     31.297                |     31                 |     599               |     3                  |     9.952                     |
|     1000            |     30.944                |     31                 |     192               |     3                  |     7.735                     |

## Consumer grab 

|     Period(ms)    |    Mean  (μS)    |     Median   (μS)    |     Max   (μS)    |     Min   (μS)    |     Std (μS)    |
|---------------------|---------------------------|------------------------|-----------------------|------------------------|-------------------------------|
|     10              |     9.136                 |     9                  |     290               |     4                  |     2.951                     |
|     100             |     9.295                 |     9                  |     362               |     3                  |     5.852                     |
|     1000            |     8.868                 |     9                  |     68                |     3                  |     2.133                     |

## Time spent in queue 

|     Period(ms)    |     Mean  (μS)    |     Median   (μS)    |     Max   (μS)    |     Min   (μS)    |     Std (μS)    |
|---------------------|---------------------------|------------------------|-----------------------|------------------------|-------------------------------|
|     10              |     70.977                |     71                 |     8364              |     35               |     17.738                    |
|     100             |     72.598                |     70                 |     706               |     35                  |    23.074                    |
|     1000            |     72.467                |     69                 |     1003               |     47                 |     27.382                    |

* # 3 Consumers
## Drift
|     Period(ms)    |     Mean  (μS)    |     Median   (μS)    |     Max   (μS)    |     Min   (μS)    |     Std (μS)    |
|---------------------|---------------------------|------------------------|-----------------------|------------------------|-------------------------------|
|     10              |     0.000667              |     0                  |     8176              |     -7618              |     110.933                   |
|     100             |     0.006194              |     0                  |     7371              |     -7384              |     99.0751                   |
|     1000            |     0.053889              |     0                  |     357               |     -357               |     39.1856                   |

## Producer add
|     Period(ms)    |     Mean  (μS)    |     Median   (μS)    |     Max   (μS)    |     Min   (μS)    |     Std (μS)    |
|---------------------|---------------------------|------------------------|-----------------------|------------------------|-------------------------------|
|     10              |     32.372                |     32                 |     649               |     11                 |     32.372                    |
|     100             |     31.843                |     32                 |     530               |     12                 |     10.235                    |
|     1000            |     31.624                |     32                 |     309               |     12                 |     9.9068                    |

## Consumer grab 
|     Period(ms)    |     Mean  (μS)    |     Median   (μS)    |     Max   (μS)    |     Min   (μS)    |     Std (μS)    |
|---------------------|---------------------------|------------------------|-----------------------|------------------------|-------------------------------|
|     10              |     8.9421                |     9                  |     274               |     3                  |     3.104                     |
|     100             |     9.2099                |     9                  |     179               |     3                  |     5.313                     |
|     1000            |     8.7092                |     8                  |     167               |     4                  |     5.535                     |
## Time spent in queue 
|     Period(ms)    |     Mean  (μS)    |     Median   (μS)    |     Max   (μS)    |     Min   (μS)    |     Std (μS)    |
|---------------------|---------------------------|------------------------|-----------------------|------------------------|-------------------------------|
|     10              |     71.426                |     71                 |     1393              |    35               |     10.420                    |
|     100             |     73.470                |     70                 |     402               |     35                 |     22.746                    |
|     1000            |     72.992                |     70                 |     350               |     36                 |     23.570                    |

* # 4 Consumers
## Drift

|     Period(ms)    |    Mean  (μS)    |     Median   (μS)    |     Max   (μS)    |     Min   (μS)    |     Std (μS)    |
|---------------------|---------------------------|------------------------|-----------------------|------------------------|-------------------------------|
|     10              |     0.000664              |     0                  |     8415              |     -8409              |     107.615                   |
|     100             |     0.005944              |     0                  |     6922              |     -6916              |     105.364                   |
|     1000            |     0.045833              |     0                  |     367               |     -379               |     41.669                    |
## Producer add 

|     Period(ms)    |     Mean  (μS)    |     Median   (μS)    |     Max   (μS)    |     Min   (μS)    |     Std (μS)    |
|---------------------|---------------------------|------------------------|-----------------------|------------------------|-------------------------------|
|     10              |     32.795                |     33                 |     571               |     11                 |     8.123                     |
|     100             |     32.400                |     32                 |     422               |     11                 |     10.013                    |
|     1000            |     31.932                |     32                 |     206               |     13                 |     8.6300                    |
## Consumer grab 
|     Period(ms)    |     Mean  (μS)    |     Median   (μS)    |     Max   (μS)    |     Min   (μS)    |     Std (μS)    |
|---------------------|---------------------------|------------------------|-----------------------|------------------------|-------------------------------|
|     10              |     9.305                 |     9                  |     278               |     4                  |     3.112                     |
|     100             |     9.137                 |     9                  |     250               |     4                  |     5.007                     |
|     1000            |     9.367                 |     9                  |     163               |     5                  |     5.173                     |
## Time spent in queue 
|     Period(ms)    |     Mean  (μS)    |     Median   (μS)    |     Max   (μS)    |     Min   (μS)    |     Std (μS)    |
|---------------------|---------------------------|------------------------|-----------------------|------------------------|-------------------------------|
|     10              |     72.858                |     72                 |     8141               |     35                 |     16.937                     |
|     100             |     75.047                |     72                 |     460               |     36                 |     22.830                    |
|     1000            |     74.353                |     72                 |     309               |     37                 |     23.195                    |
