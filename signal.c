 #include <signal.h>
 #include <stddef.h>
 
int main()
{
    sigset_t base_mask, waiting_mask;

    sigemptyset (&base_mask);
    sigaddset (&base_mask, SIGINT);
    sigaddset (&base_mask, SIGTSTP);

    /* Block user interrupts while doing other processing. */
    //sigprocmask (SIG_SETMASK, &base_mask, NULL);

    /* After a while, check to see whether any signals are pending. */
    //sigpending (&waiting_mask);

    if (sigismember (&waiting_mask, SIGINT)) {
    /* User has tried to kill the process. */
    }
    else if (sigismember (&waiting_mask, SIGTSTP)) {
    /* User has tried to stop the process. */
    }
}