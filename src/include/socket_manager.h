#ifndef SOCKET_MANAGER_H

#define SOCKET_MANAGER_H
#include <sys/select.h>
#include "../../shared/logger/logger.h"
#include "app_context.h"

int create_socket(logger_t* logger);
int bind_socket(int server_socekt,int port,logger_t* logger);
void server_loop(fd_set* master,int server_socket,AppContext* ctx);
#endif
