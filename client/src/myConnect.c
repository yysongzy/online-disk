#include "../include/func.h"

int myConnect(int sfd, char *ip, char *port){
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(port));
    serAddr.sin_addr.s_addr = inet_addr(ip);

    int ret = connect(sfd, (struct sockaddr*)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "connect");
    return 0;
}
