#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include "../../../shared/config/config.h"
#include "../../../shared/libs/sqlite3.h"

#define MAX_TAG_LENGTH 20
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 100
#define MAX_TAGS 3


typedef struct {
    char title[MAX_TITLE_LENGTH];
    char slug[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    char* body;
    char created_date[100];
//    char tags[MAX_TAGS];
} article;




/** 
 * delete last character of a string
 * 
 * @param original_string      string 
 * @param db_conn   Database connection handle
 * 
 * @return void      
 * 
 * @warning Overwrites the original_string
 */
void pop(char** original_string) {
    if (original_string == NULL || *original_string == NULL) {
        return;
    }
    
    size_t len = strlen(*original_string);
    if (len == 0) {
        free(*original_string);
        *original_string = NULL;
        return;
    }
    
    char* new_string = malloc(len);  // len instead of len+1 (removing one char)
    if (new_string == NULL) {
        return;  // Memory allocation failed
    }
    
    strncpy(new_string, *original_string, len - 1);
    new_string[len - 1] = '\0';
    
    free(*original_string);
    *original_string = new_string;
    
}


/** 
 * process the line of tags in post markdown file
 * 
 * @param line      line of tags in markdown file 
 * @param tags      array holder for the tags
 * 
 * @return void      
 * 
 * @warning     need to free the each index in array
 */

void split_tags_array(char* line,char* tags[]){
	char* copy_line = line;
	char* token = strtok(copy_line,":");
	token = strtok(NULL,":");

	char* copy_token = token;
	char* tag = strtok(copy_token+2,",");
	int tag_counter = 0;
	while(tag!=NULL && tag_counter<=MAX_TAGS){
		tags[tag_counter] = malloc(sizeof(char)*strlen(tag)+1);
		if(strrchr(tag,']') != NULL){
			strcpy(tags[tag_counter] ,tag);
			pop(&tags[tag_counter]);
		}else{
			strcpy(tags[tag_counter] ,tag);
		}
		tag = strtok(NULL,",");
		tag_counter++;
	}
}

/** 
 * turn title into slug
 * 
 * @param title         title of the blog 
 * @param slug_output   holder for the slug
 * 
 * @return void      
 * 
 *    
 */

void slug_converter(const char* title, char* slug_output) {
    int i = 0;
    for(i = 0;i<strlen(title);i++){
    	if(title[i] == ' '){
    		slug_output[i] = '-';
		}else if (isalnum(title[i])){
			slug_output[i] = tolower(title[i]);
		}
	}
	
    slug_output[i] = '\0';

}


/** 
 * add the blog to the database 
 * checks if the tags are already in the db if not it adds them
 * 
 * @param ar            article object
 * @param DB            database handler
 * @param tags          array of blog tags
 * 
 * @return void      
 * 
 *
 *    
 */
void add_article_db(article* ar, sqlite3* DB,char** tags) {

    char *err_msg = 0;
    sqlite3_stmt *res;
    char slug[100];
    
    slug_converter(ar->title, slug);
    strcpy(ar->slug, slug);
    
    printf("slug: %s\n", ar->slug);
    printf("title: %s\n", ar->title);
    printf("author: %s\n", ar->author);
    printf("date: %s\n", ar->created_date);
    
   	sqlite3_exec(DB, "BEGIN TRANSACTION", 0, 0, &err_msg);
    
    char* sql = "INSERT INTO articles (slug, title, author, body, created_date) VALUES(?, ?, ?, ?, ?);";
    int rc = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
    
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(res, 1, ar->slug, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 2, ar->title, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 3, ar->author, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 4, ar->body, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 5, ar->created_date, -1, SQLITE_STATIC);
        
        if (sqlite3_step(res) != SQLITE_DONE) {
            printf("Insert failed: %s\n", sqlite3_errmsg(DB));
        } else {
            printf("Insert successful!\n");
        }
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(DB));
    }
    
    sqlite3_finalize(res);
    int i =0;
    for(i = 0;i<MAX_TAGS;i++){
    	sqlite3_int64 tag_id;
    	char* tag = tags[i];
	    char* sql = "SELECT * FROM tags where tag = ?";
	    int rc = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
	    if (rc == SQLITE_OK) {
	        sqlite3_bind_text(res, 1,tag,-1,SQLITE_TRANSIENT);
	    }else {
	        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(DB));
	    }
	    int step = sqlite3_step(res);
	    if (step == SQLITE_ROW) {
	    	tag_id = sqlite3_column_int64(res, 0);
	        sqlite3_finalize(res);
	    }else{
	        sqlite3_finalize(res);
	        char* sql = "INSERT INTO tags (tag) VALUES(?);";
	    	int rc = sqlite3_prepare_v2(DB, sql, -1, &res, 0);
	    	if (rc == SQLITE_OK) {
	        	sqlite3_bind_text(res, 1,tag,-1,SQLITE_TRANSIENT);
	        		
	        	if (sqlite3_step(res) != SQLITE_DONE) {
	            	printf("Insert failed: %s\n", sqlite3_errmsg(DB));
	        	} else {
	            	printf("Insert successful!\n");
	            	tag_id = sqlite3_last_insert_rowid(DB);
	        	}
	    	}else {
	        	fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(DB));
	        	sqlite3_finalize(res);
	        	continue;
	    	}
		}
		sqlite3_finalize(res); 
		char* link_sql = "INSERT OR IGNORE INTO articles_tags (article_id, tag_id) VALUES(?, ?);";
		rc = sqlite3_prepare_v2(DB, link_sql, -1, &res, 0);
		if (rc == SQLITE_OK) {
		 	sqlite3_bind_text(res, 1,ar->slug,-1,SQLITE_TRANSIENT);
		 	sqlite3_bind_int64(res, 2, tag_id);
		 	if (sqlite3_step(res) != SQLITE_DONE) {
                printf("Article-tag link failed: %s\n", sqlite3_errmsg(DB));
            } else {
                printf("Article linked with tag '%s'\n", tag);
            }
		}else{
                fprintf(stderr, "Failed to prepare link statement: %s\n", sqlite3_errmsg(DB));
        }
         sqlite3_finalize(res);
	}
	sqlite3_exec(DB, "COMMIT", 0, 0, &err_msg);
	if (err_msg) {
        fprintf(stderr, "Transaction error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}


/** 
 * connecting to the db
 * @param name      name of the database
 * @param DB        database handler
 *
 * @return int      0 on success, 1 on error
 * 
 */
int db_con(char* name, sqlite3** DB) {
    int rc = sqlite3_open(name, DB);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*DB));
        sqlite3_close(*DB);
        return 1;
    }   
    sqlite3_exec(*DB, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);
    puts("DB connection SUCCESS");
    return 0;
}

/** 
 * get the value of medata of the blog (key : value)
 * @param line      line of the metedata blog
 *
 * @return char*    starting on success, 1 on error
 * 
 */
char* getValue(char* line) {
    char* colon_pos = strchr(line, ':');
    if (colon_pos == NULL) {
        return NULL;
    }
    
    char* value_start = colon_pos + 1;
    while (*value_start == ' ') {
        value_start++;
    }
    
    return value_start;
}




int main(int argc, char *argv[]) {
 
    BlogConfig* blogConfig = config_create();
    config_load(blogConfig);
    
    
    sqlite3* DB;
    DIR *dir;
    db_con(blogConfig->db_path, &DB);
    struct dirent *entry;
    char filepath[4096];
    char* article_tags[MAX_TAGS];
    
    dir = opendir(blogConfig->blogs_path);
    if (dir == NULL) {
        perror("Error opening directory");
        return 1;
    }
    
    FILE* fptr;
    
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strstr(entry->d_name, ".md")) {
            memset(filepath, 0, sizeof(filepath));
            sprintf(filepath, "%s\\%s", blogConfig->blogs_path, entry->d_name);
            
            printf("\nProcessing file: %s\n", entry->d_name);
            
            fptr = fopen(filepath, "r");
            if (fptr == NULL) {
                printf("Failed to open file: %s\n", filepath);
                continue;
            }
            
            article* current_article = malloc(sizeof(article));
            if (current_article == NULL) {
                printf("Memory allocation failed\n");
                fclose(fptr);
                continue;
            }
            
            memset(current_article, 0, sizeof(article));
            int article_valid = 0;
            
            fseek(fptr, 0, SEEK_END);
		    long file_size = ftell(fptr);
		    fseek(fptr,0, SEEK_SET);
            
            
            char line[2048];
            while (fgets(line, sizeof(line), fptr) != NULL) {
                line[strcspn(line, "\n")] = 0;
                
                if (strstr(line, "HEAD") != NULL) {
                    
                    break;
                }
                
                if (strstr(line, "status:") != NULL) {
                    char status_line[2048];
                    strcpy(status_line, line);
                    char* status = getValue(status_line);
                    if (status != NULL && strcmp(status, "U") == 0) {
                        printf("- %s BLOG NOT FINISHED YET\n", entry->d_name);
                        article_valid = 0;
                        break;
                    } else {
                        article_valid = 1;
                    }
                } else if (strstr(line, "title:") != NULL) { 
                    char title_line[2048];
                    strcpy(title_line, line);
                    char* title = getValue(title_line);
                    if (title != NULL) {
                        strncpy(current_article->title, title, sizeof(current_article->title) - 1);
                    }
                } else if (strstr(line, "date:") != NULL) {
                    char date_line[2048];
                    strcpy(date_line, line);
                    char* date = getValue(date_line);
                    if (date != NULL) {
                        strncpy(current_article->created_date, date, sizeof(current_article->created_date) - 1);
                    }
                }else if (strstr(line, "tags:") != NULL) {
					split_tags_array(line,article_tags);
					
				}else if (strstr(line, "author:") != NULL) {
                    char author_line[2048];
                    strcpy(author_line, line);
                    char* author = getValue(author_line);
                    if (author != NULL) {
                        strncpy(current_article->author, author, sizeof(current_article->author) - 1);
                    }
                }
            }
            
		    if (article_valid) {
		        current_article->body = malloc(file_size + 1);
		        if (current_article->body != NULL) {
		            size_t bytes_read = fread(current_article->body, 1, file_size, fptr);
		            current_article->body[bytes_read] = '\0';
		        }
		    add_article_db(current_article, DB,article_tags);
		    
		}
            
            if (current_article->body != NULL) {
                free(current_article->body);
            }
            free(current_article);
            fclose(fptr);
            char newfilepath[4096];
            if (snprintf(newfilepath, sizeof(newfilepath), "%s/%s", blogConfig->blogs_archive, entry->d_name) >= sizeof(newfilepath)) {
                fprintf(stderr, "Path too long: %s/%s\n", blogConfig->blogs_archive, entry->d_name);
                continue;
            }

            if (rename(filepath, newfilepath) == 0) {
                printf("File '%s' successfully moved to '%s'.\n", entry->d_name, newfilepath);
            } else {
                perror("Error moving file"); 
            }
        }
    }
    
    closedir(dir);
    sqlite3_close(DB);
    return 0;
}
