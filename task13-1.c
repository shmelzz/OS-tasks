#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
  FILE* file;
  size_t  i = 0;
  char prev [50];
  char cur [50];

  if ((file = fopen("a", "w+")) < 0) {
    printf("Can\'t open file\n");
    exit(-1);
	}
	  
  while(file !=NULL ){
	snprintf(cur, 50, "a%d", i);
	symlink(prev,cur);
   
    if ((file = fopen(cur, "w+")) == NULL) {
    printf("recursion count  = %d\n", i);
    exit(-1);
	}
	
	snprintf(prev, 50, "a%d", i);
	++i;
  }	
  
  
 
  return 0;
}
