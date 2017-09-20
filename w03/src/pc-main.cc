#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include "pc-common.h"
#include "producer.h"
#include "consumer.h"

int fill = 0;
int use = 0;
int buffer[BSZ];

sem_t empty;
sem_t full;
pthread_mutex_t pLock;
pthread_mutex_t cLock;

int main(int argc, char *argv[])
{
 if(argc < 4)
  {
   printf("Needs four arguments num_products num_producer num_consumer.\n");
   return 1;
  }

  int num_product = atoi(argv[1]);
  int np = atoi(argv[2]);
  int nc = atoi(argv[3]);
  sem_init(&empty, 0, BSZ);
  sem_init(&full, 0, 0);
  
  pthread_mutex_init(&cLock, NULL);
  pthread_mutex_init(&pLock, NULL);
  
  // Creating and starting threads
  pthread_t *pGroup = new pthread_t[np];
  pthread_t *cGroup = new pthread_t[nc];

  // Packing thread arguments
  int *pArgs = new int[2*np];
  int *cArgs = new int[2*nc];

  for(int i=0; i<np; ++i)
  {
   pArgs[2*i] = i; //Thread ID
   pArgs[2*i+1] = num_product;
   pthread_create(pGroup+i, 0, producer, (void*) (pArgs+2*i));
  }

  for(int i=0; i<nc; ++i)
  {
   cArgs[2*i] = i;  // Thread ID
   cArgs[2*i+1] = num_product;
   pthread_create(cGroup+i, 0, consumer, (void*) (cArgs+2*i));
  }

  for(int i=0; i<np; ++i)
   pthread_join(pGroup[i], NULL);

  for(int i=0; i<nc; ++i)
   pthread_join(cGroup[i], NULL);

  fprintf(stderr,"All threads joined, shutting down\n");
  delete[] pGroup;
  delete[] cGroup;
  delete[] pArgs;
  delete[] cArgs;
  
  sem_destroy(&full);
  sem_destroy(&empty);

return 0;
}

