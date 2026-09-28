#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include "./include/importer_helpers.h"
#include "../../../shared/config/config.h"
#include "../../../shared/db/database.h"
#include "../../../shared/repository/post/post.h"
#include "../../../shared/repository/post/postRepo.h"
#include "../../../shared/repository/tag/tagRepo.h"
#include "../../../shared/repository/tag/tag.h"
#include "../../../shared/repository/post_tag/post_tagRepo.h"

#define MAX_TAGS 5

int main(int argc, char *argv[]) {
    BlogConfig* blogConfig = config_create();
    
    config_load(blogConfig);
    config_expand_variables(blogConfig);
    if(blogConfig->base_dir == NULL){
        puts("NUUUUUUUUUUUUUUUUL");
    }
        printf("db path %s",blogConfig->db_path);

    database* database = database_connect(blogConfig->db_path);
    postRepo* postRepo = create_post_repo(database); 
    tagRepo* tagRepo = create_tagRepo(database); 
    post_tagRepo* postTagRepo = create_postTag_repo(database); 
    DIR *dir;
    struct dirent *entry;
    char filepath[4096];
    char* article_tags[MAX_TAGS];
    

    dir = opendir(blogConfig->blogs_path);
    if (dir == NULL){
        perror("Error opening directory");
        return 1;
    }
    
    FILE* fptr;
    
    while ((entry = readdir(dir)) != NULL) { //while the directory have files
       
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strstr(entry->d_name, ".md")) { //the file name need to be md
            puts("hna");
            memset(filepath, 0, sizeof(filepath));
            
            sprintf(filepath, "%s/%s", blogConfig->blogs_path, entry->d_name); //get the file path
             
            printf("\nProcessing file: %s\n", entry->d_name);
            
            fptr = fopen(filepath, "r"); //open the file
            if (fptr == NULL) {
                printf("Failed to open file: %s\n", filepath);
                continue;
            }
            
            post* p = NULL;
            int article_valid = 0;
            
            // fseek(fptr, 0, SEEK_END);
		    // long file_size = ftell(fptr);//get blog file size
		    fseek(fptr,0, SEEK_SET);
            
            
            char line[4096];
            char* title ;
            char* date;
             char* author;
            int tags_count;
            while (fgets(line, sizeof(line), fptr) != NULL) {
                line[strcspn(line, "\n")] = 0;
                
                if (strstr(line, "HEAD") != NULL) {
                    puts("HEAD ACHIEVED");
                    break;
                }
                
                if (strstr(line, "status:") != NULL) {
                    char status_line[2048];
                    strcpy(status_line, line);
                    char* status = getValueV2(status_line);
                    if (status != NULL && strcmp(status, "U") == 0) {
                        printf("- %s BLOG NOT FINISHED YET\n", entry->d_name);
                        article_valid = 0;
                        break;
                    } else {
                        puts("BLOG FINISHED");
                        article_valid = 1;
                    }
                } else if (strstr(line, "title:") != NULL) { 
                    char title_line[2048];
                    strcpy(title_line, line);
                     title = strdup(getValueV2(title_line)); 
                     
                    
                } else if (strstr(line, "date:") != NULL) {
                    char date_line[2048];
                    strcpy(date_line, line);
                    date = strdup(getValueV2(date_line));
                    
                }else if (strstr(line, "tags:") != NULL) {
					tags_count = split_tags_array(line,article_tags);
                    int i = 0;
                    puts("HNA KHRA LIK KULCHI ALHMAR");
                    for(;i<tags_count;i++){
                        puts(article_tags[i]);
                    }
					
				}else if (strstr(line, "author:") != NULL) {
                     char author_line[2048];
                    strcpy(author_line, line);
                    author =strdup(getValueV2(author_line));
                   
                }
                
            }
            puts("METADATA CAPTURED SUCCESSFULLY");
		    if (article_valid){
		        char* path = strdup(filepath);
		        
                char slug[strlen(title)+1];
                slug_converterv2(title,slug);
                puts(title);
                puts(date);
                puts(author);
                p = create_post(0,title,slug,path,author,date,"publish",0,0,0,1,NULL,NULL);
                puts("post created");
                database_begin_transaction(database);
                p = insert_post(postRepo,p);
                puts("POST INSRTED IN DB");

                if(p==NULL) continue;
                    puts("BEGIN INSERTING TAGS");
                for(int i =0;i<tags_count;i++){
                    tag* t = create_tag(0,article_tags[i],0);
                    if(t!=NULL) t=insert_tag(tagRepo,t);
                    if(t==NULL){
                        database_rollback(database);
                        continue;
                    }
                    add_tagToPost(postTagRepo,p->post_id,t->tag_id);
                }
                puts("tags inserted");
                database_commit(database);
                destroy_post(p);
		
            }
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
    puts("DIRECTORY EMPTY");
    closedir(dir);
    database_close(database);
    
    return 0;
}

