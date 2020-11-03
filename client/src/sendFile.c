#include "../include/func.h"

int sendFile(int sfd, char *cmdData){
    char fileName[64] = {0};
    strcpy(fileName, cmdData);
    
    char client_path[128] = {0};
    getcwd(client_path, sizeof(client_path));

    char path[128] = {0};
    sprintf(path, "%s%s%s", client_path, "/file_dir/", fileName);

    int fd = open(path, O_RDWR);
    printf("sendfd = %d\n", fd);
    ERROR_CHECK(fd, -1, "open");

    train_t train;
    memset(&train, 0, sizeof(train));

    strcpy(train.buf, fileName);
    train.dataLen = strlen(train.buf);
    send(sfd, &train, sizeof(int) + train.dataLen, 0);

    struct stat fileInfo;
    memset(&fileInfo, 0, sizeof(fileInfo));

    fstat(fd, &fileInfo);
    train.dataLen = sizeof(fileInfo.st_size);
    memcpy(&train.buf, &fileInfo.st_size, train.dataLen);
    send(sfd, &train, sizeof(int) + train.dataLen, 0);

    char *pMap = (char*)mmap(NULL, fileInfo.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    ERROR_CHECK(pMap, (char*)-1, "mmap");
    send(sfd, pMap, fileInfo.st_size, 0);

    munmap(pMap, fileInfo.st_size);
    close(fd);
    return 0;
}
