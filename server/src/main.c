#include "../include/func.h"
#include "../include/threadpool.h"
#include "../include/workque.h"

int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 2);
    FILE *fp;
    fp = fopen(argv[1], "r+");
    int threadNum;
    char ip[64];
    char port[64];
    fscanf(fp, "%s", ip);
    fscanf(fp, "%s", port);
    fscanf(fp, "%d", &threadNum);
    fclose(fp);

    ThreadPool_t threadPool;
    memset(&threadPool, 0, sizeof(threadPool));

    threadPoolInit(&threadPool, threadNum);

    threadPoolStart(&threadPool);

    //记录将要退出的子线程节点信息
    ExitNode_t exitNode[20];
    for(int i = 0; i != 20; ++i){
        exitNode[i].exitFd = -1;
        exitNode[i].exitFlag = -1;
        exitNode[i].pid = -1;
    }
    //将队列中exitPipe添加到socketpair
    int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, threadPool.que.exitPipe);
    ERROR_CHECK(ret, -1, "socketpair");

    int sfd;
    tcpInit(&sfd, ip, port);

    int timeFd;
    addTimeFd(&timeFd);
    uint64_t exp;

    int epfd = epoll_create(1);
    epollAddFd(epfd, sfd);
    epollAddFd(epfd, threadPool.que.exitPipe[0]);
    epollAddTimeFd(epfd, timeFd);

    struct epoll_event evs[5];
    memset(evs, 0, sizeof(evs));

    int newFd = 0;
    int readyNum = 0;
    while(1){
        readyNum = epoll_wait(epfd, evs, 5, -1);
        for(int i = 0; i != readyNum; ++i){
            if(sfd == evs[i].data.fd){
                newFd = accept(sfd, NULL, NULL);
                pNode_t pNew = (pNode_t)calloc(1, sizeof(Node_t));
                pNew->clientFd = newFd;
                pthread_mutex_lock(&threadPool.que.mutex);
                queInsert(&threadPool.que, pNew);
                pthread_mutex_unlock(&threadPool.que.mutex);
                pthread_cond_signal(&threadPool.que.cond);
            }
            else if(timeFd == evs[i].data.fd){
                read(evs[i].data.fd, &exp, sizeof(uint64_t));
                ++slot;
                slot %= 30;
                printf("Count down: %d\n", slot);
                for(int j = 0; j != 20; ++j){
                    if(exitNode[j].exitFlag == slot){
                        printf("pid = %d is down, bye~\n", exitNode[j].pid);
                        close(exitNode[j].exitFd);
                        kill(exitNode[j].pid, SIGUSR1);
                        //复原pid、exitFd和exitFd的值，以防重复
                        exitNode[j].pid = -1;
                        exitNode[j].exitFlag = -1;
                        exitNode[j].exitFd = -1;
                    }
                }
            }
            else if(threadPool.que.exitPipe[0] == evs[i].data.fd){
                //接收子线程发送的要退出的子线程信息
                read(threadPool.que.exitPipe[0], exitNode, 20 * sizeof(ExitNode_t));
            }
        }
    }
}
