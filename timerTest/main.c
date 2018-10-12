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


#if 0

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

#endif

#if 1

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
    value.it_value.tv_sec = 1;
    value.it_value.tv_nsec = 0;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_nsec = 5000000;
    printf_with_time("start timer");
    timer_settime(id, 0, &value, NULL);


    sleep(2);
    printf_with_time("overrun count = %d", timer_getoverrun(id));
    printf_with_time("delete timer");
    timer_delete(id);

    return 0;
}


/********************************************************************/

//wsj@linux-49xs:~/CWorkPlace/timerTest> ./main
//[127687:907654824]create timer
//[127687:907739053]start timer
//[127688:908235105]timer expiration
//[127688:913250848]timer expiration
//[127688:918210325]timer expiration
//[127688:923251906]timer expiration
//[127688:928213809]timer expiration
//[127688:933249708]timer expiration
//[127688:940041253]timer expiration
//[127688:945597313]timer expiration
//[127688:948206089]timer expiration
//[127688:953514902]timer expiration
//[127688:958290963]timer expiration
//[127688:963282062]timer expiration
//[127688:968190596]timer expiration
//[127688:973921808]timer expiration
//[127688:978213985]timer expiration
//[127688:983271930]timer expiration
//[127688:988234203]timer expiration
//[127688:993284164]timer expiration
//[127688:998324941]timer expiration
//[127689:7293147]timer expiration
//[127689:8180442]timer expiration
//[127689:13361618]timer expiration
//[127689:18312443]timer expiration
//[127689:23251637]timer expiration
//[127689:28272868]timer expiration
//[127689:33404946]timer expiration
//[127689:41622628]timer expiration
//[127689:43236239]timer expiration
//[127689:48299193]timer expiration
//[127689:53251443]timer expiration
//[127689:58288769]timer expiration
//[127689:63638629]timer expiration
//[127689:68314744]timer expiration
//[127689:75306293]timer expiration
//[127689:78476338]timer expiration
//[127689:83283383]timer expiration
//[127689:88301939]timer expiration
//[127689:93220207]timer expiration
//[127689:98282727]timer expiration
//[127689:103279451]timer expiration
//[127689:109612085]timer expiration
//[127689:113185103]timer expiration
//[127689:118271234]timer expiration
//[127689:123253143]timer expiration
//[127689:128216795]timer expiration
//[127689:133270901]timer expiration
//[127689:138251079]timer expiration
//[127689:143196813]timer expiration
//[127689:148268936]timer expiration
//[127689:153249894]timer expiration
//[127689:158273572]timer expiration
//[127689:163281823]timer expiration
//[127689:168399171]timer expiration
//[127689:174946455]timer expiration
//[127689:178486689]timer expiration
//[127689:183253641]timer expiration
//[127689:188286697]timer expiration
//[127689:193211491]timer expiration
//[127689:198365468]timer expiration
//[127689:203200495]timer expiration
//[127689:209103565]timer expiration
//[127689:213207552]timer expiration
//[127689:218271403]timer expiration
//[127689:223194328]timer expiration
//[127689:228276060]timer expiration
//[127689:233281972]timer expiration
//[127689:238251776]timer expiration
//[127689:243271709]timer expiration
//[127689:248352007]timer expiration
//[127689:253271448]timer expiration
//[127689:258209804]timer expiration
//[127689:263473069]timer expiration
//[127689:268342414]timer expiration
//[127689:275005281]timer expiration
//[127689:278658701]timer expiration
//[127689:283244649]timer expiration
//[127689:288287239]timer expiration
//[127689:293282493]timer expiration
//[127689:298371537]timer expiration
//[127689:303273131]timer expiration
//[127689:309372309]timer expiration
//[127689:313186729]timer expiration
//[127689:318276354]timer expiration
//[127689:323253919]timer expiration
//[127689:328236929]timer expiration
//[127689:333272616]timer expiration
//[127689:338269664]timer expiration
//[127689:343253137]timer expiration
//[127689:348302708]timer expiration
//[127689:353255562]timer expiration
//[127689:358190393]timer expiration
//[127689:363432958]timer expiration
//[127689:368307644]timer expiration
//[127689:376521550]timer expiration
//[127689:378224561]timer expiration
//[127689:383205935]timer expiration
//[127689:388288681]timer expiration
//[127689:393239293]timer expiration
//[127689:398687046]timer expiration
//[127689:403283922]timer expiration
//[127689:410424104]timer expiration
//[127689:413361514]timer expiration
//[127689:418280934]timer expiration
//[127689:423241931]timer expiration
//[127689:428278271]timer expiration
//[127689:433317328]timer expiration
//[127689:438264389]timer expiration
//[127689:444091746]timer expiration
//[127689:448221453]timer expiration
//[127689:453254763]timer expiration
//[127689:458311895]timer expiration
//[127689:463274727]timer expiration
//[127689:468381774]timer expiration
//[127689:477252120]timer expiration
//[127689:478216404]timer expiration
//[127689:483207809]timer expiration
//[127689:488285693]timer expiration
//[127689:493283264]timer expiration
//[127689:498471573]timer expiration
//[127689:503276130]timer expiration
//[127689:511036406]timer expiration
//[127689:513277287]timer expiration
//[127689:518244041]timer expiration
//[127689:523279000]timer expiration
//[127689:528217604]timer expiration
//[127689:533498770]timer expiration
//[127689:538277352]timer expiration
//[127689:545633550]timer expiration
//[127689:548966336]timer expiration
//[127689:553255326]timer expiration
//[127689:558255612]timer expiration
//[127689:563304283]timer expiration
//[127689:568668940]timer expiration
//[127689:573297514]timer expiration
//[127689:579195368]timer expiration
//[127689:583196583]timer expiration
//[127689:588276782]timer expiration
//[127689:593285035]timer expiration
//[127689:598288749]timer expiration
//[127689:603338176]timer expiration
//[127689:608209418]timer expiration
//[127689:613232134]timer expiration
//[127689:618259347]timer expiration
//[127689:623255518]timer expiration
//[127689:628307620]timer expiration
//[127689:633511915]timer expiration
//[127689:638268318]timer expiration
//[127689:644883684]timer expiration
//[127689:648641165]timer expiration
//[127689:653286105]timer expiration
//[127689:658308530]timer expiration
//[127689:663266349]timer expiration
//[127689:668380164]timer expiration
//[127689:673356072]timer expiration
//[127689:678399997]timer expiration
//[127689:683276224]timer expiration
//[127689:688345458]timer expiration
//[127689:693290744]timer expiration
//[127689:698547652]timer expiration
//[127689:703298036]timer expiration
//[127689:711336357]timer expiration
//[127689:713264232]timer expiration
//[127689:718280340]timer expiration
//[127689:723245286]timer expiration
//[127689:728317574]timer expiration
//[127689:733647478]timer expiration
//[127689:738328370]timer expiration
//[127689:745039220]timer expiration
//[127689:748755230]timer expiration
//[127689:753319536]timer expiration
//[127689:758287370]timer expiration
//[127689:763241418]timer expiration
//[127689:768399903]timer expiration
//[127689:773317110]timer expiration
//[127689:779109646]timer expiration
//[127689:783169922]timer expiration
//[127689:788318766]timer expiration
//[127689:793317420]timer expiration
//[127689:798303236]timer expiration
//[127689:803292344]timer expiration
//[127689:808309311]timer expiration
//[127689:813322310]timer expiration
//[127689:818288570]timer expiration
//[127689:823311186]timer expiration
//[127689:828260674]timer expiration
//[127689:833287111]timer expiration
//[127689:838313899]timer expiration
//[127689:846261919]timer expiration
//[127689:848323254]timer expiration
//[127689:853225002]timer expiration
//[127689:858234441]timer expiration
//[127689:863318890]timer expiration
//[127689:868510648]timer expiration
//[127689:873366388]timer expiration
//[127689:880314565]timer expiration
//[127689:884214796]timer expiration
//[127689:888321997]timer expiration
//[127689:893284943]timer expiration
//[127689:898316616]timer expiration
//[127689:903662943]timer expiration
//[127689:908291093]overrun count = 0
//[127689:908305092]delete timer
//wsj@linux-49xs:~/CWorkPlace/timerTest>

/********************************************************************/

#endif
