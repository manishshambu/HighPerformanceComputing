#include "pc-common.h"
#include "consumer.h"


static int get()
{
 pthread_mutex_lock(&cLock);
 sem_wait(&full);
 int tmp = buffer[use];
 use = (use + 1) % BSZ;
 sem_post(&empty);
 pthread_mutex_unlock(&pLock);
 return tmp;
}


void* consumer(void *args)
{
 int rank = ((int*) args)[0];
 int nprod = ((int*) args)[1];
 fprintf(stderr, "Starting consumer thread id=%d, num_product=%d\n", rank, nprod);
 int i;

 for(i=0; i<nprod; i++)
 {
  int b= get();
  printf("%0.5d\n",b);
 } 
}



