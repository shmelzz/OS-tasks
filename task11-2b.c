#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void)
{
  int msqid;
  char pathname[]="task11-2a.c";
  key_t  key;
  int len, maxlen;

  struct mymsgbuf
  {
    long mtype;
    struct{
		int sinfo;
	} info;
  } mybuf;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }
  
  // receive
  for (int i = 1; i <= 2; i++) {
    maxlen = 81;
    if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 1, 0)) < 0) {
      printf("Can\'t recieve message from queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
    if (mybuf.mtype == LAST_MESSAGE) {
      break;
    }
    printf("message type = %ld, hello from program %d, \n", mybuf.mtype ,mybuf.info.sinfo);
  }
  
  if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 0, 0)) < 0) {
      printf("Can\'t recieve message from queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
  
  // send
  for (int i = 1; i <= 2; i++) {

    mybuf.mtype = 2;
    mybuf.info.sinfo = 2;
    len = sizeof(mybuf.info);

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
  }

  mybuf.mtype = LAST_MESSAGE;
  len         = 0;
  
  if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }

  return 0;
}
