 #include <signal.h>
 #include <stddef.h>
 #include <stdio.h> 

int g_usr_count= 0;
int g_rt_count= 0;

void signal_dispatch(int signo)
{
    if(signo== SIGUSR1)
    {
        printf("received signal SIGUSR1.\n");
        g_usr_count++;
    }
    else if(signo== SIGRTMIN)
    {
        printf("received signal SIGRTMIN.\n");
        g_rt_count++;
    }
}

int main()
{
    sigset_t base_mask, waiting_mask;

    if(signal(SIGUSR1, signal_dispatch)== SIG_ERR)
    {
        printf("register SIGUSR1 signal failed.\n");
        return -1;
    }

    if(signal(SIGRTMIN, signal_dispatch)== SIG_ERR)
    {
        printf("register SIGRTMIN signal failed.\n");
        return -1;
    }

    sigset_t set;
    sigset_t oset;

    /*清除set中的信号，把set置空*/
    sigemptyset(&set);
    sigaddset (&set, SIGTSTP);
    sigaddset (&set, SIGRTMIN);

    /* Block user interrupts while doing other processing. */
    if(sigprocmask (SIG_SETMASK, &set, NULL))
    {
        printf("sit process signal mask error.\n");
        return -1;
    }

    sleep(10);
    /* After a while, check to see whether any signals are pending. */
    if(-1== sigpending (&waiting_mask))
    {
        printf("signal pending failed.\n");
        return -1;
    }

    if (sigismember (&waiting_mask, SIGINT)) {
    /* User has tried to kill the process. */
        printf("received a SIGINT signal.\n");
    }
    if (sigismember (&waiting_mask, SIGTSTP)) {
    /* User has tried to stop the process. */
        printf("received a SIGTSTP signal.\n");
    }
}