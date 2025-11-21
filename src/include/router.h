#ifndef ROUTER_H

#define ROUTER_H
#include "types.h"
#include "../../shared/utils/hashmap/hashmap.h" 
typedef struct router{
	hashmap_t* get_routes;
	hashmap_t* post_routes;
	hashmap_t* delete_routes;
	hashmap_t* put_routes;
}router;

void match_route(http_message* message,int socket,AppContext* ctx);
router* create_router();
void routes_api_init(router* router);
void destroy_router(router* router);

#endif
