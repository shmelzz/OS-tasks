#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int got = 1;

void wait() {
  got = 1;
}

int main(void) {
  (void)signal(SIGUSR1, wait);

  printf("sender PID: %d\n", getpid());
  int r_pid; 
  printf("enter receiver\'s PID: ");
  scanf("%d", &r_pid);
  int number;
  printf("enter number: ");
  scanf("%d", &number);

  for (int i = 0; i < 32; ++i) {
   if ((number & (1 << i)) == 0) {
      kill(r_pid, SIGUSR1);
    } else {
     kill(r_pid, SIGUSR2);
    }
    got = 0;
   while (i != 31 && got == 0) {}
  }


  printf("send: %d\n", number);
  return 0;
}
