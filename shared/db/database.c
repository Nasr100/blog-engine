#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include "database.h"
#include "../utils/helpers.h"


static database* create_dbConnection(char* db_path){
   database* db = malloc(sizeof(struct database));
   db->db_path = my_strdup(db_path);
   db->db = NULL;
   db->err_msg = NULL;
   return db;
}

int database_connect(char* db_path){
    database* database = create_dbConnection(db_path);
	int rc =  sqlite3_open(database->db_path,&(database->db));
	 
    if (rc != SQLITE_OK) {
        pritnf("Cannot open database: %s\n", sqlite3_errmsg(database->db));
        sqlite3_close(database->db);
        return 1;
    }   
    puts("DB connection SUCCESS");
    return 0;
}


int database_begin_transaction(database* database){
    return sqlite3_exec(database->db, "BEGIN TRANSACTION", 0, 0, &(database->err_msg));
}

int database_commit(database* database){
    return sqlite3_exec(database->db, "COMMIT", 0, 0, &(database->err_msg));
}

void destroy_database(database* database){
   if(database) free(database);
}
void database_close(database* database){
    sqlite3_close(database->db);
    destroy_database(database);
}
