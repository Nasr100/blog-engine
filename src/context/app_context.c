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
    
    //init  router
    puts("router init start");
    ctx->router = NULL;
    ctx->router =  create_router();
    puts("router created");
    //add dynamic routes to router
    puts("routes init started");
    routes_init(ctx->router);
     puts("routes init finished");
     
 postRepo* postRepo = create_post_repo(ctx->database);
    printf("🔍 Created postRepo: %p\n", (void*)postRepo);
    if(postRepo == NULL) return NULL;
    
    tagRepo* tagRepo = create_tagRepo(ctx->database);
    printf("🔍 Created tagRepo: %p\n", (void*)tagRepo);
    if(tagRepo == NULL) return NULL;
post_tagRepo* post_tagRepo = create_postTag_repo(ctx->database);
printf("🔍 Created post_tagRepo: %p\n", (void*)post_tagRepo);
if(post_tagRepo == NULL) return NULL;
    //init services
    ctx->postService = postService_create(postRepo, tagRepo,post_tagRepo);
    printf("🔍 Created postService: %p\n", (void*)ctx->postService);
    printf("🔍 postService->postRepo: %p\n", (void*)ctx->postService->postRepo);
    printf("🔍 postService->tagRepo: %p\n", (void*)ctx->postService->tagRepo);
    if(ctx->postService == NULL) return NULL;

    // ctx->tagService = tagService_create(tagRepo);
    // printf("🔍 Created tagService: %p\n", (void*)ctx->tagService);
    // printf("🔍 tagService->tagRepo: %p\n", (void*)ctx->tagService->tagRepo);
    // if(ctx->tagService == NULL) return NULL;
    ctx->tagService = NULL;
    log_info(ctx->logger, "App context initialized successfully!");
    return ctx;
}

void app_context_destroy(AppContext* ctx) {
    printf("🚨 app_context_destroy called for: %p\n", (void*)ctx);
    if (!ctx) {
        printf("🚨 ctx is NULL, returning early\n");
        return;
    }
    
    printf("🔧 Destroying services...\n");
    if(ctx->postService){
        printf("🗑️  Destroying postService at: %p\n", (void*)ctx->postService);
        postService_destroy(&(ctx->postService));
        ctx->postService = NULL;
        printf("✅ postService destroyed\n");
    }
    
    if(ctx->tagService){
        printf("🗑️  Destroying tagService at: %p\n", (void*)ctx->tagService);
        tagService_destroy(&(ctx->tagService));
        ctx->tagService = NULL;
        printf("✅ tagService destroyed\n");
    }
    
    printf("🔧 Destroying router...\n");
    if (ctx->router){
        printf("🗑️  Destroying router at: %p\n", (void*)ctx->router);
        destroy_router(ctx->router); 
        ctx->router = NULL;
        printf("✅ router destroyed\n");
    }

    printf("🔧 Closing database...\n");
    if (ctx->database){
        printf("🗑️  Closing database at: %p\n", (void*)ctx->database);
        database_close(ctx->database);
        ctx->database = NULL;
        printf("✅ database closed\n");
    }
    
    printf("🔧 Destroying logger...\n");
    if (ctx->logger){
        printf("🗑️  Destroying logger at: %p\n", (void*)ctx->logger);
        destroy_logger(ctx->logger);
        ctx->logger = NULL;
        printf("✅ logger destroyed\n");
    }
    
    printf("🔧 Freeing config...\n");
    if (ctx->config){
        printf("🗑️  Freeing config at: %p\n", (void*)ctx->config);
        config_free(ctx->config);
        ctx->config = NULL;
        printf("✅ config freed\n");
    }
    
    printf("🔧 Freeing AppContext struct...\n");
    free(ctx);
    printf("✅ AppContext freed - DESTRUCTION COMPLETE\n");
}

logger_t* app_context_get_logger(AppContext* ctx) {
    return ctx->logger;
}

router* app_context_get_router(AppContext* ctx) {
    return ctx->router;
}


