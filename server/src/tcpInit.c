#include "../include/func.h"

int tcpInit(int *socketFd, char *ip, char *port){
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sfd, -1, "socket");

    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(port));
    serAddr.sin_addr.s_addr = inet_addr(ip);

    int reuse = 1;
    int ret = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ERROR_CHECK(ret, -1, "setsockopt");

    ret = bind(sfd, (struct sockaddr*)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(sfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    *socketFd = sfd;
    return 0;
}
