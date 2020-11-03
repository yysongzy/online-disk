#include "../include/func.h"

int recvFile(int sfd){
    char buf[1000] = {0};

    //沉睡1秒，因为下一次recv为非阻塞，所以需要给服务器时间去准备
    //否则很容易是客户端的recv先执行，因为设置为了非阻塞
    //所以由于服务器还未来得及发送数据而返回-1
    sleep(1);
    int dataLen = 0;
    //设置为非阻塞，若对端无数据发送则直接返回-1
    int ret = recv(sfd, &dataLen, sizeof(int), MSG_DONTWAIT);
    ERROR_CHECK(ret, -1, "recv");
    printf("dataLen = %d\n", dataLen);
    recv(sfd, buf, dataLen, MSG_WAITALL);
    printf("buf = %s\n", buf);

    int fd = open(buf, O_RDWR|O_CREAT, 0666);
    ERROR_CHECK(fd, -1, "open");
    memset(buf, 0, sizeof(buf));

    off_t fileSize;
    recv(sfd, &dataLen, sizeof(int), MSG_WAITALL);
    recv(sfd, &fileSize, dataLen, MSG_WAITALL);

    ftruncate(fd, fileSize);
    char *pMap = (char*)mmap(NULL, fileSize, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
    ERROR_CHECK(pMap, (char*)-1, "mmap");

    recv(sfd, pMap, fileSize, MSG_WAITALL);

    munmap(pMap, fileSize);
    close(fd);
    return 0;
}
