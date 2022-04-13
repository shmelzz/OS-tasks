#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <stdlib.h>

int main()
{
  int     *array;
  int     shmid;
  int     new = 1;
  char    pathname[] = "task9-2b.c";
  key_t   key;
  long    i;
  
  int    semid;                              
  struct sembuf myD; 
  struct sembuf myA; 

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  
  if ((semid = semget(key, 1, 0666 | IPC_CREAT|IPC_EXCL) < 0)) {
	  if((semid=semget(key,0,0)<0)){
			printf("Can\'t create or find semaphore set\n");
			exit(-1);
	}
  }else{
	  semctl(semid, 0 , SETVAL, 1);
  }
  
  
  
  myD.sem_num = 0;
  myD.sem_op  = -1;
  myD.sem_flg = 0;
  
  myA.sem_num = 0;
  myA.sem_op  = 1;
  myA.sem_flg = 0;

  if ((shmid = shmget(key, 3*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create shared memory\n");
      exit(-1);
    } else {
      if ((shmid = shmget(key, 3*sizeof(int), 0)) < 0) {
        printf("Can\'t find shared memory\n");
        exit(-1);
      }
      new = 0;
    }
  }

  if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }  

  if (semop(semid, &myD, 1) < 0) {
    printf("Can\'t wait\n");
	exit(-1);
  }
  
  if (new) {
    array[0] =  0;
    array[1] =  1;
    array[2] =  1;
  } else {
    array[1] += 1;
    for(i=0; i<2000000000L; i++);
    for(i=0; i<2000000000L; i++);
    for(i=0; i<2000000000L; i++);
    array[2] += 1;
  }
  
   printf
    ("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
    array[0], array[1], array[2]);
  
  if (semop(semid, &myA, 1) < 0) {
    printf("Can\'t add\n");
    exit(-1);
  }
  
  if (shmdt(array) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  return 0;
}
