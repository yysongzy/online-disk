#include "../include/MessageProtocol.h"
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

int cmd_LS(char *curPath){
    struct dirent *ptr;
    DIR *dir = opendir(curPath);
    printf("\n");
    int count = 0;
    while(NULL != (ptr = readdir(dir))){
        printf("%-15s\t", ptr->d_name);
        ++count;
        if(0 == count % 5){
            printf("\n");
        }
    }
    
    printf("\n");
    return 0;
}
