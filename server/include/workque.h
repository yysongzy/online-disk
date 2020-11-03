#ifndef __WORKQUE_H__
#define __WORKQUE_H__

#include "func.h"

typedef struct node{
    struct node *pNext;
    int clientFd;
}Node_t, *pNode_t;

typedef struct{
    //记录此时要退出的线程fd
    int exitFd;
    //记录要退出的客户端进程pid，以便在退出时kill掉
    int pid;
    //设置为各子线程的退出标志
    int exitFlag;
}ExitNode_t, *pExitNode_t;

typedef struct{
    pNode_t pHead, pTail;
    int size;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int exitPipe[2];
    int cwdPipe[2];
    ExitNode_t exitNode[20];
}Que_t, *pQue_t;

int queInit(pQue_t);
int queInsert(pQue_t, pNode_t);
int queGet(pQue_t, pNode_t *pGet);

#endif
