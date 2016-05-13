#include "stdio.h"
#include "string.h"

void printArray(void)
{
	int Array[4];
	int i;
	for(i= 0; i<= 4; i++)
	{
		Array[i-1]-= 4;
		printf("Array i:%d value:%d memeory address:0x%08x\n", i-1, Array[i-1], &Array[i-1]);
	}

//	printf("Array")
}


int main(int argc, char *argv[])
{
	int t;
	t= 0;
	printArray();
	printf("program address: 0x%08x\n", printArray);
	return 0;
}