#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "../../shared/db/database.h"
#include "../../shared/utils/file_utils.h"
#include "../include/response_builder.h"
#include "../include/md_parser.h"
#include "../../shared/libs/md4c-html.h"
#include "../include/blog.h"


char* home_header_file = "/mnt/c/Users/nasr0/OneDrive/Desktop/2026 C BOOTCAMP/BLOG ENGINE/public/template/html/home/home-header.html";
char* home_footer_file = "/mnt/c/Users/nasr0/OneDrive/Desktop/2026 C BOOTCAMP/BLOG ENGINE/public/template/html/home/home-footer.html";

char* post_header_file = "/mnt/c/Users/nasr0/OneDrive/Desktop/2026 C BOOTCAMP/BLOG ENGINE/public/template/html/post/post-header.html";
char* post_footer_file = "/mnt/c/Users/nasr0/OneDrive/Desktop/2026 C BOOTCAMP/BLOG ENGINE/public/template/html/post/post-footer.html";


void render_article(int socket, http_message* message,AppContext* ctx) {
    char* slug = file_get_basename(message->request->request_uri);
    HtmlBuffer html = {NULL, 0};
    article* article = get_blog_by_slug(ctx->db,slug);
    
    if(article == NULL) {
        send_404_page(socket);    
    } else {
         md_html(
            article->body,
            strlen(article->body),
            append_html,
            &html,
            MD_DIALECT_COMMONMARK, 
            0                      
        );
        
        int generated_html_length = html.size + strlen(article->title) + strlen("<h1 class=\"post-title\"></h1><div class=\"post-main\"></div>");
        char* post_header = file_read(post_header_file);
        char* post_footer = file_read(post_footer_file);
        
        if(post_header && post_footer) {
            int full_length = generated_html_length + strlen(post_header) + strlen(post_footer);
            create_response(200, HTML, full_length, socket);
            
            send(socket, post_header, strlen(post_header), 0);
            
            char* html_buffer = malloc(generated_html_length + 1);
            if(html_buffer) {
                snprintf(html_buffer, generated_html_length + 1, "<h1 class=\"post-title\">%s</h1><div class=\"post-main\">%s</div>", article->title, html.data);
                send(socket, html_buffer, strlen(html_buffer), 0);
                free(html_buffer);
            }
            
            send(socket, post_footer, strlen(post_footer), 0);
            free(post_header);
            free(post_footer);
        }
    }

    if(article) free(article);
    if(html.data) free(html.data);
}


void render_home(int socket,http_message* message,AppContext* ctx){
	
	article_array* array = get_all_blogs(ctx->db,ctx->logger);
	printf("Total articles in array: %d\n", array->count);
	char* temp_buffer = "<h3> no post created yet</h3>";
	char buffer [2048];
	int i = 0;
	char* li[100] = {0};
	int li_length = 0;
	if(array->count > 0){
			for(i = 0;i<array->count;i++){
				snprintf(buffer, sizeof(buffer), "<li><a href=\"/post/%s\">%s</a></li>", array->articles[i]->slug, array->articles[i]->title);				
				li[i] = calloc(strlen(buffer)+1,sizeof(char));
				strcpy(li[i],buffer);
				li_length = li_length + strlen(li[i]);
			}	
	}else{
		li_length = li_length + strlen(temp_buffer);
	}
	
	char* home_header = file_read(home_header_file);
	char* home_footer = file_read(home_footer_file);
	 if(home_header && home_footer) {
        li_length += strlen(home_header) + strlen(home_footer);
        create_response(200, HTML, li_length, socket);
        
        send(socket, home_header, strlen(home_header), 0);
        
        if(array != NULL && array->count > 0) {
            for(i = 0; i < array->count && li[i] != NULL; i++) {
                send(socket, li[i], strlen(li[i]), 0);
            }   
        } else {
            send(socket, temp_buffer, strlen(temp_buffer), 0);
        }
        
        send(socket, home_footer, strlen(home_footer), 0);
    }
    if(home_header) free(home_header);
    if(home_footer) free(home_footer);
	
	for(i = 0;i<array->count;i++){
		free(li[i]);
	}

}






