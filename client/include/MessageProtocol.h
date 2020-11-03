#ifndef  MESSAGEPROTOCOL_H_
#define  MESSAGEPROTOCOL_H_

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

int cmd_LS(char*);

int getCmdType(char*, Msg_t*);
#endif
