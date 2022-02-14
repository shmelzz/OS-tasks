#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int     fd[2], result;
  int fd1[2];

  size_t size;
  char  resstring[14];
  char  resstring1[15];

  if (pipe(fd) < 0) {
    printf("Can\'t open pipe1\n");
    exit(-1);
  }
  
  if (pipe(fd1) < 0) {
    printf("Can\'t open pipe2\n");
    exit(-1);
  }

  result = fork();

  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (result > 0) {

   /* Parent process */

    if (close(fd[0]) < 0) {
      printf("parent: Can\'t close reading side of pipe1\n"); exit(-1);
    }

    size = write(fd[1], "Hello, child!", 14);

    if (size != 14) {
      printf("Can\'t write all string to pipe1\n");
      exit(-1);
    }

    if (close(fd[1]) < 0) {
      printf("parent: Can\'t close writing side of pipe1\n"); exit(-1);
    }

    printf("Parent exit\n");
    
    // child
    if (close(fd1[0]) < 0) {
      printf("child: Can\'t close reading side of pipe2\n"); exit(-1);
    }

    size = write(fd1[1], "Hello, parent!", 15);

    if (size != 15) {
      printf("Can\'t write all string to pipe2\n");
      exit(-1);
    }

    if (close(fd1[1]) < 0) {
      printf("child: Can\'t close writing side of pipe2\n"); exit(-1);
    }

    printf("child exit\n");

  } else {

    /* Child process */

    if (close(fd[1]) < 0) {
      printf("child: Can\'t close writing side of pipe1\n"); exit(-1);
    }

    size = read(fd[0], resstring, 14);

    if (size < 0) {
      printf("Can\'t read string from pipe1\n");
      exit(-1);
    }

    printf("Child exit, resstring:%s\n", resstring);

    if (close(fd[0]) < 0) {
      printf("child: Can\'t close reading side of pipe1\n"); exit(-1);
    }
    
    // parent
    if (close(fd1[1]) < 0) {
      printf("parent: Can\'t close writing side of pipe2\n"); exit(-1);
    }

    size = read(fd1[0], resstring1, 15);

    if (size < 0) {
      printf("Can\'t read string from pipe2\n");
      exit(-1);
    }

    printf("parent exit, resstring:%s\n", resstring1);

    if (close(fd1[0]) < 0) {
      printf("parent: Can\'t close reading side of pipe2\n"); exit(-1);
    }
  }

  return 0;
}
