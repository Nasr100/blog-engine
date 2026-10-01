#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/router.h"
#include "../include/response_builder.h"
#include "../include/request_parser.h"
#include "../include/static_files.h"
#include "../../shared/utils/helpers.h"
#include "../controller/post/postController.h"
#include "../controller/home/homeController.h"
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
    r->dynamic_routes = dynamic_array_create();
    r->static_routes = hashmap_create();
    return r;
}





void match_route(http_message* message, int socket, AppContext* ctx)
{
    router* r = ctx->router;
    entryRoute* targetRoute = NULL;

    // Dynamic route matching
    for (int i = 0; i < r->dynamic_routes->count; i++) {

        entryRoute* tempRoute = r->dynamic_routes->array[i];

        if (regexec(
                tempRoute->reg,
                message->request->request_uri,
                0,
                NULL,
                0
            ) == 0)
        {
            puts("ROUTE REGEX MATCHED");

            if (strcmp(message->request->method, tempRoute->method) == 0) {
                puts("==== routes METHOD matched ====");
                targetRoute = tempRoute;
                break;
            }
        }
    }

    // Dynamic route found
    if (targetRoute != NULL) {
        targetRoute->handler(socket, message, ctx);
        return;
    }

    // Static route matching
    targetRoute = hashmap_get(
        ctx->router->static_routes,
        message->request->request_uri
    );

    if (targetRoute != NULL) {
        targetRoute->handler(socket, message, ctx);
        return;
    }

    // No route matched
    if (!is_api(message->request->request_uri)) {
        const char* error = "NOT FOUND";

        send_response(
            404,
            TEXT,
            strlen(error),
            socket,
            error
        );

        printf(
            "❌ No API route matched for: %s\n",
            message->request->request_uri
        );

        return;   
    }

    // Not an API route -> try static file
    render_static_file(socket, message, ctx);
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


void add_dynamic_route(router* router,char* method,char* pattern,route_handler_t handler){
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
    dynamic_array_push_elem(router->dynamic_routes,entry);
}

void add_static_route(router* router,char* method,char* uri,route_handler_t handler){
    if (!router || !method || !uri || !handler) return;
    //create entryroute
 	entryRoute* entry = malloc(sizeof(entryRoute));
	if(entry==NULL)return ;
    entry->pattern = strdup(uri);
    entry->handler = handler;
    entry->method = strdup(method);
    entry->reg = NULL;
    hashmap_insert(router->static_routes,uri,entry);
}

// add api routes here
void routes_init(router* router){
    puts("adding dynamic routes");
    add_dynamic_route(router,"GET","^/api/posts/([^/]+)",&get_postbyId_json); // /api/posts/id
    add_dynamic_route(router,"GET","^/posts/([^/]+)",&get_postbyId_html);
    puts("adding static routes");
    add_static_route(router,"GET","/api/posts",&postController_get_AllpostsPreview); // /api/posts
    add_static_route(router,"GET","/home",&render_home_page); ///home
    puts("routes added");
    }

     

    void destroy_router(router* router){
         int i = 0;
        for(i=0;i<router->dynamic_routes->count;i++){
            destroy_route(router->dynamic_routes->array[i]);
        }
        dynamic_array_destroy(router->dynamic_routes);
        router->dynamic_routes = NULL;
    }


