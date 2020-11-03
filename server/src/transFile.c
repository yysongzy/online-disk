#include "../include/func.h"
#include "../include/threadpool.h"

int transFile(int clientFd, char *cmdData){
    char fileName[64] = {0};
    strcpy(fileName, cmdData);

    int ret = 0;
    int downSize = 0;

    int fd = open(fileName, O_RDWR);
    printf("opennnnnn%d\n", fd);
    ERROR_CHECK(fd, -1, "open");

    train_t train;
    memset(&train, 0, sizeof(train));

    strcpy(train.buf, fileName);
    train.dataLen = strlen(train.buf);
    send(clientFd, &train, sizeof(int) + train.dataLen, 0);

    struct stat fileInfo;
    memset(&fileInfo, 0, sizeof(fileInfo));

    fstat(fd, &fileInfo);
    train.dataLen = sizeof(fileInfo.st_size);
    memcpy(&train.buf, &fileInfo.st_size, train.dataLen);
    send(clientFd, &train, sizeof(int) + train.dataLen, 0);
    recv(clientFd, &downSize, 4, 0);
    lseek(fd, downSize, SEEK_SET);

    while(1){
        train.dataLen = read(fd, train.buf, sizeof(train.buf));
        ret = send(clientFd, &train, 4 + train.dataLen, 0);
        if(-1 == ret){
            break;
        }
        if(0 == train.dataLen){
            break;
        }
    }

    close(fd);
    return 0;
}
