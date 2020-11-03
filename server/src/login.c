#include "../include/MessageProtocol.h"

#define STR_LEN 11
int generateSalt(char *salt){
    int flag;

    srand(time(NULL));
    for(int i = 0; i != STR_LEN; ++i){
        flag = rand() % 3;
        switch(flag){
        case 0:
            salt[i] = rand() % 26 + 'a';
            break;
        case 1:
            salt[i] = rand() % 26 + 'A';
            break;
        case 2:
            salt[i] = rand() % 26 + '0';
            break;
        }
    }
    salt[0] = '$';
    salt[1] = '6';
    salt[2] = '$';
    return 0;
}

int insertLogin(char *username, char *salt, char *ciphertext){
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW *row;
    char *server = "localhost";
    char *user = "root";
    char *password = "key123";
    char *database = "wangpan";
    char query[200] = {0};
    sprintf(query, "insert into login(username, salt, ciphertext) values('%s', '%s', '%s')", username, salt, ciphertext);
    int t, r;
    conn = mysql_init(NULL);
    if(!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)){
        printf("Error connecting to database: %s\n", mysql_error(conn));
    }
    else{
        printf("Connected...\n");
    }
    t = mysql_query(conn, query);
    if(t){
        printf("Error making query: %s\n", mysql_error(conn));
    }
    else{
        printf("Insert successfully\n");
    }
    mysql_close(conn);
    return 0;
}

int queryLogin(char *username, char *salt){
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server = "localhost";
    char *user = "root";
    char *password = "key123";
    char *database = "wangpan";
    char query[300] = "select * from login where username = '";
    sprintf(query, "%s%s%s", query, username, "'");
    puts(query);
    unsigned int t, r;
    conn = mysql_init(NULL);
    if(!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)){
        printf("Error connecting to database: %s\n", mysql_error(conn));
        return -1;
    }
    else{
        printf("Connected...\n");
    }
    t = mysql_query(conn, query);
    if(t){
        printf("Error making query: %s\n", mysql_error(conn));
    }
    else{
        res = mysql_store_result(conn);
        printf("mysql_num_rows(res) = %lld\n", mysql_num_rows(res));
        row = mysql_fetch_row(res);
        if(NULL == row){
            printf("This user is not registered\n");
            mysql_free_result(res);
            mysql_close(conn);
            return 0;
        }
        else{
            do{
                strcpy(salt, row[2]);
                //fields - 2是为了不显示密文
                for(t = 0; t < mysql_num_fields(res) - 2; ++t){
                    printf("%-10s", row[t]);
                }
                printf("\n");
            }while(NULL != (row = mysql_fetch_row(res)));
            mysql_free_result(res);
            mysql_close(conn);
            return 1;
        }
    }
    return 0;
}

int queryCiphertext(char *ciphertext){
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server = "localhost";
    char *user = "root";
    char *password = "key123";
    char *database = "wangpan";
    char query[300] = "select * from login where ciphertext = '";
    sprintf(query, "%s%s%s", query, ciphertext, "'");
    puts(query);
    unsigned int t, r;
    conn = mysql_init(NULL);
    if(!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)){
        printf("Error connecting to database: %s\n", mysql_error(conn));
        return -1;
    }
    else{
        printf("Connected...\n");
    }
    t = mysql_query(conn, query);
    if(t){
        printf("Error making query: %s\n", mysql_error(conn));
    }
    else{
        res = mysql_store_result(conn);
        printf("mysql_num_rows(res) = %lld\n", mysql_num_rows(res));
        row = mysql_fetch_row(res);
        if(NULL == row){
            printf("This user is not registered\n");
            mysql_free_result(res);
            mysql_close(conn);
            return 0;
        }
        else{
            do{
                //fields - 2是为了不显示密文
                for(t = 0; t < mysql_num_fields(res) - 2; ++t){
                    printf("%-10s", row[t]);
                }
                printf("\n");
            }while(NULL != (row = mysql_fetch_row(res)));
            mysql_free_result(res);
            mysql_close(conn);
            return 1;
        }
    }
    return 0;
}


int login(int sfd, char *username){
    char salt[64] = {0};
    char ciphertext[72] = {0};
    //先查询此用户名是否已注册
    //queryLogin返回1表示此用户已注册，返回0表示此用户未注册
    //若已注册则取出salt值
    int ret = queryLogin(username, salt);
    //isRegistered判断是否已注册，若已注册则发送消息给客户端
    //取消之后的salt和密文传送
    //0表示未注册，1表示已注册
    char isRegistered[2] = "0";
    //isPasswordCorrect判断密码是否正确，0表示密码错误，1表示密码正确
    char isPasswordCorrect[2] = "1";
    if(1 == ret){
        //若已注册，则验证客户端发来的密文正确性
        strcpy(isRegistered, "1");
        send(sfd, isRegistered, 2, 0);
        //发送salt
        send(sfd, salt, 64, 0); 
label:
        //重置为1
        strcpy(isPasswordCorrect, "1");
        //接收密文
        recv(sfd, ciphertext, 72, 0);
        //queryCiphertext返回1表示密码正确，返回0表示密码错误
        ret = queryCiphertext(ciphertext);
        if(0 == ret){
            strcpy(isPasswordCorrect, "0");
            send(sfd, isPasswordCorrect, 2, 0);
            printf("Wrong password\n");
            goto label;
        }
        else{
            send(sfd, isPasswordCorrect, 2, 0);
            printf("This user is already registered\n");
        }
        return 0;
    }
    send(sfd, isRegistered, 2, 0);
    //生成salt
    generateSalt(salt);
    //发送salt
    send(sfd, salt, 64, 0); 
    //接收密文
    recv(sfd, ciphertext, 72, 0);
    //将用户名，salt，密文写入login数据库
    insertLogin(username, salt, ciphertext);
    return 0;
}
