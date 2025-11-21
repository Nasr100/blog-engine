#ifndef SERVER_H
#define SERVER_H
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/select.h>
#include "types.h"

typedef struct server{
    AppContext* ctx;
    char* name;
    int server_socket;
    fd_set* master;
    int port;
    int max_clients;
    int is_running;
}server;

server* create_server(int port,char* servername,int max_clients);
void server_loop(server* server);
void destroy_server(server* s);



#endif