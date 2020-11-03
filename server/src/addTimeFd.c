#include "../include/threadpool.h"

int addTimeFd(int *timeFd){
    struct itimerspec new_value;
    struct timespec now;

    clock_gettime(CLOCK_REALTIME, &now);

    new_value.it_value.tv_sec = 1;
    new_value.it_value.tv_nsec = now.tv_nsec;

    new_value.it_interval.tv_sec = 1;
    new_value.it_interval.tv_nsec = 0;

    //构建定时器，并返回timefd文件描述符
    *timeFd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);
    //启动定时器
    timerfd_settime(*timeFd, 0, &new_value, NULL);
    
    return 0;
}
