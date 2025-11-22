#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "../include/server.h"
#include "../../shared/utils/helpers.h"
#include "../include/request_parser.h"
#include "../include/app_context.h"
#include <errno.h>

static int bind_socket(server* server){
    struct sockaddr_in address;
	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(server->port);
    int opt = 1;
    if (setsockopt(server->server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        // Don't return -1 here, just continue
    }
    if(bind(server->server_socket,(struct sockaddr*)&address,sizeof(address)) <0){
    	printf("Error Binding: %s\n", strerror(errno));
		log_error(app_context_get_logger(server->ctx),"Error Binding");
    	return -1;
	}
	
	log_info(app_context_get_logger(server->ctx),"BINDING SUCCEED");
	 if(listen(server->server_socket, server->max_clients) <0) {
        perror("Listen failed");
        return -1;
    }
	log_info(app_context_get_logger(server->ctx),"SERVER LISTENING ON PORT %d",server->port);
    return 0;
}

server* create_server(int port,char* servername,int max_clients){
    puts("CREATING SERVER ...");
    printf("create_server called with port: %d\n", port);
    
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0) {
        printf("server_logger,Error at socket(): %s\n", strerror(errno));
        return NULL; 
    }
    puts("SOCKET CREATED ...");
    
    server* s = malloc(sizeof(server));
    if(s == NULL){
        printf("Failed to allocate memory for %s \n",servername);
        close(socketFd);
        return NULL;
    }
    memset(s, 0, sizeof(server));
    s->server_socket = socketFd;
    
    // 🎯 CREATE CONTEXT
    s->ctx = app_context_create();
    printf("APP CONTEXT CREATED at address: %p\n", s->ctx);
    
    if(s->ctx == NULL){
        printf("❌ Failed to create app context\n");
        close(s->server_socket);
        free(s);
        return NULL;
    }
    
    // 🎯 INITIALIZE CONTEXT - don't overwrite the pointer!
    if(app_context_init(s->ctx) == NULL){
        printf("❌ Failed to initialize app context\n");
        app_context_destroy(s->ctx);  // 🎯 CLEAN UP the created context!
        close(s->server_socket);
        free(s);
        return NULL;
    }
    
    // Check logger
    if (s->ctx) {
        logger_t* logger = app_context_get_logger(s->ctx);
        printf("Logger pointer: %p\n", logger);
        
        if (logger == NULL) {
            printf("ERROR: Logger is NULL!\n");
            app_context_destroy(s->ctx);
            close(s->server_socket);
            free(s);
            return NULL;
        }
    }
    
    s->port = port;
    if(bind_socket(s) != 0){
        printf("❌ Failed to bind socket\n");
        app_context_destroy(s->ctx);
        close(s->server_socket);
        free(s);
        return NULL;
    }
    
    s->max_clients = max_clients;
    s->is_running = 1;
    s->name = my_strdup(servername);
    s->master = malloc(sizeof(fd_set));
    if (!s->master) {
        printf("❌ Failed to allocate fd_set\n");
        app_context_destroy(s->ctx);
        close(s->server_socket);
        free(s->name);
        free(s);
        return NULL;
    }
    FD_ZERO(s->master);
    FD_SET(s->server_socket, s->master);

    printf("%s created successfully\n", s->name);
    return s;
}




static void handle_new_connection(server* server, int* fdmax){
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    
    client_socket = accept(server->server_socket, (struct sockaddr*)&client_addr, &addr_len);
    
    if(client_socket < 0) {
        perror("accept failed");
		log_error(app_context_get_logger(server->ctx),"accept client failed");
        return;
    }
    
    if(client_socket > *fdmax){
        *fdmax = client_socket;
    }
    FD_SET(client_socket, server->master);
    
  
	log_debug(app_context_get_logger(server->ctx),"New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
}

static void handle_request(int current_socket,server* server){

	http_message* message = parse_http_message(current_socket,server->ctx); 
	if(message == NULL){
		log_error(app_context_get_logger(server->ctx),"socket %d : http message parsed FAILED!",current_socket);
		return;
	}
	log_info(app_context_get_logger(server->ctx),"socket %d : http message parsed successfully!",current_socket);
	match_route(message,current_socket,server->ctx);
	close(current_socket); 
	destroy_httpMessage(message); 
}

void server_loop(server* server){
	fd_set copy_master;
	int fdmax = server->server_socket;
	int i = 0;
	while(1){
		copy_master = *(server->master);
		int socket_count = select(fdmax+1,&copy_master,NULL,NULL,NULL);
		
		 if (socket_count < 0) {
            perror("select failed");
            break;
        }
        	
		for(i = 0;i<=fdmax;i++){
			
			
				if(FD_ISSET(i,&copy_master)){
					if(i == server->server_socket){
						//handle new connection
						handle_new_connection(server,&fdmax);
//						puts("client connected");
					}else{
//						puts("REQUEST COMMING");
						//handle existing connection
						handle_request(i,server);
						FD_CLR(i, server->master);
						if (i == fdmax) {
                        while (fdmax > 0 && !FD_ISSET(fdmax, server->master))
                            fdmax--;
                    	}
						
					}
				}
			
		}
	}	
	
}

// void destroy_server(server* s){
//     if(s==NULL)return;
//     s->is_running = 0;
//     close(s->server_socket);
//     if(s->ctx)app_context_destroy(s->ctx);
//     s->ctx=NULL;
//     if(s->name)free(s->name);
//     s->name=NULL;
//     if(s->master)free(s->master);
//     s->master=NULL;
//     free(s);
    

// }
void destroy_server(server* srv) {
    printf("🚨 ===== DESTROY_SERVER START =====\n");
    printf("🚨 destroy_server called for: %p\n", (void*)srv);
    
    if (!srv) {
        printf("🚨 destroy_server: NULL server pointer\n");
        return;
    }
    
    // Print all server fields for debugging
    printf("🔧 Server contents:\n");
    printf("🔧   name: %p -> '%s'\n", (void*)srv->name, srv->name ? srv->name : "NULL");
    printf("🔧   server_socket: %d\n", srv->server_socket);
    printf("🔧   master: %p\n", (void*)srv->master);
    printf("🔧   port: %d\n", srv->port);
    printf("🔧   max_clients: %d\n", srv->max_clients);
    printf("🔧   is_running: %d\n", srv->is_running);
    printf("🔧   ctx: %p\n", (void*)srv->ctx);
    
    printf("🔧 Freeing fd_set master...\n");
    if (srv->master) {
        free(srv->master);
        srv->master = NULL;
        printf("✅ fd_set master freed\n");
    } else {
        printf("✅ fd_set master was NULL\n");
    }
    
    printf("🔧 Closing server socket...\n");
    if (srv->server_socket > 0) {
        close(srv->server_socket);
        srv->server_socket = -1;
        printf("✅ Server socket closed\n");
    } else {
        printf("✅ Server socket was invalid: %d\n", srv->server_socket);
    }
    
    printf("🔧 Destroying app context...\n");
    if (srv->ctx) {
        app_context_destroy(srv->ctx);
        srv->ctx = NULL;
        printf("✅ App context destroyed\n");
    } else {
        printf("✅ App context was NULL\n");
    }
    
    printf("🔧 Freeing server name...\n");
    if (srv->name) {
        free(srv->name);
        srv->name = NULL;
        printf("✅ Server name freed\n");
    } else {
        printf("✅ Server name was NULL\n");
    }
    
    printf("🔧 Freeing server struct at: %p\n", (void*)srv);
    free(srv);
    printf("✅ Server struct freed\n");
    
    printf("🚨 ===== DESTROY_SERVER COMPLETE =====\n");
}
