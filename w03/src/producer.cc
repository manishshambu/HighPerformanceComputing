#include "pc-common.h"
#include "producer.h"


static int put(int value)
{
 pthread_mutex_lock(&pLock);
 sem_wait(&empty);
 buffer[fill] = value;
 fill = (fill+1) % BSZ;
 sem_post(&full);
 pthread_mutex_unlock(&cLock);
}


void* producer(void *args)
{
 int rank = ((int*) args)[0];
 int nprod = ((int*) args)[1];
 fprintf(stderr,"Starting producer thread id=%d, num_product=%d\n",rank,nprod);
 int i;
 for(i=0; i<nprod; i++)
 {
  put(rank*nprod+i);
 }
}


