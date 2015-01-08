#include "stdio.h"

#define POLYNOMIAL 0xD8  /* 11011 followed by 0's */
#define uint8_t unsigned char

uint8_t
crcNaive(uint8_t const message)
{
    uint8_t  remainder;	


    /*
     * Initially, the dividend is the remainder.
     */
    remainder = message;

    /*
     * For each bit position in the message....
     */
    for (uint8_t bit = 8; bit > 0; --bit)
    {
        /*
         * If the uppermost bit is a 1...
         */
        if (remainder & 0x80)
        {
            /*
             * XOR the previous remainder with the divisor.
             */
            remainder ^= POLYNOMIAL;
        }

        /*
         * Shift the next bit of the message into the remainder.
         */
        remainder = (remainder << 1);
    }

    /*
     * Return only the relevant bits of the remainder as CRC.
     */
    return (remainder >> 4);

}   /* crcNaive() */

int main()
{
	uint8_t crc= crcNaive(0x45);
	printf("crc is 0x%02x\n",crc);
}