#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
  int     fd[2];
  int size_pipe=0;
  char *c;

  char  resstring[14];

  if (pipe(fd) < 0) {
    printf("Can\'t open pipe\n");
    exit(-1);
  }
  
  fcntl(fd[1], F_SETFL,O_NONBLOCK);

	int size=1;
	while(size==1){
	   size=write(fd[1], "p", 1);
	   if(size!=1){
		    break;
		   }
	   size_pipe+=1;
	}
	printf("размер: %d байт\n", size_pipe);
    return 0;
}
