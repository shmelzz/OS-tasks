
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>

// s = 0
// цикл:
//    PARENT: 
//  write
//  A(s,2)
//  Z(s)
//  read

//    CHILD:
//  D(s,1)
//  read
//  write
//  D(s,1)

int main()
{
  int fd[2], result;
  size_t size;
  char  message[16];

  char pathname[] = "task9-3.c";
  key_t key;
  struct sembuf mybuf;
  int semid;

  int N;
  scanf("%d", &N);

  if (pipe(fd) < 0) {
    printf("can\'t open pipe\n");
    exit(-1);
  }

  if ((key = ftok(pathname,0)) < 0) {
    printf("can\'t generate key\n");
    exit(-1);
  }

  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    printf("can\'t get semaphore set\n");
    exit(-1);
  }

  result = fork();
  if (result < 0) {
    printf("can\'t fork child\n");
    exit(-1);
  }  

  for (int i = 0; i < N; ++i) {
    if (result > 0) {
      // write
      size = write(fd[1], "Hey from parent!", 16);
      if (size != 16) {
        printf("can\'t write to pipe\n");
        exit(-1);
      }
      // A(S,2)
      mybuf.sem_num = 0;
      mybuf.sem_op = 2;
      mybuf.sem_flg = 0;
      if (semop(semid, &mybuf, 1) < 0) {
        printf("can\'t wait for condition\n");
        exit(-1);
      }
      // Z(S)
      mybuf.sem_num = 0;
      mybuf.sem_op = 0;
      mybuf.sem_flg = 0;
      if (semop(semid, &mybuf, 1) < 0) {
        printf("can\'t wait for condition\n");
        exit(-1);
      }
      // read
      size = read(fd[0], message, 16);
      if (size != 16) {
        printf("can\'t read string from pipe\n");
        exit(-1);
      }
      printf("read: %s\n", message);
    } else {
      // D(S,1)
      mybuf.sem_num = 0;
      mybuf.sem_op = -1;
      mybuf.sem_flg = 0;
      if (semop(semid, &mybuf, 1) < 0) {
        printf("can\'t wait for condition\n");
        exit(-1);
      }
      // read
      size = read(fd[0], message, 16);
      if (size != 16) {
        printf("can\'t read from pipe\n");
        exit(-1);
      }
      printf("read: %s\n", message);
      // write
      size = write(fd[1], "Hey from child!", 16);
      if (size != 16) {
        printf("can\'t write all string to pipe\n");
        exit(-1);
      }
      // D(S,1)
      mybuf.sem_num = 0;
      mybuf.sem_op = -1;
      mybuf.sem_flg = 0;
      if (semop(semid, &mybuf, 1) < 0) {
        printf("can\'t wait for condition\n");
        exit(-1);
      }
    }
  }

  if (close(fd[0]) < 0) {
    printf("can\'t close reading side of pipe\n"); 
    exit(-1);
  }

  if (close(fd[1]) < 0) {
    printf("can\'t close writing side of pipe\n"); 
    exit(-1);
  }

  return 0;
}
