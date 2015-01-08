#include <stdio.h>

unsigned char test[16] =
{0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
unsigned char len = 16;
void main( void )
{
	unsigned long temp = 0;
	unsigned int crc;
	unsigned char i;
	unsigned char *ptr = test;
	while( len-- )
	{
		for(i = 0x80; i != 0; i = i >> 1)
		{
			temp = temp * 2;
			printf("len: %d:0x%02x ", len, i);
			printf("ptr:0x%02x  temp:0x%05x  ", *ptr, temp);
			if((temp & 0x10000) != 0)
			{
				temp = temp ^ 0x11021;
				printf("s1:0x%05x   ",temp);
			}
			else{
				printf("s1:0x%05x   ",0xCCCCC);
			}	
			if((*ptr & i) != 0)
			{
				temp = temp ^ (0x10000 ^ 0x11021);
				printf("s2:0x%05x   ",temp);
			}
			else{
				printf("s2:0x%05x   ",0xCCCCC);
			}

			printf("s3:0x%05x\n",temp);
		}
		ptr++;
	}
	crc = temp;
	printf("0x%x \n",crc);
}
