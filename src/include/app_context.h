#ifndef APP_CONTEXT_H
#define APP_CONTEXT_H

#include <sqlite3.h>
#include "../../shared/logger/logger.h"
#include "../../shared/config/config.h"
#include "../service/post/postService.h"
#include "../service/tag/tagService.h"
#include "../../shared/db/database.h"  
#include "../include/router.h"
#include "types.h"
typedef struct router router;
typedef struct AppContext{
    logger_t* logger;
    database* database;
    router* router;
    postService* postService;        
    tagService* tagService;        
    BlogConfig* config;
}AppContext;

AppContext* app_context_create(void);
void app_context_destroy(AppContext* ctx);
int app_context_init(AppContext* ctx);  
logger_t* app_context_get_logger(AppContext* ctx);
router* app_context_get_router(AppContext* ctx);
#endif