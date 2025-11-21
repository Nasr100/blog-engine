#include<stdio.h>
#include <string.h>
#include<stdlib.h>
#include "../include/app_context.h"
#include "../../shared/utils/hashmap/hashmap.h"
#include "../../shared/repository/post/postRepo.h"
#include "../../shared/repository/tag/tagRepo.h"
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
    
    //init logger

    ctx->logger = create_logger(config_get_log_file(ctx->config));
    
    //database coonnect
    ctx->database = database_connect((char*)config_get_db_path(ctx->config));
    if (ctx->database == NULL) {
        return -1;
    }
    
    //init router
    ctx->router = create_router();
     
    if (!ctx->router) {
        log_error(ctx->logger, "Failed to create router");
        return -1;
    }
    
    routes_init(ctx->router);

    


    //init repositories
    postRepo* postRepo = create_post_repo(ctx->database);
    if(postRepo == NULL) return -1;
    tagRepo* tagRepo = create_tagRepo(ctx->database);
    if(tagRepo == NULL) return -1;

    //init services
    ctx->postService = postService_create(postRepo,tagRepo);
    if(ctx->postService == NULL) return -1;

    ctx->tagService = tagService_create(tagRepo);
    if(ctx->tagService == NULL) return -1;
    log_info(ctx->logger, "App context initialized successfully!");
    return 0;
}

void app_context_destroy(AppContext* ctx) {
    if (!ctx) return;
    
    if (ctx->router) destroy_router((ctx->router)); 
    ctx->router = NULL;
    if (ctx->database) database_close(ctx->database);
    ctx->database= NULL;
    if (ctx->logger) destroy_logger(ctx->logger);
    ctx->logger = NULL;
    if (ctx->config) config_free(ctx->config);
    ctx->config = NULL;
    //vaalid free
    if(ctx->postService) postService_destroy(&(ctx->postService));
    if(ctx->tagService) tagService_destroy(&(ctx->tagService));
    free(ctx);
}

logger_t* app_context_get_logger(AppContext* ctx) {
    return ctx->logger;
}

router* app_context_get_router(AppContext* ctx) {
    return ctx->router;
}