#include "../include/func.h"

int recvFile(int sfd){
    char client_path[128] = {0};
    getcwd(client_path, sizeof(client_path));

    char buf[1000] = {0};
    char fileName[128] = {0};

    struct stat st;
    char filePath[128] = {0};
    struct dirent *ptr;
    DIR *dir = opendir("./file_dir");

    //沉睡1秒，因为下一次recv为非阻塞，所以需要给服务器时间去准备
    //否则很容易是客户端的recv先执行，因为设置为了非阻塞
    //所以由于服务器还未来得及发送数据而返回-1
    sleep(1);
    int dataLen = 0;
    //设置为非阻塞，若若对端无数据发送则直接返回-1
    int ret = recv(sfd, &dataLen, 4, MSG_DONTWAIT);
    ERROR_CHECK(ret, -1, "recv");
    recv(sfd, buf, dataLen, MSG_WAITALL);
    char path[128] = {0};
    memcpy(fileName, buf, 128);
    sprintf(path, "%s%s%s", client_path, "/file_dir/", buf);
    
    int fd = open(path, O_RDWR|O_CREAT, 0666);
    ERROR_CHECK(fd, -1, "open");
    memset(buf, 0, sizeof(buf));

    off_t fileSize = 0, downSize = 0, lastSize = 0;
    off_t slice = fileSize / 100;
    recv(sfd, &dataLen, 4, MSG_WAITALL);
    recv(sfd, &fileSize, dataLen, MSG_WAITALL);
    printf("fileSize = %ld\n", fileSize);
    while(NULL != (ptr = readdir(dir))){
        if(0 == strcmp(fileName, ptr->d_name)){
            sprintf(filePath, "./file_dir/%s", fileName);
            stat(filePath, &st);
            downSize = st.st_size;
            lseek(fd, downSize, SEEK_SET);
            printf("downSize = %ld\n", downSize);
            send(sfd, &downSize, 4, 0);
        }
    }

    while(1){
        recvCycle(sfd, &dataLen, 4);
        if(0 == dataLen){
            break;
        }
        recvCycle(sfd, buf, dataLen);
        write(fd, buf, dataLen);
        downSize += dataLen;
        if(downSize - lastSize >= slice){
            printf("%5.2f%%\r", (float)downSize / fileSize * 100);
            lastSize = downSize;
            fflush(stdout);
        }
    }

    printf("100.00%%\n");
    close(fd);
    return 0;
}
