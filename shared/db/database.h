#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include "../../shared/libs/sqlite3.h"
#include "blog_db.h"
#include "../logger/logger.h"


// Database connection
int database_connect(sqlite3** db, const char* db_path, logger_t* logger);
void database_close(sqlite3* db);

// Blog operations
article_array* get_all_blogs(sqlite3* db, logger_t* logger);
article* get_blog_by_slug(sqlite3* db, char* slug);
tag_array* get_all_tags(sqlite3* db, logger_t* logger);

// Import tool operations (to add later)
// int insert_blog(sqlite3* db, article* blog, logger_t* logger);
// int update_blog(sqlite3* db, article* blog, logger_t* logger);

#endif