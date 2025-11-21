#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/router.h"
#include "../include/response_builder.h"
#include "../include/request_parser.h"
#include "../include/static_files.h"
#include "../../shared/utils/helpers.h"
#include "../controller/post/postController.h"

typedef void (*route_handler_t)(int socket, http_message* message, AppContext* ctx);

router* create_router(){
	router* router = malloc(sizeof(router));
	if(router == NULL) return NULL;
	router->get_routes = hashmap_create();
	if(router->get_routes == NULL) return NULL;

	router->post_routes = NULL;
	// if(router->post_routes == NULL) return NULL;

	router->delete_routes = NULL;
	// if(router->delete_routes == NULL) return NULL;

	router->put_routes = NULL;
	// if(router->put_routes == NULL) return NULL;
	
	
	
	return router;
}
void routes_init(router* router){
	//get routes 
	hashmap_insert(router->get_routes,"/home",postController_get_AllpostsPreview);
	hashmap_insert(router->get_routes,"/post/*",postController_get_postbyId);
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
				send_json_response(socket,"{\"error\":\"route not found\"}",404);
				log_info(ctx->logger,"404 PAGE was sent to socket %d ",socket);
			}else{
				handler(socket,message,ctx);
			}
		}
		
	}
}

void destroy_router(router* router){
	if(router==NULL)return;
	if(router->get_routes == NULL) hashmap_destroy(router->get_routes);
	router->get_routes = NULL;
	free(router);
	// if(router->post_routes == NULL) hashmap_destroy(router->post_routes);
	// if(router) free(router);
	
}


