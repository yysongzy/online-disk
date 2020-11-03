#include "../include/MessageProtocol.h"

void writeLog(Msg_t msg, char *username){
    char cmd[64] = {0};
    switch(msg.cmd){
    case GETSFILE:
        memcpy(cmd, "gets", 64);
        insertLog(username, cmd, msg.data);
        break;
    case PUTSFILE:
        memcpy(cmd, "puts", 64);
        insertLog(username, cmd, msg.data);
        break;
    case REMOVE:
        memcpy(cmd, "remove", 64);
        insertLog(username, cmd, msg.data);
        break;
    case PWD:
        memcpy(cmd, "pwd", 64);
        insertLog(username, cmd, msg.data);
        break;
    case MKDIR:
        memcpy(cmd, "mkdir", 64);
        insertLog(username, cmd, msg.data);
        break;
    case LS:
        memcpy(cmd, "ls", 64);
        insertLog(username, cmd, msg.data);
        break;
    case CD:
        memcpy(cmd, "cd", 64);
        insertLog(username, cmd, msg.data);
        break;
    case ENTER:
        memcpy(cmd, "enter", 64);
        insertLog(username, cmd, msg.data);
        break;
    default:
        break;
    }
}
