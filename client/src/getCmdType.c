#include "../include/MessageProtocol.h"
#include <string.h>

int getCmdType(char *cmd, Msg_t *pMsg){
    if(0 == strncmp(cmd, "getsfile", 4)){
        pMsg->cmd = GETSFILE;
    }
    else if(0 == strncmp(cmd, "putsfile", 4)){
        pMsg->cmd = PUTSFILE;
    }
    else if(0 == strncmp(cmd, "remove", 6)){
        pMsg->cmd = REMOVE;
    }
    else if(0 == strncmp(cmd, "pwd", 3)){
        pMsg->cmd = PWD;
    }
    else if(0 == strncmp(cmd, "mkdir", 3)){
        pMsg->cmd = MKDIR;
    }
    else if(0 == strncmp(cmd, "ls", 2)){
        pMsg->cmd = LS;
    }
    else if(0 == strncmp(cmd, "cd", 2)){
        pMsg->cmd = CD;
    }
    else if(0 == strncmp(cmd, "\n", 1)){
        pMsg->cmd = ENTER;
    }
    else{
        pMsg->cmd = CMDERROR;
    }
    return 0;
}
