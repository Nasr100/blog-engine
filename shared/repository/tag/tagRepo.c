
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tagRepo.h"


static int callback_getAllTags(void *user_data, int argc, char **argv, char **azColName) {
    dynamic_array* ar = (dynamic_array*)user_data;
    int id = atoi(argv[0]);
    int is_deleted = atoi(argv[2]);
    tag* new_tag = create_tag(id,argv[1],is_deleted);
    dynamic_array_push_elem(ar,new_tag);
    return 0;
}

tagRepo* create_tagRepo(database* database){
    if(database == NULL)return NULL;
    tagRepo* tagRepo = malloc(sizeof(tagRepo));
    if(tagRepo==NULL) return NULL;
    tagRepo->database = database;
    return tagRepo;
}

tag* insert_tag(tagRepo* tagRepo,tag* new_tag){
    sqlite3_stmt *res;
	char* sql = "INSERT INTO tags (name) VALUES(?);";
    int rc = sqlite3_prepare_v2(tagRepo->database->db, sql, -1, &res, 0);
    if (rc == SQLITE_OK) {
	    sqlite3_bind_text(res, 1,new_tag->name,-1,SQLITE_TRANSIENT);	
	    if (sqlite3_step(res) != SQLITE_DONE) {
	        printf("Insert failed: %s\n", sqlite3_errmsg(tagRepo->database->db));
            return NULL;
	    } else {
	        printf("Insert successful!\n");
	        int new_tag_id = sqlite3_last_insert_rowid(tagRepo->database->db);
            new_tag->tag_id = new_tag_id;
	    }
	}else{
	    fprintf(stderr, "Failed to execute insert tag statement: %s\n", sqlite3_errmsg(tagRepo->database->db));
	    sqlite3_finalize(res);
        return NULL;    	
	}
    sqlite3_finalize(res);
    if (tagRepo->database->err_msg) {
        fprintf(stderr, "Transaction error: %s\n", tagRepo->database->err_msg);
        sqlite3_free(tagRepo->database->err_msg);
         return NULL;
    }
    return new_tag;
}

tag* select_tagById(tagRepo* tagRepo,int tag_id){
    tag* tag = NULL;
    sqlite3_stmt *res;
    const char *sql = "SELECT tag_id, name, is_deleted,  FROM tags WHERE tag_id = ?";
     int rc = sqlite3_prepare_v2(tagRepo->database->db, sql, -1, &res, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(tagRepo->database->db));
        return NULL;
    }
    sqlite3_bind_int(res, 1, tag_id);
    int step = sqlite3_step(res);
    
    if (step == SQLITE_ROW) {
        tag = create_tag(atoi(sqlite3_column_text(res, 0)),sqlite3_column_text(res, 1),atoi(sqlite3_column_text(res, 2)));
    }else{
        return NULL;
    }
    sqlite3_finalize(res);
    return tag;
}

tag* select_tagByName(tagRepo* tagRepo,char* name){
    tag* tag = NULL;
    sqlite3_stmt *res;
    const char *sql = "SELECT tag_id, name, is_deleted,  FROM tags WHERE name = ?";
    int rc = sqlite3_prepare_v2(tagRepo->database->db, sql, -1, &res, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(tagRepo->database->db));
        return NULL;
    }
    sqlite3_bind_text(res, 1, name,-1,SQLITE_TRANSIENT);
    int step = sqlite3_step(res);
    if (step == SQLITE_ROW) {
        tag = create_tag(atoi(sqlite3_column_text(res, 0)),sqlite3_column_text(res, 1),atoi(sqlite3_column_text(res, 2)));
    }else{
        return NULL;
    }
    sqlite3_finalize(res);
    return tag;
}


tag* update_tag(tagRepo* tagRepo ,tag* new_tag){
    // tag* tag = NULL;
    sqlite3_stmt *res;
    const char *sql = "UPDATE tags SET name = ?  WHERE tag_id = ?";
    
    int rc = sqlite3_prepare_v2(tagRepo->database->db, sql, -1, &res, 0);
     if (rc == SQLITE_OK){
        sqlite3_bind_text(res, 1,new_tag->name,-1,SQLITE_TRANSIENT);	
        sqlite3_bind_int(res, 2,new_tag->tag_id);	
	    if (sqlite3_step(res) != SQLITE_DONE) {
	        printf("TAG UPDATE failed: %s\n", sqlite3_errmsg(tagRepo->database->db));
            return NULL;
	    } else {
	        printf("TAG UPADTE successful!\n");
	        
	    }
     }else {
	    fprintf(stderr, "Failed to execute update tag statement: %s\n", sqlite3_errmsg(tagRepo->database->db));
	    sqlite3_finalize(res);
         return NULL;  	
	}
    sqlite3_finalize(res);
    if (tagRepo->database->err_msg) {
        fprintf(stderr, "UPDATE GET Transaction error: %s\n", tagRepo->database->err_msg);
        sqlite3_free(tagRepo->database->err_msg);
        return NULL;
    }
    return new_tag;
}

int delete_tagById(tagRepo* tagRepo,int tag_id){
    sqlite3_stmt *res;
    const char *sql = "UPDATE tags SET is_deleted = 1  WHERE tag_id = ?";
    int rc = sqlite3_prepare_v2(tagRepo->database->db, sql, -1, &res, 0);
     if (rc == SQLITE_OK){
        sqlite3_bind_int(res, 1,tag_id);	
	    if (sqlite3_step(res) != SQLITE_DONE) {
	        printf("TAG DELETE failed: %s\n", sqlite3_errmsg(tagRepo->database->db));
            return -1;
	    } else {
	        printf("TAG DELETE successful!\n");
	        
	    }
     }else {
	    fprintf(stderr, "Failed to execute DELETE tag statement: %s\n", sqlite3_errmsg(tagRepo->database->db));
	    sqlite3_finalize(res);
        return -1;  	
	}
    sqlite3_finalize(res);
    if (tagRepo->database->err_msg) {
        fprintf(stderr, "DELETE GET Transaction error: %s\n", tagRepo->database->err_msg);
        sqlite3_free(tagRepo->database->err_msg);
         return -1;
    }
    return 1;
}

dynamic_array* select_allTags(tagRepo* tagRepo){
    dynamic_array* tags_array = dynamic_array_create();
 	char *sql = "SELECT tag_id,name,is_deleted FROM tags";
    int rc = sqlite3_exec(tagRepo->database->db,sql,callback_getAllTags,tags_array,&tagRepo->database->err_msg);
    if (rc != SQLITE_OK){
        puts("Failed to select ALL TAGS data\n");
        printf("SQL error: %s\n",tagRepo->database->err_msg);

        sqlite3_free(tagRepo->database->err_msg);
        dynamic_array_destroy(tags_array);
        return NULL;
    } 
    return tags_array;
}

dynamic_array* find_post_tags(tagRepo* tagRepo,int post_id){
       dynamic_array* tags_array = dynamic_array_create();
    sqlite3_stmt *res;
    char *sql = "SELECT tags.tag_id, tags.name, tags.is_deleted FROM tags INNER JOIN post_tags ON post_tags.tag_id = tags.tag_id WHERE post_tags.post_id = ?";
    
    int rc = sqlite3_prepare_v2(tagRepo->database->db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(tagRepo->database->db));
        dynamic_array_destroy(tags_array);
        return NULL;
    }
    sqlite3_bind_int(res, 1, post_id);
     while ((rc = sqlite3_step(res)) == SQLITE_ROW) {
        tag* t = create_tag(sqlite3_column_int(res, 0),(char*)sqlite3_column_text(res, 1),sqlite3_column_int(res, 2));
        dynamic_array_push_elem(tags_array, t);
    }
    if (rc != SQLITE_DONE) {
        printf("Query failed: %s\n", sqlite3_errmsg(tagRepo->database->db));
    }
    
    sqlite3_finalize(res);
    return tags_array;
}