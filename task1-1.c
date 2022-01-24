#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	printf("user id: %d\n", getuid());
	printf("group id: %d\n",getgid());
	return 0;
}

