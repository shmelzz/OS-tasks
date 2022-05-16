#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
  int msqid;      
  char pathname[]="task12-client.c"; 
                             
  key_t  key;     
  int len;      

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
  
  double number;
  scanf("%lf", &number);


    mybufReq.mtype = 1;
    mybufReq.client_request.client=getpid();
    mybufReq.client_request.client_num = number;
    len = sizeof(mybufReq.client_request);

    if (msgsnd(msqid, (struct msgbuf *) &mybufReq, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

    int maxlen = sizeof(mybufRes.server_response);
    if (( len = msgrcv(msqid, (struct msgbuf *) &mybufRes, maxlen, getpid(), 0)) < 0) {
      printf("Can\'t recieve message from queue\n");
      exit(-1);
    }
    
    printf("client: request = %lf, response = %lf\n", mybufReq.client_request.client_num, mybufRes.server_response.server_num);
  

  return 0;
}
