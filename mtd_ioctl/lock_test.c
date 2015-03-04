#include "stdio.h"
int main(int argc, char *argv[])
{
	long count= 0;
	while(1)
	{
		printf("%d\n", count++);
		usleep(100000);
	}
}
