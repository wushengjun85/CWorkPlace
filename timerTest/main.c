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


#if 0
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

#endif

#if 0
void handle()
{
    time_t t;
    char p[32];

    time(&t);
    strftime(p, sizeof(p), "%T", localtime(&t));

    printf("time is %s\n", p);
}

int main()
{
    struct sigevent evp;
    struct itimerspec ts;
    timer_t timer;
    int ret;

    evp.sigev_value.sival_ptr = &timer;
    evp.sigev_notify = SIGEV_SIGNAL;
    evp.sigev_signo = SIGUSR1;
    signal(SIGUSR1, handle);

    ret = timer_create(CLOCK_REALTIME, &evp, &timer);
    if( ret )
        perror("timer_create");

    ts.it_interval.tv_sec = 1;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = 1;
    ts.it_value.tv_nsec = 0;

    ret = timer_settime(timer, 0, &ts, NULL);
    if( ret )
    perror("timer_settime");

    while(1);
}

/********************************************************************/
//result
//time is 10:28:34
//time is 10:28:35
//time is 10:28:36
//time is 10:28:37
//time is 10:28:38
//time is 10:28:39
//time is 10:28:40
//time is 10:28:41
//time is 10:28:42
//time is 10:28:43
//time is 10:28:44
//time is 10:28:45
//time is 10:28:46
//time is 10:28:47
//time is 10:28:48
//time is 10:28:49
//time is 10:28:50
/********************************************************************/

#endif



#include <signal.h>    /* union sigval / struct sigevent */
#include <stdio.h>    /* printf */
#include <string.h>    /* memset */
#include <unistd.h> /* sleep */
#include <time.h>

#define printf_with_time(format, ...)                                        \
{                                                                            \
    struct timespec spec;                                                    \
    clock_gettime(CLOCK_MONOTONIC, &spec);                                    \
    printf("[%lu:%lu]"format"\n", spec.tv_sec, spec.tv_nsec, ##__VA_ARGS__);\
}

timer_t id;

void timer_notify_cb(union sigval val)
{
    printf_with_time("timer expiration");
}

int main(void)
{
    /* Variable Definition */
    struct timespec spec;
    struct sigevent ent;
    struct itimerspec value;
    struct itimerspec get_val;

    /* Init */
    memset(&ent, 0x00, sizeof(struct sigevent));
    memset(&get_val, 0x00, sizeof(struct itimerspec));

    /* create a timer */
    ent.sigev_notify = SIGEV_THREAD;
    ent.sigev_notify_function = timer_notify_cb;
    printf_with_time("create timer");
    timer_create(CLOCK_MONOTONIC, &ent, &id);

    /* start a timer */
    value.it_value.tv_sec = 6;
    value.it_value.tv_nsec = 0;
    value.it_interval.tv_sec = 2;
    value.it_interval.tv_nsec = 0;
    printf_with_time("start timer");
    timer_settime(id, 0, &value, NULL);

    sleep(2);
    timer_gettime(id, &get_val);
    printf_with_time("timer will expiration after %lu.%lus", get_val.it_value.tv_sec, get_val.it_value.tv_nsec);
    printf_with_time("timer repeat period = %lu.%lus", get_val.it_interval.tv_sec, get_val.it_interval.tv_nsec);

    sleep(10);
    printf_with_time("delete timer");
    timer_delete(id);

    return 0;
}

/********************************************************************/
//result
//[126863:306654172]create timer
//[126863:306738469]start timer
//[126865:307427945]timer will expiration after 3.999329709s
//[126865:307444018]timer repeat period = 2.0s
//[126869:307511123]timer expiration
//[126871:307578299]timer expiration
//[126873:307634776]timer expiration
//[126875:307643987]delete timer
/********************************************************************/
