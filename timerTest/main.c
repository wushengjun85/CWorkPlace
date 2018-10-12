#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include <signal.h>
#include <time.h>



#if 0
void sig_handler(int signo)
{
    switch(signo) {
    case SIGUSR1:
        printf("receive sigusr1! \n");
        break;
    case SIGALRM:
        printf("receive sigarlm!\n");
        break;
    }
}

int main()
{
    /**
     struct sigaction
     {
     void (*sa_handler)(int);信号响应函数地址
     void (*sa_sigaction)(int, siginfo_t *, void *);   但sa_flags为SA——SIGINFO时才使用
     sigset_t sa_mask;         说明一个信号集在调用捕捉函数之前，会加入进程的屏蔽中，当捕捉函数返回时，还原
     int sa_flags;
     void (*sa_restorer)(void);未用
     };
    */
    timer_t timer1, timer2;
    struct sigevent evp1, evp2;
    struct sigaction act;

    //for timer1
    memset(&act, 0, sizeof(act));
    act.sa_handler = sig_handler;
    act.sa_flags = 0;

    sigemptyset(&act.sa_mask);

    if (sigaction(SIGUSR1, &act, NULL) == -1) {
        perror("fail to sigaction");
        exit(-1);
    }

    //for timer2
    memset(&act, 0, sizeof(act));
    act.sa_handler = sig_handler;
    act.sa_flags = 0;

    sigemptyset(&act.sa_mask);

    if (sigaction(SIGALRM, &act, NULL) == -1) {
        perror("fail to sigaction");
        exit(-1);
    }
    //for timer1
    memset(&evp1, 0, sizeof(struct sigevent));
    evp1.sigev_signo = SIGUSR1;
    evp1.sigev_notify = SIGEV_SIGNAL;
    if (timer_create(CLOCK_REALTIME, &evp1, &timer1) == -1) {
        perror("fail to timer_create");
        exit(-1);
    }

    struct itimerspec it;
    it.it_interval.tv_sec = 2;
    it.it_interval.tv_nsec = 0;
    it.it_value.tv_sec = 1;
    it.it_value.tv_nsec = 0;
    if (timer_settime(timer1, 0, &it, 0) == -1) {
        perror("fail to timer_settime");
        exit(-1);
    }

    //for timer2
    memset(&evp2, 0, sizeof(struct sigevent));
    evp2.sigev_signo = SIGALRM;
    evp2.sigev_notify = SIGEV_SIGNAL;
    if (timer_create(CLOCK_REALTIME, &evp2, &timer2) == -1) {
        perror("fail to timer_create");
        exit(-1);
    }
    it.it_interval.tv_sec = 4;
    it.it_interval.tv_nsec = 0;
    it.it_value.tv_sec = 2;
    it.it_value.tv_nsec = 0;
    if (timer_settime(timer2, 0, &it, 0) == -1) {
         perror("fail to timer_settime");
         exit(-1);
    }

    for(;;);

    return 0;
}
#endif



void handle(union sigval v)
{
    time_t t;
    char p[32];

    time(&t);
    strftime(p, sizeof(p), "%T", localtime(&t));

    printf("%s thread %lu, val = %d, signal captured.\n", p, pthread_self(), v.sival_int);
    return;
}



int main()
{
    struct sigevent evp;
    struct itimerspec ts;
    timer_t timer;
    int ret;

    memset (&evp, 0, sizeof (evp));
    evp.sigev_value.sival_ptr = &timer;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = handle;
    evp.sigev_value.sival_int = 6; //作为handle()的参数

    ret = timer_create(CLOCK_REALTIME, &evp, &timer);
    if( ret)
        perror("timer_create");

    ts.it_value.tv_sec = 3;
    ts.it_value.tv_nsec = 0;
    ts.it_interval.tv_sec = 2;
    ts.it_interval.tv_nsec = 0;

    ret = timer_settime(timer, TIMER_ABSTIME, &ts, NULL);
    if( ret )
        perror("timer_settime");

    while(1);
}

/********************************************************************/
//result
// 10:16:45 thread 2974729984, val = 6, signal captured.
// 10:16:47 thread 2974729984, val = 6, signal captured.
// 10:16:49 thread 2974729984, val = 6, signal captured.
// 10:16:51 thread 2974729984, val = 6, signal captured.
// 10:16:53 thread 2974729984, val = 6, signal captured.
/********************************************************************/

