#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/router.h"
#include "../include/static_files.h"
#include "../include/response_builder.h"
#include "../../shared/logger/logger.h"

typedef void (*route_handler_t)(int socket, http_message* message, AppContext* ctx);

router* create_router(){
	router* router = malloc(sizeof(router));
	router->get_routes = hashmap_create();
	router->post_routes = hashmap_create();
	return router;
}

int is_static(char* uri){
	return strstr(uri, ".css") || strstr(uri, ".js") || 
           strstr(uri, ".png") || strstr(uri, ".jpg") ||
           strcmp(uri, "/favicon.ico") == 0;
}


void match_route(http_message* message,int socket,AppContext* ctx){
		route_handler_t handler = NULL;
	
	if(strcmp(message->request->method,"GET") == 0){
		int res = is_static(message->request->request_uri);
		if(res){
			render_static_file(socket,message->request->request_uri,ctx);
			
		}else{
			handler = hashmap_get(ctx->router->get_routes,message->request->request_uri);
			char* temp = strstr(message->request->request_uri,"/post");
			if(handler == NULL &&  temp!= NULL){
				handler = hashmap_get(ctx->router->get_routes,"/post/*");
				
			}
		
			if(handler == NULL){
				send_404_page(socket);
				log_info(ctx->logger,"404 PAGE was sent to socket %d ",socket);
			}else{
				handler(socket,message,ctx);
			}
		}
		
	}
}

void destroy_router(router* router){
	if(router->get_routes == NULL) hashmap_destroy(router->get_routes);
	if(router->post_routes == NULL) hashmap_destroy(router->post_routes);
	if(router) free(router);
	
}


