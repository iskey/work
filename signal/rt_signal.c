#include <signal.h>
#include <stddef.h>
#include <stdio.h> 
#include <errno.h>
#include <string.h>

int g_usr_count= 0;
int g_rt_count= 0;

void signal_dispatch(int signo)
{
    if(signo== SIGUSR1)
    {
        printf("received signal SIGUSR1, count=%d.\n", ++g_usr_count);
    }
    else if(signo== SIGRTMIN)
    {
        printf("received signal SIGRTMIN, count=%d.\n", ++g_rt_count);
    }
    else
    {
        printf("received signal (%d).\n", signo);
    }
}

int main()
{
    sigset_t base_mask, waiting_mask;

    int i;
    for(i= 0; i< NSIG; i++)
    {
        if(i== SIGKILL || i== SIGSTOP)
            continue;

        if(signal(i, signal_dispatch)== SIG_ERR)
        {
            printf("signal for signo(%d) failed (%s)\n", i, strerror(errno));
        }
    }

    sigset_t set;
    sigset_t oset;

    /*清除set中的信号，把set置空*/
    sigemptyset(&set);
    sigaddset (&set, SIGUSR1);
    sigaddset (&set, SIGRTMIN);

    /* Block user interrupts while doing other processing. */
    if(sigprocmask (SIG_SETMASK, &set, &oset))
    {
        printf("sit process signal mask error.\n");
        return -1;
    }

    sleep(10);
    /* After a while, check to see whether any signals are pending. */
    if(-1== sigpending (&set))
    {
        printf("signal pending failed.\n");
        return -1;
    }

    printf("number of signal is %d.\n", NSIG);
    printf("SIGUSR1 is %d\n", SIGUSR1);
    printf("SIGRTMIN is %d\n", SIGRTMIN);

    if (1== sigismember (&set, SIGRTMIN)) {
    /* User has tried to kill the process. */
        printf("signal SIGRTMIN exist.\n");
    }
    if (1== sigismember (&set, SIGUSR1)) {
    /* User has tried to stop the process. */
        printf("signal SIGUSR1 exist.\n");
    }

    /*unlock of signals.*/
    if(sigprocmask(SIG_SETMASK, &oset, NULL)== -1)
    {
        perror("set oldset failed.\n");
        return -1;
    }

    return 0;
}