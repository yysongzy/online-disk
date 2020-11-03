#ifndef  MESSAGEPROTOCOL_H_
#define  MESSAGEPROTOCOL_H_

#include "func.h"

enum CMD_TYPE{
    LUANMA,
    CD,
    LS,
    PUTSFILE,
    GETSFILE,
    PWD,
    MKDIR,
    REMOVE,
    CMDERROR,
    ENTER
}cmd;
typedef unsigned int message_size_type;
typedef struct Message {
    message_size_type len;
    enum CMD_TYPE cmd;
    char data[128];
}Msg_t;

int cmd_CD(char*, char*, char*);
void writeLog(Msg_t, char *);
int insertLog(char *username, char *cmd, char *data);
int insertLogin(char *username, char *salt, char *ciphertext);
int queryLogin(char *username, char *salt);
int queryCiphertext(char *ciphertext);
int login(int sfd, char *username);
int generateSalt(char *salt);

#endif
