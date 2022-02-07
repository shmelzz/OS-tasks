#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
  pid_t pid, ppid;
  int a = 0;

  pid=fork();
  
  if(pid==-1){
	 printf("error occured");
  } else if (pid==0) {
	(void) execle("/bin/cat", "/bin/cat", "task3-4.c", 0, envp);
	a=a+5;	 
  } else {
	a=a+3;
  }
 
  pid  = getpid();
  ppid = getppid();

  printf("My pid = %d, my ppid = %d, result = %d\n", (int)pid, (int)ppid, a);

  return 0;
}
