//Alexander Witwer Operating System Homework 1

#include "external.h"
#include <err.h>
#include <errno.h>


sem_t empty,full;
int in = 0;
int out = 0;
int buffer[BUFFSIZ];
int item;
pthread_mutex_t mutex;

void* consumer(void *num){
   for(int i = 0; i < MAXIMUM; i++){
   sleep(2);

   pthread_mutex_lock(&mutex);
   item = buffer[out];

   printf("Item %d has been consumed by the Consumer\n",num,item,out);
   out = (out+1)%BUFFSIZ;
   pthread_mutex_unlock(&mutex);
   sem_post(&empty);
   }


}

int main(){

   sleep(1);

   int myShm = shm_open("Memory", O_EXCL | O_RDWR, 0611);

   int *table = mmap(0,sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED, myShm,0);
   pthread_t pt[MAXIMUM];

   for(int i = 0; i < MAXIMUM; i++){
   printf("Consumer thread created\n");
   pthread_create(&pt[i],NULL, &consumer, table);
  }

   for(int i = 0; i < MAXIMUM; i++){
printf("HELP\n");
     if( pthread_join(pt[i],NULL)!=0){perror("ERROR!!!!!\n");}
	printf("HELP\n");
   }

   sem_destroy(&empty);
   sem_destroy(&full);
   pthread_mutex_destroy(&mutex);

   munmap(table,sizeof(int));
   close(myShm);
   shm_unlink("Memory");
   printf("Done\n");
   return 0;
}
