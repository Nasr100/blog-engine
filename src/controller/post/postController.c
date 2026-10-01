#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "postController.h"
#include "../../../shared/utils/helpers.h"
#include "../../service/post/postService.h"
#include "../../include/post_serializer.h"
#include "../../include/response_builder.h"
#include "../../include/request_parser.h"

int get_postbyId_json(int socket, http_message* message,AppContext* ctx){
    char* str_id = get_last_uri(message->request->request_uri);
    int id = atoi(str_id);
    if(id == 0){
        char* err = "{\"error\":\"post id needs to be an integer\"}";
        send_response(400,JSON,strlen(err),socket,err);
        return -1;
    }
    repsonseMsg* rm = postService_get_postById(ctx->postService,id);
    post* p = (post*)rm->data;
    if(p == NULL){
        // char* err = "{\"error\":\"post id needs to be an integer\"}";
        send_response(404,JSON,strlen(err),socket,err);
        return -1;
    }
    cJSON* post_json = post_to_json((post*)rm->data);
    char* json_str = cJSON_Print(post_json);
    // send_json_response(socket, json_str, 200);
    free(json_str);
    cJSON_Delete(post_json);
    destroy_post((post*)rm->data);
    destroy_responseMsg(rm);
    return 0;
}
// int get_postbyId_html(int socket,http_message* message,AppContext* ctx){
//      char* str_id = get_last_uri(message->request->request_uri);
//     int id = atoi(str_id);
//     if(id == 0){
//         //send 404 page
        
//         // send_json_response(socket, "{\"error\":\"post id need to be an integer\"}", 400);
//         return -1;
//     }
//     repsonseMsg* rm = postService_get_postById(ctx->postService,id);
    
//     if(rm->data == NULL){
//         //send 404 page
//     }
//     post* p = rm->data;
//     //convert to html
//     return 0;
// }

// int postController_get_AllpostsPreview(int socket, http_message* message, AppContext* ctx){
//     (void)message;
//     puts("IM ENTERING GET POSTS API..");
//     repsonseMsg* rm = postService_get_allPostsPreview(ctx->postService);
    
//     if(rm->data == NULL){
//         // send_json_response(socket, rm->err_msg, rm->code_status);
//         destroy_responseMsg(rm);
//         return -1;
//     }
    
//     dynamic_array* posts = (dynamic_array*)rm->data;
    
//     // 🎯 Create proper JSON structure
//     cJSON* root = cJSON_CreateObject();
//     cJSON* posts_array = cJSON_CreateArray();
    
//     // // Add each post to the array
//     for(int i = 0; i < posts->count; i++){
//         post* p = posts->array[i];
//         cJSON* post_json = post_to_json(p);
//         cJSON_AddItemToArray(posts_array, post_json);
//     }
    
//     // // 🎯 Add the array to the root object
//     cJSON_AddItemToObject(root, "posts", posts_array);
//     cJSON_AddNumberToObject(root, "count", posts->count);
    
//     char* json_str = cJSON_Print(root);
    
//     // send_json_response(socket, json_str, 200);
    
//     // Cleanup
//     free(json_str);
//     cJSON_Delete(root);
    
//     for(int i = 0; i < posts->count; i++){
//         post* p = posts->array[i];
//         destroy_post(p);
//     }
//     dynamic_array_destroy(posts);
//     destroy_responseMsg(rm);
//     return 0;
// }