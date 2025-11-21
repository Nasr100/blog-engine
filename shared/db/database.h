#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include "../../shared/libs/sqlite3.h"

 typedef struct database{
    sqlite3* db;
    char* db_path;
    char* err_msg;
}database;
// Database connection
database* database_connect(char* db_path);
void database_close(database* database);
// transaction
int database_begin_transaction(database* database);
int database_commit(database* database);
int database_rollback(database* database);


#endif