#ifndef ROUTER_H

#define ROUTER_H
#include "request_parser.h"
#include "app_context.h"

typedef struct router{
	hashmap_t* get_routes;
	hashmap_t* post_routes;
}router;

void match_route(http_message* message,int socket,AppContext* ctx);
router* create_router();
void destroy_router(router* router);

#endif
