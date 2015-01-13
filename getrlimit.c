#include "stdio.h"
#include "sys/resource.h"
#include "sys/time.h"

int main()
{
	struct rlimit sig_pend_limit;

	getrlimit(RLIMIT_SIGPENDING, &sig_pend_limit);

	printf("Current limit is %d: Max limit is %d\n", sig_pend_limit.rlim_cur, sig_pend_limit.rlim_max);

	sig_pend_limit.rlim_cur=70;
	sig_pend_limit.rlim_max=70;
	setrlimit(RLIMIT_SIGPENDING, &sig_pend_limit);


	getrlimit(RLIMIT_SIGPENDING, &sig_pend_limit);
	printf("Current limit is %d: Max limit is %d\n", sig_pend_limit.rlim_cur, sig_pend_limit.rlim_max);

	return 0;
}
