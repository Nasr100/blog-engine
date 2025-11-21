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

database* database_connect(char* db_path){
    database* database = create_dbConnection(db_path);
	int rc =  sqlite3_open(database->db_path,&(database->db));
	 
    if (rc != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(database->db));
        sqlite3_close(database->db);
        return NULL;
    }   
    puts("DB connection SUCCESS");
    return database;
}


int database_begin_transaction(database* database){
    int rc= sqlite3_exec(database->db, "BEGIN TRANSACTION", 0, 0, &(database->err_msg));
     if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", database->err_msg);
        sqlite3_free(database->err_msg);
    }
    return rc;
}

int database_commit(database* database){
    int rc = sqlite3_exec(database->db, "COMMIT", 0, 0, &(database->err_msg));
     if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", database->err_msg);
        sqlite3_free(database->err_msg);
    }
    return rc;
}
int database_rollback(database* database){
    int rc = sqlite3_exec(database->db, "ROLLBACK;", 0, 0, &database->err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", database->err_msg);
        sqlite3_free(database->err_msg);
    }
    return rc;
   

}
void destroy_database(database** database_ptr){
    if(*database_ptr==NULL) return;
    database*database = *database_ptr;
    if(database->db_path)free(database->db_path);
    database->db_path = NULL;
   if(database) free(database);
   database = NULL;
}
void database_close(database* database){
    sqlite3_close(database->db);
    database->db = NULL;
    destroy_database(&database);
}
