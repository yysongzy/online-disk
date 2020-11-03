#include "../include/func.h"

int recvCycle(int sfd, void *buf, int totalLen){
    int ret = 0;
    int recvLen = 0;
    while(recvLen < totalLen){
        ret = recv(sfd, (char*)buf + recvLen, totalLen - recvLen, 0);
        if(0 == ret){
            printf("Bye~\n");
            return -1;
        }
        recvLen += ret;
    }
    return 0;
}
