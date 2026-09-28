#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/router.h"
#include "../include/response_builder.h"
#include "../include/request_parser.h"
#include "../include/static_files.h"
#include "../../shared/utils/helpers.h"
#include "../controller/post/postController.h"

// char* create_route_key(const char* method, const char* path) {
//     size_t len = strlen(method) + strlen(path) + 2;
//     char* key = malloc(len);
//     if (key) {
//         snprintf(key, len, "%s:%s", method, path);
//     }
//     return key;
// }



router* create_router(){
	router* r = malloc(sizeof(router));
	if(r == NULL) return NULL;
    r->routes = dynamic_array_create();
    return r;
}




// void match_route(http_message* message,int socket,AppContext* ctx){
// 	// route_handler_t handler = NULL;
// 	  printf("🎯 match_route called\n");
//     printf("🎯 URI: '%s'\n", message->request->request_uri);
//     printf("🎯 Method: '%s'\n", message->request->method);
//     printf("🎯 URI length: %zu\n", strlen(message->request->request_uri));
// 	// 
// 	// 

// 	if(strncmp(message->request->request_uri, "/api", 4)==0){
// 		hashmap_get(ctx->api_router->get_routes,);
// 	}




// 	if(strncmp(message->request->request_uri, "/api", 4)==0){
// 		 printf("🚀 API route detected\n");
// 		if (strcmp(message->request->request_uri, "/api/posts") == 0) {
// 			printf("✅ Matched /api/posts route\n");
//         		postController_get_AllpostsPreview(socket, message, ctx);
// 			}
// 			else if (strncmp(message->request->request_uri, "/api/posts/", 11) == 0 && strlen(message->request->request_uri) > 11) {
// 				  printf("✅ Matched /api/posts/:id route\n");
// 				postController_get_postbyId(socket, message, ctx);
// 			}else{
// 				printf("❌ No API route matched for: %s\n", message->request->request_uri);
// 				send_json_response(socket,"{\"error\":\"route not found\"}",404);
// 				log_info(ctx->logger,"404 PAGE was sent to socket %d ",socket);
// 			}
			
// 	}else{
// 		 printf("Static file route detected\n");
// 			if (strcmp(message->request->request_uri, "/") == 0 || strcmp(message->request->request_uri, "/home") == 0) {
// 				printf("Serving home page\n");
// 				serve_static_file(socket, "home.html", ctx);
// 			}
// 			else if (strncmp(message->request->request_uri, "/posts/",7)==0) {
// 				printf("Serving post details page for: %s\n", message->request->request_uri);
// 				serve_static_file(socket, "posts.html", ctx);
// 			}
// 			else {
// 				printf("Serving static file: %s\n", message->request->request_uri);
// 				render_static_file(socket, message->request->request_uri, ctx);
// 			}
		
// 	}	
// }
void match_route(http_message* message,int socket,AppContext* ctx){
    router* r = ctx->api_router; 
    entryRoute* targetRoute  =NULL;
    int i = 0;
    for(i=0;i<r->routes->count;i++){
        entryRoute* tempRoute = r->routes->array[i];
        if(regexec(tempRoute->reg,message->request->request_uri,0,NULL,0)==0){
            puts("ROUTE REGEX MATCHED");
            if(strcmp(message->request->method,tempRoute->method)==0){
                puts("====routes METHOD matched=========");
                targetRoute = tempRoute;
                break;
            }
        }
    }
    if(targetRoute != NULL){
        targetRoute->handler(socket,message,ctx);
    }else{
            printf("❌ No API route matched for: %s\n", message->request->request_uri);
			send_json_response(socket,"{\"error\":\"route not found\"}",404);
			log_info(ctx->logger,"404 PAGE was sent to socket %d ",socket);
    }
    
}


void destroy_route(entryRoute* route){
	if(route==NULL)return;
	if(route->pattern){
		free(route->pattern);
		route->pattern = NULL;
	}
    if(route->method){
        free(route->method);
        route->method = NULL;
    }
    if(route->reg){
        regfree(route->reg);
        route->reg = NULL;
    }
	free(route);
}


void add_route(router* router,char* method,char* pattern,route_handler_t handler){
	if (!router || !method || !pattern || !handler) return;
    
    

    //create entryroute
 	entryRoute* entry = malloc(sizeof(entryRoute));
	if(entry==NULL)return ;
    entry->pattern = strdup(pattern);
    entry->handler = handler;
    entry->method = strdup(method);
    entry->reg = malloc(sizeof(regex_t));
    puts("begining  regex");
    regcomp(entry->reg,pattern,REG_EXTENDED);
    puts("finished regex");
    puts("entry routes created");
    //add entry to the routes
    dynamic_array_push_elem(router->routes,entry);
}

// add api routes here
void routes_api_init(router* router){
    puts("adding routes");
    add_route(router,"GET","^/api/posts$",&postController_get_AllpostsPreview); // /api/posts
    puts("route adeed");
    add_route(router,"GET","^/api/posts/([^/]+)",&postController_get_postbyId); // /api/posts/id
    }

    void destroy_router(router* router){
         int i = 0;
        for(i=0;i<router->routes->count;i++){
            destroy_route(router->routes->array[i]);
        }
        dynamic_array_destroy(router->routes);
        router->routes = NULL;
    }


