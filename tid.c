#include "stdio.h"
#include "sys/syscall.h"
#include "sys/types.h"
#include "pthread.h"

pid_t gettid()
{
	return syscall(SYS_gettid);
}

int main()
{
	printf("gettid = %d\n", gettid());
	printf("pthread_self = %ld\n", pthread_self());

	return 0;
}
