#ifndef PC_COMMON_H
#define PC_COMMON_H


#include <pthread.h>
#include <cstdio>
#include <cstdlib>

extern int fill;
extern int use;

#define BSZ 5

extern int buffer[BSZ];
#include <semaphore.h>

extern sem_t empty;
extern sem_t full;

extern pthread_mutex_t pLock;
extern pthread_mutex_t cLock;
#endif

