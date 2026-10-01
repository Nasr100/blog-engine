#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "homeController.h"
#include "../../service/post/postService.h"
#include "../../../shared/utils/file_utils.h"
#include "../../include/response_builder.h"




void build_home_page(FILE* temp_file, dynamic_array* pp)
{
    char* header_buffer = get_home_header_html("Home");

    fputs(header_buffer, temp_file);
    free(header_buffer);

    char html_txt[4096];

    fputs("<main class=\"home-main\">", temp_file);

    for (int i = 0; i < pp->count; i++) {

        post* current_post = pp->array[i];

        snprintf(
            html_txt,
            sizeof(html_txt),

            "<div class=\"card\">"

                "<div class=\"card-header\">"

                    "<div class=\"image-wrapper\">"
                    "</div>"

                    "<div class=\"user-meta\">"
                        "<p class=\"user-name\">%s</p>"
                    "</div>"

                "</div>"

                "<div class=\"card-main\">"

                    "<h2>"
                        "<a href=\"/posts/%zu\">%s</a>"
                    "</h2>"

                    "<div class=\"tags\">",

            current_post->author,
            current_post->post_id,
            current_post->title
        );

        fputs(html_txt, temp_file);

        // Tags
        dynamic_array* tags = current_post->tags;

        for (int j = 0; j < tags->count; j++) {

            tag* current_tag = tags->array[j];

            snprintf(
                html_txt,
                sizeof(html_txt),

                "<a href=\"/tags/%s\" class=\"tag\">"
                    "<span class=\"tag-prefix\">#%s</span>"
                "</a>",

                current_tag->name,
                current_tag->name
            );

            fputs(html_txt, temp_file);
        }

        // Finish card
        snprintf(
            html_txt,
            sizeof(html_txt),

            "</div>"

            "<div class=\"card-footer\">"

                "<a href=\"\" class=\"likes\">"

                    "<i class=\"fa-regular fa-heart\"></i>"

                    "<span class=\"likes-number\">%zu</span>"

                "</a>"

            "</div>"

            "</div>",

            current_post->likes
        );

        fputs(html_txt, temp_file);
    }

    fputs("</main>", temp_file);

    char* footer_buffer = get_home_footer_html();

    fputs(footer_buffer, temp_file);

    // free(footer_buffer);
}

void render_home_page(int socket,http_message* message,AppContext* ctx){
    FILE *temp_file = create_tmp_file();
   repsonseMsg* posts_preview =  postService_get_allPostsPreview(ctx->postService);
   dynamic_array* pp = posts_preview->data;
    build_home_page(temp_file,pp);
    fseek(temp_file, 0, SEEK_END);
    size_t size = ftell(temp_file);
    rewind(temp_file);
    char buffer [size];
    fread(buffer,sizeof(*buffer),size,temp_file);
    send_response(200,HTML,size,socket,buffer);
    int i = 0;
    for(i=0;i<pp->count;i++){
        post* current_post = pp->array[i];
        destroy_post(current_post);
    }
    dynamic_array_destroy(pp);
   
}