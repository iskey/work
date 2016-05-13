#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
  
#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
  __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)
 
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)
 
int main()
{
	int fd;
	void *map_base, *virt_addr;
	off_t target= 0xf4100052;
	unsigned long read_result, writeval;
	
	if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) FATAL;
    
	printf("/dev/mem opened.\n"); 
	
    fflush(stdout);
	
	map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target & ~MAP_MASK);
    if(map_base == (void *) -1) FATAL;
    printf("Memory mapped at address %p.\n", map_base); 
    fflush(stdout);
	
	virt_addr = map_base + (target & MAP_MASK);
	
	read_result = *((unsigned char *) virt_addr);

	printf("Value at address 0x%X (%p): 0x%X\n", target, virt_addr, read_result); 
    fflush(stdout);
	
	
	if(munmap(map_base, MAP_SIZE) == -1) FATAL;
    close(fd);
}