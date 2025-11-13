#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include "../include/socket_manager.h"
#include "../include/request_parser.h"
#include "../include/router.h"

int create_socket(logger_t* logger){
    int ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (ConnectSocket < 0) {
		log_error(logger,"server_logger,Error at socket(): %s\n", strerror(errno));
        return -1; 
    }
    log_info(logger,"socket created");
    return ConnectSocket;
}
int bind_socket(int server_socekt,int port,logger_t* logger){
	struct sockaddr_in address;
	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    if(bind(server_socekt,(struct sockaddr*)&address,sizeof(address)) <0){
    	
		log_error(logger,"Error Binding");
    	return 1;
	}
	
	log_info(logger,"BINDING SUCCEED");
	 if(listen(server_socekt, 5) <0) {
        perror("Listen failed");
        return 1;
    }
	log_info(logger,"SERVER LISTENING ON PORT 8080");
	return 0;
}

void handle_new_connection(int server_socket, fd_set* master, int* fdmax,logger_t* logger){
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
    
    if(client_socket < 0) {
        perror("accept failed");
		log_error(logger,"accept client failed");
        return;
    }
    
    if(client_socket > *fdmax){
        *fdmax = client_socket;
    }
    FD_SET(client_socket, master);
    
  
	log_debug(logger,"New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
}

void handle_request(int current_socket,AppContext* ctx){
	http_message* message;
	message = parse_http_message(current_socket,ctx->logger); 
	if(message == NULL){
		log_error(ctx->logger,"socket %d : http message parsed FAILED!",current_socket);
		return;
	}
	log_info(ctx->logger,"socket %d : http message parsed successfully!",current_socket);
	match_route(message,current_socket,ctx);
	close(current_socket); 
	destroy_request(message,ctx->logger); 
    
}


void server_loop(fd_set* master,int server_socket,AppContext* ctx){
	fd_set copy_master;
	int fdmax = server_socket;
	int i = 0;
	while(1){
		copy_master = *master;
		int socket_count = select(fdmax+1,&copy_master,NULL,NULL,NULL);
		
		 if (socket_count < 0) {
            perror("select failed");
            break;
        }
        	
		for(i = 0;i<=fdmax;i++){
			
			
				if(FD_ISSET(i,&copy_master)){
					if(i == server_socket){
						//handle new connection
						handle_new_connection(server_socket,master,&fdmax,ctx->logger);
//						puts("client connected");
					}else{
//						puts("REQUEST COMMING");
						//handle existing connection
						handle_request(i,ctx);
						FD_CLR(i, master);
						if (i == fdmax) {
                        while (fdmax > 0 && !FD_ISSET(fdmax, master))
                            fdmax--;
                    	}
						
					}
				}
			
		}
	}	
	
}






