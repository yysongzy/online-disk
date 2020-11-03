#include "../include/threadpool.h"
#include "../include/MessageProtocol.h"

int threadPoolInit(pThreadPool_t pPool, int threadNum){
    memset(pPool, 0, sizeof(ThreadPool_t));
    queInit(&pPool->que);
    pPool->startFlag = 0;
    pPool->threadNum = threadNum;
    pPool->pthid = (pthread_t*)calloc(threadNum, sizeof(pthread_t));
    return 0;
}

int threadPoolStart(pThreadPool_t pPool){
    if(0 == pPool->startFlag){
        for(int i = 0; i != pPool->threadNum; ++i){
            pthread_create(pPool->pthid + i, NULL, threadFunc, &pPool->que);
        }
        pPool->startFlag = 1;
        return 0;
    }
    return -1;
}

void *threadFunc(void *pArg){
    pQue_t pQue = (pQue_t)pArg;
    pNode_t pCur = NULL;
    int getSuccess = 0;
    char begPath[128] = {0};
    //保存上一次的路径
    char lastPath[128] = {0};
    char curPath[256] = {0};
    int ret = 0;
    char noFile[128] = "No such file or directory";
    Msg_t msg;
    memset(&msg, 0, sizeof(msg));
    while(1){
        pthread_mutex_lock(&pQue->mutex);
        if(0 == pQue->size){
            printf("wait\n");
            pthread_cond_wait(&pQue->cond, &pQue->mutex);
            printf("wake up\n");
        }
        getSuccess = queGet(pQue, &pCur);
        pthread_mutex_unlock(&pQue->mutex);
        if(0 == getSuccess){
            //记录需要退出的子线程信息
            int pid = 0;
            char username[64] = {0};
            //接收pid
            recv(pCur->clientFd, &pid, 4, 0);
            //接收用户名
            recv(pCur->clientFd, username, 64, 0);
            //将username,salt,ciphertext写入login数据库
            login(pCur->clientFd, username);
            //获取初始路径
            getcwd(begPath, sizeof(begPath));
            while(1){
                pQue->exitNode[pCur->clientFd].exitFlag = slot++;
                slot %= 30;
                pQue->exitNode[pCur->clientFd].pid = pid;
                pQue->exitNode[pCur->clientFd].exitFd = pCur->clientFd;
                printf("exitFlag = %d\t", pQue->exitNode[pCur->clientFd].exitFlag);
                printf("pid = %d\t", pQue->exitNode[pCur->clientFd].pid);
                printf("exitFd = %d\n", pQue->exitNode[pCur->clientFd].exitFd);
                write(pQue->exitPipe[1], pQue->exitNode, 20 * sizeof(ExitNode_t));
                //清空消息
                memset(&msg, 0, sizeof(msg));
                ret = recv(pCur->clientFd, &msg, sizeof(msg), 0); 
                if(0 == ret || -1 == ret){
                    break;
                }
                writeLog(msg, username);
                switch(msg.cmd){
                case GETSFILE:
                    ret = transFile(pCur->clientFd, msg.data);
                    if(-1 == ret){
                        //沉睡2秒，因为客户端此时也在睡眠
                        sleep(2);
                        send(pCur->clientFd, noFile, sizeof(noFile), 0);
                    }
                    break;
                case PUTSFILE:
                    recvFile(pCur->clientFd, msg.data);
                    break;
                case REMOVE:
                    sprintf(curPath, "%s%c%s", begPath, '/', msg.data);
                    remove(curPath);
                    printf("Remove successfully\n");
                    break;
                case PWD:
                    send(pCur->clientFd, begPath, sizeof(begPath), 0);
                    break;
                case MKDIR:
                    sprintf(curPath, "%s%c%s", begPath, '/', msg.data);
                    mkdir(curPath, 0777);
                    printf("Make directory successfully\n");
                    break;
                case LS:
                    send(pCur->clientFd, begPath, sizeof(begPath), 0);
                    break;
                case CD:
                    ret = cmd_CD(begPath, msg.data, lastPath);
                    if(0 == ret){
                        socketpair(AF_UNIX, SOCK_STREAM, 0, pQue->cwdPipe);
                        write(pQue->cwdPipe[1], begPath, 128);
                        if(0 == fork()){
                            //注意：因为创建的进程继承了之前的进程环境，
                            //所以当前目录就是家目录
                            char parentPath[128] = {0};
                            read(pQue->cwdPipe[0], parentPath, 128);
                            chdir(parentPath);
                            //更新工作目录
                            getcwd(parentPath, 128);
                            write(pQue->cwdPipe[0], parentPath, 128);
                            exit(0);
                        }
                        read(pQue->cwdPipe[1], begPath, 128);
                        send(pCur->clientFd, begPath, sizeof(begPath), 0);
                    }
                    if(-1 == ret){
                        send(pCur->clientFd, noFile, sizeof(noFile), 0);
                    }
                    break;
                default:
                    break;
                }
            }
            free(pCur);
            pCur = NULL;
        }
    }
}
