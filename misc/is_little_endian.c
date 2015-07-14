#include "stdio.h"

int is_little_endian()
{
	union _T_BIG_ENDIAN_
	{
		int a;
		char b;
	}probe;

	probe.a= 1;
	return (probe.b== 1);
}

int main()
{
	if(is_little_endian())
	{
		printf("CPU is work in little endian mode.\n");
	}
	else{
		printf("CPU is work in big endian mode.\n");
	}
}
