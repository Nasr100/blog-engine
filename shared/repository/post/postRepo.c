
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "postRepo.h"


postRepo* create_post_repo(database* database){
    if(database == NULL)return NULL;
    postRepo* postRepo = malloc(sizeof(postRepo));
    if(postRepo==NULL) return NULL;
    postRepo->database = database;
    return postRepo;
}

post* insert_post(postRepo* postRepo,post* new_post){
    sqlite3_stmt *res;
	char* sql = "INSERT INTO posts (title,slug,body,author,date,status) VALUES(?,?,?,?,?,?);";
    int rc = sqlite3_prepare_v2(postRepo->database->db, sql, -1, &res, 0);
    if (rc == SQLITE_OK) {
	    sqlite3_bind_text(res, 1,new_post->title,-1,SQLITE_TRANSIENT);	
	    sqlite3_bind_text(res, 2,new_post->slug,-1,SQLITE_TRANSIENT);	
	    sqlite3_bind_text(res, 3,new_post->body,-1,SQLITE_TRANSIENT);	
	    sqlite3_bind_text(res, 4,new_post->author,-1,SQLITE_TRANSIENT);	
	    sqlite3_bind_text(res, 5,new_post->date,-1,SQLITE_TRANSIENT);	
	    sqlite3_bind_text(res, 6,new_post->status,-1,SQLITE_TRANSIENT);	
	    if (sqlite3_step(res) != SQLITE_DONE) {
	        printf("Insert failed: %s\n", sqlite3_errmsg(postRepo->database->db));
            return NULL;
	    } else {
	        printf("Insert successful!\n");
	        int new_post_id = sqlite3_last_insert_rowid(postRepo->database->db);
            new_post->post_id = new_post_id;
	    }
	}else {
	    fprintf(stderr, "Failed to execute insert tag statement: %s\n", sqlite3_errmsg(postRepo->database->db));
	    sqlite3_finalize(res);
         return NULL;
	        	
	}
    sqlite3_finalize(res);
    if (postRepo->database->err_msg) {
        fprintf(stderr, "Transaction error: %s\n", postRepo->database->err_msg);
        sqlite3_free(postRepo->database->err_msg);
         return NULL;
    }
    return new_post;
}

dynamic_array* select_allPosts_preview(postRepo* postRepo){
    sqlite3_stmt *res;
    dynamic_array* posts_array = NULL;
     char* sql = "SELECT posts.post_id,posts.title,posts.slug,posts.author,posts.date,posts.likes,posts.views,users.user_id,users.username,users.is_admin FROM posts  INNER JOIN users ON  users.user_id = posts.FK_posts_users where posts.status=\"publish\" AND posts.is_deleted = 0 AND users.is_deleted=0 ORDER BY posts.date DESC;";
     int rc = sqlite3_prepare_v2(postRepo->database->db, sql, -1, &res, 0);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(postRepo->database->db));
        return NULL;
    }
    if(sqlite3_step(res)== SQLITE_ROW){
            int post_id = sqlite3_column_int(res, 0);
	        const unsigned char *title = sqlite3_column_text(res, 1);
	        const unsigned char *slug = sqlite3_column_text(res, 2);
	        const unsigned char *author = sqlite3_column_text(res, 3);
	        const unsigned char *date = sqlite3_column_text(res, 4);
	        int likes = sqlite3_column_int(res,5);
	        int views = sqlite3_column_int(res,6);
	        int user_id = sqlite3_column_int(res,7);
	        const unsigned char *username = sqlite3_column_text(res, 8);
            int is_admin = sqlite3_column_int(res,9);
            user* u = create_user(user_id,NULL,NULL,NULL,NULL,username,-1,' ',0,is_admin);
            post* p = create_post(post_id,title,slug,NULL,author,date,"publish",likes,views,0,user_id,u,NULL);
            dynamic_array_push_elem(posts_array,p);
    }
    return posts_array;
}


post* select_PostById(postRepo* postRepo,int post_id){
    sqlite3_stmt *res;
    post* p = NULL;
    char* sql = "SELECT posts.post_id,posts.title,posts.slug,posts.body,posts.author,posts.date,posts.likes,posts.views,users.user_id,users.username,users.is_admin FROM posts INNER JOIN users ON  users.user_id = posts.FK_posts_users where posts.post_id = ? AND posts.status=\"publish\" AND posts.is_deleted = 0 AND users.is_deleted=0 ORDER BY posts.date DESC;";
    int rc = sqlite3_prepare_v2(postRepo->database->db, sql, -1, &res, 0);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(postRepo->database->db));
        return NULL;
    }
    sqlite3_bind_int(res, 1,post_id);	
    if(sqlite3_step(res)== SQLITE_ROW){
            int post_id = sqlite3_column_int(res, 0);
	        const unsigned char *title = sqlite3_column_text(res, 1);
	        const unsigned char *slug = sqlite3_column_text(res, 2);
	        const unsigned char *body = sqlite3_column_text(res, 3);
	        const unsigned char *author = sqlite3_column_text(res, 4);
	        const unsigned char *date = sqlite3_column_text(res, 5);
	        int likes = sqlite3_column_int(res, 6);
	        int views = sqlite3_column_int(res, 7);

	        int user_id = sqlite3_column_int(res, 8);
	        const unsigned char *username = sqlite3_column_text(res, 9);
            int is_admin = sqlite3_column_int(res, 10);

            user* u = create_user(user_id,NULL,NULL,NULL,NULL,username,-1,' ',0,is_admin);
            p = create_post(post_id,title,slug,body,author,date,"publish",likes,views,0,user_id,u,NULL);
    }
    return p;
}



dynamic_array* get_Posts_byTagId(postRepo* postRepo,int tag_id){
    sqlite3_stmt *res;
    dynamic_array* posts_array = dynamic_array_create();
 	char *sql = "SELECT posts.post_id,posts.title,posts.slug,posts.status,posts.author,posts.date,posts.likes,posts.views,users.user_id,users.username,users.is_admin FROM posts,users  INNER JOIN tags_post ON tags_post.FK_tags_posts=post.post_id where tags.tag_id = ? AND users.user_id = posts.FK_posts_users AND posts.status=\"publish\" AND posts.is_deleted=0 AND users.is_deleted=0 ORDER BY posts.date DESC;";
    int rc = sqlite3_prepare_v2(postRepo->database->db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(postRepo->database->db));
        return NULL;
    }
    sqlite3_bind_int(res, 1,tag_id);	
    sqlite3_bind_int(res, 2,tag_id);	
     
    while ((rc=sqlite3_step(res)) == SQLITE_ROW) {
	        int post_id = sqlite3_column_int(res, 0);
	        const unsigned char *title = sqlite3_column_text(res, 1);
	        const unsigned char *slug = sqlite3_column_text(res, 2);
	        const unsigned char *author = sqlite3_column_text(res, 3);
	        const unsigned char *date = sqlite3_column_text(res, 4);
	        int likes = sqlite3_column_int(res, 5);
	        int views = sqlite3_column_int(res, 6);

	        int user_id = sqlite3_column_int(res, 7);
	        const unsigned char *username = sqlite3_column_text(res, 8);
            int is_admin = sqlite3_column_int(res, 9);
            user* u = create_user(user_id,NULL,NULL,NULL,NULL,username,0,' ',0,is_admin);
            post* p = create_post(post_id,title,slug,NULL,author,date,"publish",likes,views,0,u->user_id,u,NULL);
            dynamic_array_push_elem(posts_array,p);
	    } 
    return posts_array;
}



