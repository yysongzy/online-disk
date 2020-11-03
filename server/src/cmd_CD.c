#include "../include/func.h"
#include "../include/MessageProtocol.h"

int cmd_CD(char *curPath, char *data, char *lastPath){
    char newPath[128] = {0};
    if('~' == data[0]){
        memcpy(lastPath, curPath, 128);
        memcpy(curPath, data, 128);
        return 0;
    }
    else if('/' == data[0]){
        memcpy(lastPath, curPath, 128);
        memcpy(curPath, data, 128);
        return 0;
    }
    else if('-' == data[0]){
        memcpy(curPath, lastPath, 128);
        return 0;
    }
    else{
        sprintf(newPath, "%s%c%s", curPath, '/', data);
        if(NULL == opendir(newPath)){
            printf("No such file or directory\n");
            return -1;
        }
        memcpy(lastPath, curPath, 128);
        memcpy(curPath, newPath, 128);
    }
    return 0;
}
