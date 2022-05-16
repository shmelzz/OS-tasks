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
  char pathname[]="task12-client.c";
  key_t  key;
  int len, maxlen;

  struct mymsgbufReq 
  {
    long mtype;
    struct{
		pid_t client;
		double client_num;
	} client_request;
  } mybufReq;
  
  struct mymsgbufRes 
  {
    long mtype;
    struct{
		double server_num;
	} server_response;
  } mybufRes;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }
  
  while(1){
	maxlen = sizeof(mybufReq.client_request);
	if (( len = msgrcv(msqid, (struct msgbuf *) &mybufReq, maxlen, 1, 0)) < 0) {
      printf("Can\'t recieve message from queue\n");
      exit(-1);
    }
    
    mybufRes.mtype = mybufReq.client_request.client;
	mybufRes.server_response.server_num = mybufReq.client_request.client_num*mybufReq.client_request.client_num;
    len = sizeof(mybufRes.server_response);
    
    if (msgsnd(msqid, (struct msgbuf *) &mybufRes, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
  }
  

  return 0;
}
