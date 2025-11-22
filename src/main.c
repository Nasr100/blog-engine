#include <stdio.h>
#include <stdlib.h>


// #include "./include/socket_manager.h"
#include "./include/app_context.h"
#include "../shared/logger/logger.h"

#include "./include/server.h"

#define PORT 8081

int main(){
	printf("MAIN STARTED\n");
    
    server* server = create_server(PORT,"blogServer",20);
    if (!server) {
        printf("Failed to create server\n");
        return 1;
    }
    printf("Server created: %p\n", (void*)server);
    
    printf("Starting server loop with is_running=%d\n", server->is_running);
    while(server->is_running){
        printf("Server loop iteration...\n");
        server_loop(server);
        printf("Server loop iteration ended, is_running=%d\n", server->is_running);
    }
    printf("Server loop ended\n");
    
    printf("Destroying server at %p...\n", (void*)server);
    destroy_server(server);
    server = NULL;
    printf("Server destroyed and set to NULL\n");
    
    printf("Final cleanup completed\n");
    printf("Returning from main\n");
    return 0;

}
