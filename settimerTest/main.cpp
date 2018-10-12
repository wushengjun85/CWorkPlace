
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

void signalHandler(int signo)
{
    switch (signo){
        case SIGALRM:
            printf("Caught the SIGALRM signal!\n");
            break;
   }
}

int main(int argc, char *argv[])
{
    signal(SIGALRM, signalHandler);

    struct itimerval new_value, old_value;

    //假如it_value为0是不会触发信号的，所以要能触发信号，it_value得大于0；如果it_interval为零，只会延时，不会定时（也就是说只会触发一次信号)。
    //old_value参数，通常用不上，设置为NULL，它是用来存储上一次setitimer调用时设置的new_value值。
    new_value.it_value.tv_sec = 0;
    new_value.it_value.tv_usec = 1;//时间不能大于等于1秒
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 100000;//时间不能大于等于1秒
    int ret = setitimer(ITIMER_REAL, &new_value, &old_value);/*NULL*/
    printf("ret = %d\n",ret);

    for(;;);

    return 0;
}
