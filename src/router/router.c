#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/router.h"
#include "../include/response_builder.h"
#include "../include/request_parser.h"
#include "../include/static_files.h"
#include "../../shared/utils/helpers.h"
#include "../controller/post/postController.h"

char* create_route_key(const char* method, const char* path) {
    size_t len = strlen(method) + strlen(path) + 2;
    char* key = malloc(len);
    if (key) {
        snprintf(key, len, "%s:%s", method, path);
    }
    return key;
}



router* create_router(){
	router* r = malloc(sizeof(router));
	if(r == NULL) return NULL;
    r->get_routes = hashmap_create();
    r->post_routes = hashmap_create();
    r->put_routes = hashmap_create();
    r->delete_routes = hashmap_create();
    return r;
}




void match_route(http_message* message,int socket,AppContext* ctx){
	// route_handler_t handler = NULL;
	  printf("🎯 match_route called\n");
    printf("🎯 URI: '%s'\n", message->request->request_uri);
    printf("🎯 Method: '%s'\n", message->request->method);
    printf("🎯 URI length: %zu\n", strlen(message->request->request_uri));
	// 
	// need to create routes hashmap so when i have a new route add it directly to hashmap and create a match function 
	// 
	if(strncmp(message->request->request_uri, "/api", 4)==0){
		 printf("🚀 API route detected\n");
		 //i assumed its get protocole
		if (strcmp(message->request->request_uri, "/api/posts") == 0) {
			printf("✅ Matched /api/posts route\n");
        		postController_get_AllpostsPreview(socket, message, ctx);
			}
			else if (strncmp(message->request->request_uri, "/api/posts/", 11) == 0 && strlen(message->request->request_uri) > 11) {
				  printf("✅ Matched /api/posts/:id route\n");
				postController_get_postbyId(socket, message, ctx);
			}else{
				printf("❌ No API route matched for: %s\n", message->request->request_uri);
				send_json_response(socket,"{\"error\":\"route not found\"}",404);
				log_info(ctx->logger,"404 PAGE was sent to socket %d ",socket);
			}
			
	}else{
		 printf("Static file route detected\n");
			if (strcmp(message->request->request_uri, "/") == 0 || strcmp(message->request->request_uri, "/home") == 0) {
				printf("Serving home page\n");
				serve_static_file(socket, "home.html", ctx);
			}
			else if (strncmp(message->request->request_uri, "/posts/",7)==0) {
				printf("Serving post details page for: %s\n", message->request->request_uri);
				serve_static_file(socket, "posts.html", ctx);
			}
			else {
				printf("Serving static file: %s\n", message->request->request_uri);
				render_static_file(socket, message->request->request_uri, ctx);
			}
		// if(strcmp(message->request->method,"GET") == 0){
			
		// }
	}	
}

void destroy_route(entryRoute* route){
	if(route==NULL)return;
	if(route->pattern){
		free(route->pattern);
		route->pattern = NULL;
	}
	free(route);
}


// void add_route(router* router,char* method,char* pattern,route_handler_t handler){
// 	if (!router || !method || !pattern || !handler) return;

// 	  hashmap_t* target_map = NULL;
    
//     if (strcmp(method, "GET") == 0) target_map = r->get_routes;
//     else if (strcmp(method, "POST") == 0) target_map = r->post_routes;
//     else if (strcmp(method, "PUT") == 0) target_map = r->put_routes;
//     else if (strcmp(method, "DELETE") == 0) target_map = r->delete_routes;
//     else return;

//  	entryRoute* entry = malloc(sizeof(entryRoute));
// 	if(entry==NULL)return NULL;
//     entry->pattern = strdup(pattern);
//     entry->handler = handler;
//     entry->is_dynamic = (strstr(pattern, ":") != NULL);
    
//     hashmap_insert(target_map, pattern, entry);
	
// }

