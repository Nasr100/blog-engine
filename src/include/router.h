#ifndef ROUTER_H

#define ROUTER_H
#include "types.h"
#include "../../shared/utils/hashmap/hashmap.h" 
#include "../../shared/utils/dynamic_array/dynamic_array.h" 
#include <regex.h>
typedef int (*route_handler_t)(int socket, http_message* message, AppContext* ctx);

typedef struct entryRoute{
	route_handler_t handler;
	char* pattern;
	regex_t* reg;
	char* method;
	// dynamic_array* params;
	// int is_dynamic;
}entryRoute;
typedef struct router{
	dynamic_array* routes;
	// hashmap_t* get_routes;
}router;

void add_route(router* router,char* method,char* pattern,route_handler_t handler);
void match_route(http_message* message,int socket,AppContext* ctx);
router* create_router();
void routes_api_init(router* router);
void destroy_router(router* router);

#endif
