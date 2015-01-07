#include "stdio.h"
#include "sys/resource.h"
#include "time.h"
#include "string.h"
#include "signal.h"

void callback(union sigval si)
{
    const char* data= si.sival_ptr;
    printf("Insided callback data is %s\n", data);
}

int main()
{
	timer_t timer_id;

	struct sigevent ev;

	memset(&ev, 0, sizeof(ev));

	ev.sigev_notify                         = SIGEV_THREAD;
	ev.sigev_value.sival_ptr                = "Welcome";
	ev.sigev_notify_function                = callback;
	ev._sigev_un._sigev_thread._attribute   = NULL;

	if(timer_create(CLOCK_REALTIME, &ev, &timer_id)!= 0)
	{
	    perror("Error creating timer.\n");
	    return -1;
	}

	struct itimerspec ts={{1,0},{2,0}};

	if(timer_settime(timer_id, 0, &ts, NULL)!= 0)
	{
	    perror("Error setting up timer.\n");
	    return -1;
	}

	printf("Press <enter> to continue ...\n");
	getchar();

	timer_delete(timer_id);
	return 0;
}
