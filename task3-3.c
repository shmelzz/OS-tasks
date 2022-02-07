#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
	printf("argv: \n");
	for(int i=0; i<argc;++i){
		printf("%s ",argv[i]);
	}
	printf("\nenvp: \n");
	int i=0;
	while (envp[i]!=NULL){
		printf("%s\n",envp[i]);
		++i;
	}
}
