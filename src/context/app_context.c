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
    if(ctx->config == NULL){
        puts("CONFIG NOT CREATED");
        return NULL;
    }
    if(config_load(ctx->config)==0){
        puts("CONFIG LOADED SUCCEFULLY");
    }else{
        puts("CONFIG FAILD TO LOAD");
    }
    config_expand_variables(ctx->config);

    return ctx;
}

AppContext* app_context_init(AppContext* ctx) {
    if (!ctx) return NULL;
    
    //init logger

    ctx->logger = create_logger(config_get_log_file(ctx->config));
    if(ctx->logger == NULL){
        puts("logger not created ");
        return NULL;
    }
    
    //database coonnect
    ctx->database = database_connect((char*)config_get_db_path(ctx->config));
    if (ctx->database == NULL) {
        return NULL;
    }
    
    //init  routers
    ctx->api_router = create_router();
     
    if (!ctx->api_router) {
        log_error(ctx->logger, "Failed to create api router");
        return NULL;
    }
    routes_api_init(ctx->api_router);



    


    //init repositories
    postRepo* postRepo = create_post_repo(ctx->database);
    if(postRepo == NULL) return NULL;
    tagRepo* tagRepo = create_tagRepo(ctx->database);
    if(tagRepo == NULL) return NULL;

    //init services
    ctx->postService = postService_create(postRepo,tagRepo);
    if(ctx->postService == NULL) return NULL;

    ctx->tagService = tagService_create(tagRepo);
    if(ctx->tagService == NULL) return NULL;
    log_info(ctx->logger, "App context initialized successfully!");
    return ctx;
}

void app_context_destroy(AppContext* ctx) {
    if (!ctx) return;
    
    if (ctx->api_router) destroy_router((ctx->api_router)); 
    ctx->api_router = NULL;

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
    return ctx->api_router;
}
