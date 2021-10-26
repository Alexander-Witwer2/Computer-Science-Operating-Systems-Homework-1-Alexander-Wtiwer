#include "external.h"

sem_t empty,full;
int in = 0;
int out = 0;
int buffer[BUFFSIZ];
pthread_mutex_t mutex;

void *producer(void *num){
   int item;
   for(int i=0;i<MAXIMUM; ++i){
      item = rand()%2+1;
      sem_wait(&empty);
      pthread_mutex_lock(&mutex);
      buffer[in] = item;
      printf("Producer starts producing item %d:\n",num,buffer[in],in);
      in = (in+1);
      pthread_mutex_unlock(&mutex);
      sem_post(&full);
   }

}

int main(){
   //Gotta Make that shared Memory Space
   int myShm = shm_open("Memory", O_CREAT, 0611);

   ftruncate(myShm,sizeof(int));

   int* table = mmap(0,sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, myShm, 0);

   //initialzing the data
   pthread_mutex_init(&mutex,NULL);
   sem_init(&empty,0,BUFFSIZ);
   sem_init(&full,0,0);
   pthread_t pt[MAXIMUM];

   for(int i = 0; i < MAXIMUM; ++i){
      printf("Producer created thread...\n");
      pthread_create(&pt[i], NULL, &producer, table);
   }

   for(int i = 0; i < MAXIMUM; ++i){
      pthread_join(pt[i],NULL);
   }

   //destroy the semaphores and the mutex
   sem_destroy(&empty);
   sem_destroy(&full);
   pthread_mutex_destroy(&mutex);

   //unmap the shared data and unlink the shared memory
   munmap(table,sizeof(int));
   close(myShm);
   shm_unlink("Memory");
   return 0;
}
