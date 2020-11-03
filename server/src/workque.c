#include "../include/workque.h"

int queInit(pQue_t pQue){
    pQue->size = 0;
    memset(pQue->exitNode, -1, 20 * sizeof(ExitNode_t));
    pthread_mutex_init(&pQue->mutex, 0);
    pthread_cond_init(&pQue->cond, 0);
    pQue->pHead = pQue->pTail = NULL;
    memset(pQue->exitPipe, 0, sizeof(pQue->exitPipe));
    memset(pQue->cwdPipe, 0, sizeof(pQue->cwdPipe));
    return 0;
}

int queInsert(pQue_t pQue, pNode_t pNew){
    if(NULL == pQue->pHead){
        pQue->pHead = pNew;
        pQue->pTail = pNew;
    }
    else{
        pQue->pTail->pNext = pNew;
        pQue->pTail = pNew;
    }
    ++pQue->size;
    return 0;
}

int queGet(pQue_t pQue, pNode_t *pGet){
    if(0 == pQue->size){
        return -1;
    }
    else{
        *pGet = pQue->pHead;
        pQue->pHead = pQue->pHead->pNext;
        if(NULL == pQue->pHead){
            pQue->pTail = NULL;
        }
        --pQue->size;
        return 0;
    }
}
