#include <stdio.h>
#include <string.h>
#include "md5.h"

int md5_test_file(const char *filename)
{
	FILE *file;
	int len;
	unsigned char buf[1024];
	unsigned char sig_name[1024];

	char *md5_sig;
	unsigned char md5_file[64]={0};
	unsigned char md5_dig[16];

	snprintf(sig_name, 1024, "%s.sig", filename);
	if((file= fopen(sig_name, "rb"))== NULL)
	{
		goto ERR;
	}
	else
	{
		while(len= fread(buf, 1, 1024, file))
		{
			md5_sig= strtok(buf, " ");
		}
	}

	if(0!= MD5_File(filename, md5_dig))
	{
		return -1;
	}

	int i;
	for(i= 0; i< 16; i++)
	{
		snprintf(&md5_file[i*2], 3, "%02x", md5_dig[i]);
	}
	md5_file[i*2]= 0;

	if(0== strcmp(md5_file, md5_sig))
	{
		printf("MD5 check OK.\n");
	}
	else{
		printf("MD5 check failed.\n");
		goto ERR;
	}

	return 0;

ERR:
	return -1;
}

int main(int argc, char* argv[])
{
	unsigned char digest[16];
	int i;
	if(argc< 2)
		printf("Please give a filename.\n");
#if 0
	MD5_File(argv[1], digest);
	printf("FILE-%s MD5= ", argv[1]);
	for(i= 0; i< 16; i++)
		printf("%02x", digest[i]);
	printf("\n");
#endif
	
	md5_test_file(argv[1]);
	//return -1;

	int ret= MD5_File(argv[1], digest);

	if(ret!= 0){
		printf("MD5 generate failed.\n");
		return -1;
	}

	printf("FILE-%s MD5= ", argv[1]);
	for(i= 0; i< 16; i++)
		printf("%02x", digest[i]);
	printf("\n");
}
