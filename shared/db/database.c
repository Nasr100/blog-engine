#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include "database.h"

void insert_article(article_array* article_array,char* slug,char* title,char* author,char* body,char* created_at);

void insert_tag(tag_array* tag_array,int id,char* tagname);

static int get_blogs_callback(void *user_data, int argc, char **argv, char **azColName) {
      if (argc < 5) {
        fprintf(stderr, "Error: Expected 5 columns, got %d\n", argc);
        return 0; 
    	}
   	article_array* ar = (article_array*)user_data;
    insert_article(ar,argv[0],argv[1],argv[2],argv[3],argv[4]);
    return 0;
}

static int get_tags_callback(void *user_data, int argc, char **argv, char **azColName) {
   tag_array* ar = (tag_array*)user_data;
   int id = atoi(argv[0]);
    insert_tag(ar,id,argv[1]);
    return 0;
}

int database_connect(sqlite3** db, const char* db_path, logger_t* logger){
	int rc =  sqlite3_open(db_path, db);
	 
    if (rc != SQLITE_OK) {
        
        log_error(logger,"Cannot open database: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);
        
        return 1;
    }   
    puts("DB connection SUCCESS");
    return 0;
}


void insert_tag(tag_array* tag_array,int id,char* tagname){
	tag* tag = malloc(sizeof(tag));
	if(tag == NULL){
		puts("COULDNT ALLOCATE ARTICLE MEMORY");
		return ;
	}
	tag->id = id;
	strcpy(tag->tag,tagname);

	
	if(tag_array->count == tag_array->size -1 ){
		tag_array->size = tag_array->size * 2;
		tag_array->tags = realloc(tag_array->tags,tag_array->size);
	}
	
	tag_array->tags[tag_array->count] = tag;
	tag_array->count = tag_array->count + 1; 
}

void insert_article(article_array* article_array,char* slug,char* title,char* author,char* body,char* created_at){
	article* ar = malloc(sizeof(article));
	if(ar == NULL){
		puts("COULDNT ALLOCATE ARTICLE MEMORY");
		return ;
	}

	strcpy(ar->slug,slug);
	strcpy(ar->title,title);
	strcpy(ar->author,author);
	ar->body = calloc(strlen(body)+1,sizeof(char));
	strcpy(ar->body,body);
	
	strcpy(ar->created_date,created_at);
	
	ar->slug[sizeof(ar->slug) - 1] = '\0';
    ar->title[sizeof(ar->title) - 1] = '\0';
    ar->author[sizeof(ar->author) - 1] = '\0';
    ar->body[strlen(ar->body)+1] = '\0';
    ar->created_date[sizeof(ar->created_date) - 1] = '\0';
    
	if(article_array->count >= article_array->size -1 ){
		int new_size = article_array->size * 2;
		
		article** new_articles = realloc(article_array->articles, new_size * sizeof(article*));
        if(new_articles == NULL) {
            puts( "Failed to reallocate article array");
            free(ar);
            return;
        }
        article_array->articles = new_articles;
        article_array->size = new_size;
	}
	
	article_array->articles[article_array->count] = ar;
	article_array->count = article_array->count + 1; 
}

article_array* get_all_blogs(sqlite3* DB,logger_t* logger){
	article_array* ar = create_article_array();
	char* err_msg = 0;
 	char *sql = "SELECT * FROM articles ORDER BY created_date DESC";
	int rc = sqlite3_exec(DB,sql,get_blogs_callback,ar,&err_msg);
	
	if (rc != SQLITE_OK ) {
        
        log_error(logger,"Failed to select BLOGS data\n");
        log_error(logger,"SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(DB);
        free_article_array(ar);
        
        return NULL;
    } 

	return ar;
}

article* get_blog_by_slug(sqlite3* db, char* slug) {
    article* a = NULL;
    sqlite3_stmt *res;

    const char *sql = "SELECT slug, title, author, body, created_date FROM articles WHERE slug = ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_text(res, 1, slug, -1, SQLITE_TRANSIENT);

    if (sqlite3_step(res) == SQLITE_ROW) {
        a = malloc(sizeof(article));
        if (!a) {
            fprintf(stderr, "Out of memory\n");
            sqlite3_finalize(res);
            return NULL;
        }

        strcpy(a->slug, (const char*)sqlite3_column_text(res, 0));
        strcpy(a->title, (const char*)sqlite3_column_text(res, 1));
        strcpy(a->author, (const char*)sqlite3_column_text(res, 2));

        const unsigned char* body_text = sqlite3_column_text(res, 3);
        if (body_text) {
            size_t body_len = strlen((const char*)body_text);
            a->body = malloc(body_len + 1);
            strcpy(a->body, (const char*)body_text);
        } else {
            a->body = strdup("");
        }

        strcpy(a->created_date, (const char*)sqlite3_column_text(res, 4));
        printf("BLOG TITLE: %s\n", a->title);
    } else {
        puts("No rows found.");
    }

    sqlite3_finalize(res);
    return a;
}
tag_array* get_all_tags(sqlite3* db,logger_t* logger){
	tag_array* ar = create_tag_array();
	char** err_msg = 0;
	char *sql = "SELECT  FROM articles ORDER BY created_time DESC";
	int rc = sqlite3_exec(db,sql,get_tags_callback,ar,err_msg);
	 if (rc != SQLITE_OK ) {
        
        log_error(logger, "Failed to select data");
        log_error(logger, "SQL error: %s",sqlite3_errmsg(db));

        sqlite3_free(err_msg);
        sqlite3_close(db);
        
        return NULL;
    } 
	return ar;
}
void database_close(sqlite3* db){
    sqlite3_close(db);
}