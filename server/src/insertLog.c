#include "../include/func.h"
#include "../include/MessageProtocol.h"

int insertLog(char *username, char *cmd, char *data){
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW *row;
    char *server = "localhost";
    char *user = "root";
    char *password = "key123";
    char *database = "wangpan";
    char query[200] = {0};
    sprintf(query, "insert into log(username, cmd, data) values('%s', '%s', '%s')", username, cmd, data);
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
