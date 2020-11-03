#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "func.h"
#include "workque.h"

typedef struct{
    int dataLen;
    char buf[1000];
}train_t;

typedef struct{
    Que_t que;
    short startFlag;
    int threadNum;
    pthread_t *pthid;
}ThreadPool_t, *pThreadPool_t;

int threadPoolInit(pThreadPool_t pPool, int threadNum);
int threadPoolStart(pThreadPool_t pPool);
void *threadFunc(void *pArg);
int tcpInit(int*, char*, char*);
int epollAddFd(int, int);
int epollAddTimeFd(int, int);
int transFile(int, char*);
int recvFile(int, char*);
int addTimeFd(int *timeFd);
//设置一个全局变量slot，表示newFd现在移动到哪个位置
int slot;

#endif
