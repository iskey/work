#include "stdio.h"

#define POLYNOMIAL 0xB0  /* 11011 followed by 0's */
//#define POLYNOMIAL 0x1B
typedef  unsigned char uint8_t;

/*
 * The width of the CRC calculation and result.
 * Modify the typedef for a 16 or 32-bit CRC standard.
 */
typedef uint8_t crc;

#define WIDTH  (8 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))

crc
crcSlow(uint8_t const message[], int nBytes)
{
    crc  remainder = 0;	


    /*
     * Perform modulo-2 division, a byte at a time.
     */
    for (int byte = 0; byte < nBytes; ++byte)
    {
        /*
         * Bring the next byte into the remainder.
         */
        remainder ^= (message[byte] << (WIDTH - 8));

        /*
         * Perform modulo-2 division, a bit at a time.
         */
        for (uint8_t bit = 8; bit > 0; --bit)
        {
            /*
             * Try to divide the current data bit.
             */
            if (remainder & TOPBIT)
            {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
    }

    /*
     * The final remainder is the CRC result.
     */
    return (remainder>>4);

}   /* crcSlow() */

int main()
{
	uint8_t message= 0x45;
	uint8_t crc= crcSlow(&message, 1);
	printf("crc is 0x%02x\n",crc);
}