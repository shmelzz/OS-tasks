#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int s_pid;
int num = 0b11111111111111111111111111111111;
int bit = 0;

void zero() {
  num = num ^ (1 << bit);
  ++bit;
  if (bit == 32) {
    printf("got number: %d\n", num);
    exit(0);
  } else {
    kill(s_pid, SIGUSR1);
  }
}

void one() {
  ++bit;
  if (bit == 32) {
    printf("got number %d\n", num);
    exit(0);
  } else {
    kill(s_pid, SIGUSR1);
  }
}

int main(void) {
  (void)signal(SIGUSR1, zero);
  (void)signal(SIGUSR2, one);

  printf("my PID: %d\n", getpid());
  printf("enter sender: ");
  scanf("%d", &s_pid);

  while(1) {};
}
