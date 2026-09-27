#ifndef ROUTER_H

#define ROUTER_H
#include "types.h"
#include "../../shared/utils/hashmap/hashmap.h" 
// #include <regex.h>
typedef void (*route_handler_t)(int socket, http_message* message, AppContext* ctx);

typedef struct entryRoute{
	route_handler_t handler;
	char* pattern;
	// dynamic_array* params;
	int is_dynamic;
}entryRoute;
typedef struct router{
	hashmap_t* get_routes;
	hashmap_t* post_routes;
	hashmap_t* put_routes;
	hashmap_t* delete_routes;
}router;

// void add_route(router* router,char* method,char* pattern,route_handler_t handler);
void match_route(http_message* message,int socket,AppContext* ctx);
router* create_router();
// void routes_api_init(router* router);
// void destroy_router(router* router);

#endif
