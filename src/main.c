#include <stdio.h>
#include <stdlib.h>


// #include "./include/socket_manager.h"
#include "./include/app_context.h"
#include "../shared/logger/logger.h"

#include "./include/server.h"

#define PORT 8080
/*


*/



int main(){
	server* server = create_server("8080","blogServer",20);
	while(server->is_running){
		server_loop(server);
	}
	return 0;
}
