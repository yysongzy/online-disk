#ifndef __FUNC_H__
#define __FUNC_H__

#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/sendfile.h>
#include <shadow.h>
#include <crypt.h>
#include <mysql/mysql.h>
#include <sys/timerfd.h>
#include <assert.h>
#define ARGS_CHECK(argc, val) {if(argc != val){printf("args error!\n"); return -1;}}
#define ERROR_CHECK(ret, val, info) {if(ret == val){perror(info); return -1;}}
#define THREAD_ERRORCHECK(ret, funcName) do{if(0 != ret){printf("%s : %s\n", funcName, strerror(ret));}}while(0)

#endif
