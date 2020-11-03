#include "../include/func.h"
#include "../include/MessageProtocol.h"

void sigFunc(int sigNum){
    printf("I'm down, bye~\n");
    exit(0);
}

int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 3);   
    int ret = 0;

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sfd, -1, "socket");

    char username[64] = {0};
    char *passwd;
    char salt[64] = {0};
    char *ciphertext = NULL;
    int pid = getpid();
    printf("Please enter your username:");
    scanf("%s", username);

    myConnect(sfd, argv[1], argv[2]);

    //发送pid
    send(sfd, &pid, 4, 0);
    //发送用户名
    send(sfd, username, 64, 0);
    char isRegistered[2] = {0};
    char isPasswordCorrect[2] = {0};
    //输入密码
    passwd = getpass("Please enter password:");
    recv(sfd, isRegistered, 2, 0);
    if(0 == strcmp(isRegistered, "0")){
        //接收服务器随机生成的salt
        recv(sfd, salt, 64, 0);
        //根据salt和密码生成密文
        ciphertext = crypt(passwd, salt);
        //发送密文
        send(sfd, ciphertext, 72, 0);
        printf("This is your first login and an account has been registered for you\n");
    }
    else{
        //接收服务器随机生成的salt
        recv(sfd, salt, 64, 0);
label:
        //根据salt和密码生成密文
        ciphertext = crypt(passwd, salt);
        //发送密文
        send(sfd, ciphertext, 72, 0);
        //接收密码是否正确的消息
        recv(sfd, isPasswordCorrect, 2, 0);
        if(0 == strcmp(isPasswordCorrect, "0")){
            passwd = getpass("Wrong password, please re-enter");
            goto label;
        }
        else{
            printf("This user is already registered\n");
        }
    }

    printf("Please enter the command:\n");
    char cmd[64] = {0};
    int cmdLen = 0;
    Msg_t msg;
    memset(&msg, 0, sizeof(msg));

    char curPath[128] = {0};

    while(1){
        signal(SIGUSR1, sigFunc);
        memset(curPath, 0, sizeof(curPath));
        memset(cmd, 0, sizeof(cmd));
        memset(&msg, 0, sizeof(msg));
        fgets(cmd, 64, stdin);
        getCmdType(cmd, &msg);
        for(int i = 0; i != sizeof(cmd) - 1; ++i){
            if(' ' == cmd[i] || '\0' == cmd[i]){
                cmdLen = i + 1;
                break;
            }
        }
        for(int i = 0; i != sizeof(cmd) - 1; ++i){
            if('\n' == cmd[i + cmdLen]){
                break;
            }
            msg.data[i] = cmd[i + cmdLen];
        }
        msg.len = strlen(msg.data);
        send(sfd, &msg, sizeof(msg), 0);
        switch(msg.cmd){
        case GETSFILE:
            ret = recvFile(sfd);
            //接收noFile发来的错误消息
            if(-1 == ret){
                recv(sfd, curPath, sizeof(curPath), 0);
                printf("%s\n", curPath);
            }
            printf("Please enter the command:\n");
            break;
        case PUTSFILE:
            ret = sendFile(sfd, msg.data);
            if(-1 == ret){
                printf("No such file, please re-enter the command:\n");
                break;
            }
            printf("Please enter the command:\n");
            break;
        case REMOVE:
            printf("Remove successfully\n");
            printf("Please enter the command:\n");
            break;
        case PWD:
            recv(sfd, curPath, sizeof(curPath), 0);
            printf("%s\n", curPath);
            printf("Please enter the command:\n");
            break;
        case MKDIR:
            printf("Make directory successfully\n");
            printf("Please enter the command:\n");
            break;
        case LS:
            ret = recv(sfd, curPath, sizeof(curPath), 0);
            cmd_LS(curPath);
            printf("Please enter the command:\n");
            break;
        case CD:
            recv(sfd, curPath, sizeof(curPath), 0);
            printf("%s\n", curPath);
            if('N' == curPath[0]){
                printf("Please enter the command:\n");
                break;
            }
            cmd_LS(curPath);
            printf("Please enter the command:\n");
            break;
        case CMDERROR:
            printf("No such command, please re-enter:\n");
            break;
        case ENTER:
            break;
        default:
            break;
        }
    }
    close(sfd);
    return 0;
}
