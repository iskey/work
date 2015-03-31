#include <stdio.h>
#include "md5.h"

int main(int argc, char* argv[])
{
	unsigned char digest[16];
	if(argc< 2)
		printf("Please give a filename.\n");

	int ret= MD5_File(argv[1], digest);

	if(ret!= 0){
		printf("MD5 generate failed.\n");
		return -1;
	}

	printf("FILE-%s MD5= ", argv[1]);
	int i;
	for(i= 0; i< 16; i++)
		printf("%02x", digest[i]);
	printf("\n");
}
