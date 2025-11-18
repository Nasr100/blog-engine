#include<stdio.h>
#include <string.h>
#include<stdlib.h>
#include "../include/app_context.h"
#include "../../shared/db/database.h"  
#include "../include/blog.h"
#include "../../shared/utils/hashmap/hashmap.h"
#include "../include/router.h"
AppContext* app_context_create(void) {
    AppContext* ctx = malloc(sizeof(AppContext));
    if (!ctx) return NULL;
    
    memset(ctx, 0, sizeof(AppContext));
    
    //load config
    ctx->config = config_create();
    config_load(ctx->config);
    config_expand_variables(ctx->config);

    return ctx;
}

int app_context_init(AppContext* ctx) {
    if (!ctx) return -1;
    
    
    ctx->logger = create_logger(config_get_log_file(ctx->config));
    
    //stabase coonnect
    if (database_connect(&ctx->db, config_get_db_path(ctx->config), ctx->logger) != 0) {
        return -1;
    }
    
    
    ctx->router = create_router();
     
    if (!ctx->router) {
        log_error(ctx->logger, "Failed to create router");
        return -1;
    }
    ctx->router->get_routes = hashmap_create();
    ctx->router->post_routes = hashmap_create();

     if (!ctx->router->get_routes || !ctx->router->post_routes) {
        if (ctx->router->get_routes) hashmap_destroy(ctx->router->get_routes);
        if (ctx->router->post_routes) hashmap_destroy(ctx->router->post_routes);
        destroy_router(ctx->router);
        log_error(ctx->logger, "Failed to create hashmaps router");
        return -1;
    }
    //get routes
    hashmap_insert(ctx->router->get_routes, "/home", render_home);
    hashmap_insert(ctx->router->get_routes, "/post/*", render_article);

    //post routes
    
    log_info(ctx->logger, "App context initialized successfully!");
    return 0;
}

void app_context_destroy(AppContext* ctx) {
    if (!ctx) return;
    
    if (ctx->router) destroy_router(ctx->router); 
    if (ctx->db) database_close(ctx->db);
    if (ctx->logger) destroy_logger(ctx->logger);
    if (ctx->config) config_free(ctx->config);
    free(ctx);
}