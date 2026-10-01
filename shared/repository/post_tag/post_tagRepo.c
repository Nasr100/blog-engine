
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "post_tagRepo.h"
#include "../tag/tag.h"
#include "../post/post.h"
#include "../user/user.h"



static int callback_get_tagPosts(void *user_data, int argc, char **argv, char **azColName) {
    dynamic_array* ar = (dynamic_array*)user_data;
    int id = atoi(argv[0]);
    int is_deleted = atoi(argv[2]);
    tag* new_tag = create_tag(id,argv[1],is_deleted);
    dynamic_array_push_elem(ar,new_tag);
    return 0;
}

post_tagRepo* create_postTag_repo(database* database){
    if(database == NULL)return NULL;
    post_tagRepo* post_tagRepo = malloc(sizeof(post_tagRepo));
    if(post_tagRepo==NULL) return NULL;
    post_tagRepo->database = database;
    return post_tagRepo;
}

int add_tagToPost(post_tagRepo* post_tagRepo,int post_id,int tag_id){
    if(post_tagRepo == NULL) return 0;
    sqlite3_stmt *res;
    char* sql = "INSERT INTO tags_post (FK_tags_posts,FK_posts_tags)VALUES(?,?);";
    int rc = sqlite3_prepare_v2(post_tagRepo->database->db, sql, -1, &res, 0);
    if (rc == SQLITE_OK) {
	    sqlite3_bind_int(res, 1,post_id);	
	    sqlite3_bind_int(res, 2,tag_id);	
	    if (sqlite3_step(res) != SQLITE_DONE) {
	        printf("TAG_POST Insert failed: %s\n", sqlite3_errmsg(post_tagRepo->database->db));
            return -1;
	    } else {
	        printf("TAG_POST Insert successful!\n");
	    }
	}else {
	    fprintf(stderr, "Failed to execute insert TAG_POST statement: %s\n", sqlite3_errmsg(post_tagRepo->database->db));
	    sqlite3_finalize(res);
         return -1;    	
	}
    sqlite3_finalize(res);
    // database_commit(post_tagRepo->database);
    if (post_tagRepo->database->err_msg) {
        fprintf(stderr, "Transaction error: %s\n", post_tagRepo->database->err_msg);
        sqlite3_free(post_tagRepo->database->err_msg);
         return -1;
    }
    return 1;
}

int delete_tagFromPost(post_tagRepo* post_tagRepo,int post_id,int tag_id){
     if(post_tagRepo == NULL) return -1;
    sqlite3_stmt *res;
    char* sql = "DELETE FROM tags_post where (FK_tags_posts = ? AND FK_posts_tags = ?);";
    int rc = sqlite3_prepare_v2(post_tagRepo->database->db, sql, -1, &res, 0);
    if (rc == SQLITE_OK) {
	    sqlite3_bind_int(res, 1,post_id);	
	    sqlite3_bind_int(res, 2,tag_id);	
	    if (sqlite3_step(res) != SQLITE_DONE) {
	        printf("TAG_POST DELETE failed: %s\n", sqlite3_errmsg(post_tagRepo->database->db));
            return 0;
	    } else {
	        printf("TAG_POST DELETE successful!\n");
	    }
	}else {
	    fprintf(stderr, "Failed to execute delete TAG_POST statement: %s\n", sqlite3_errmsg(post_tagRepo->database->db));
	    sqlite3_finalize(res);
         return -1;    	
	}
    sqlite3_finalize(res);
    if (post_tagRepo->database->err_msg) {
        fprintf(stderr, "Transaction error: %s\n", post_tagRepo->database->err_msg);
        sqlite3_free(post_tagRepo->database->err_msg);
         return -1;
    }
    return 1;
}

dynamic_array* get_post_tags(post_tagRepo* post_tagRepo,int post_id){
      if(post_tagRepo == NULL) return -1;
      sqlite3_stmt *res;
    char* sql = "SELECT tags.tag_id,tags.name FROM posts,tags INNER JOIN tags_post ON tags_post.FK_posts_tags = ? WHERE tags.is_deleted = 0";
     int rc = sqlite3_prepare_v2(post_tagRepo->database->db, sql, -1, &res, 0);
     if (rc != SQLITE_OK){
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(post_tagRepo->database->db));
        return NULL;
    }
      sqlite3_bind_int(res, 1,post_id);
      dynamic_array* tags = dynamic_array_create();
      while(sqlite3_step(res)== SQLITE_ROW){
        int tag_id = sqlite3_column_int(res,0);
        const unsigned char *name = sqlite3_column_text(res,1);
        tag* t = create_tag(tag_id,name,0);
        if(t) dynamic_array_push_elem(tags,t);
      }
      sqlite3_finalize(res);
      puts("tags selected succefully for a post");
      return tags;
}

