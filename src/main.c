#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/select.h>

#include "./include/socket_manager.h"
#include "./include/app_context.h"
#include "../shared/logger/logger.h"


#define PORT 8080
/*


*/


int main(int argc, char *argv[]) {
	AppContext* ctx = app_context_create();
	app_context_init(ctx);

	 int connect_socket ;
	 log_info(ctx->logger,"socket created");
	puts("===================================================");
	connect_socket = create_socket(ctx->logger);
	if (connect_socket < 0) {
        fprintf(stderr, "Failed to create socket\n");
        return 1;
    }
    
    if (bind_socket(connect_socket, PORT,ctx->logger) != 0) {
        fprintf(stderr, "Failed to bind socket\n");
        close(connect_socket);
        return 1;
    }
	// system("start chrome http://localhost:8080/home");
	fd_set master;
	FD_ZERO(&master);
	FD_SET(connect_socket,&master);
	server_loop(&master,connect_socket,ctx);
	
	app_context_destroy(ctx);
	close(connect_socket);
	return 0;
}
