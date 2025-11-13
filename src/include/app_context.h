#ifndef APP_CONTEXT_H
#define APP_CONTEXT_H

#include <sqlite3.h>
#include "../../shared/logger/logger.h"
#include "../../shared/config/config.h"
typedef struct router router;
typedef struct AppContext{
    logger_t* logger;
    sqlite3* db;
    router* router;        
    BlogConfig* config;
}AppContext;

AppContext* app_context_create(void);
void app_context_destroy(AppContext* ctx);
int app_context_init(AppContext* ctx);  

#endif