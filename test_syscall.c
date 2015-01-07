#include "unistd.h"
#include "stdio.h"

int main()
{
	int t;
	t= syscall(367,321);
	printf("syscall is %d\n",t);
	return 0;
}
