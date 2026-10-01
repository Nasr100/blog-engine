
#include <stdlib.h>  // for malloc, free
#include <string.h>
#include "../include/post_serializer.h"

cJSON* post_to_json(post* post) {
    cJSON* json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "id", post->post_id);
    cJSON_AddStringToObject(json, "title", post->title);
    cJSON_AddStringToObject(json, "slug", post->slug);
    cJSON_AddStringToObject(json, "path", post->path);
    cJSON_AddStringToObject(json, "author", post->author);
    cJSON_AddStringToObject(json, "date", post->date);
    cJSON_AddStringToObject(json, "status", post->status);
    cJSON_AddNumberToObject(json, "likes", post->likes);
    cJSON_AddNumberToObject(json, "views", post->views);
    
    // Add user if available
    if (post->user){
        cJSON* user_json = cJSON_CreateObject();
        cJSON_AddNumberToObject(user_json, "id", post->user->user_id);
        cJSON_AddStringToObject(user_json, "username", post->user->username);
        cJSON_AddItemToObject(json, "user", user_json);
    } else {
        cJSON_AddNumberToObject(json, "user_id", post->user_id);
    }
    
    // Add tags array 
    if (post->tags && post->tags->count > 0) {
        cJSON* tags_array = cJSON_CreateArray();
        for (size_t i = 0; i < post->tags->count; i++) {
            tag* t = (tag*)(post->tags->array[i]);
            cJSON* tag_json = cJSON_CreateObject();
            cJSON_AddNumberToObject(tag_json, "id", t->tag_id);
            cJSON_AddStringToObject(tag_json, "name", t->name);
            cJSON_AddItemToArray(tags_array, tag_json);
        }
        cJSON_AddItemToObject(json, "tags", tags_array);
    }
    
    return json;
}

cJSON* post_to_json_basic(post* post) {
    cJSON* json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "id", post->post_id);
    cJSON_AddStringToObject(json, "title", post->title);
    cJSON_AddStringToObject(json, "slug", post->slug);
    cJSON_AddStringToObject(json, "author", post->author);
    cJSON_AddStringToObject(json, "date", post->date);
    cJSON_AddNumberToObject(json, "likes", post->likes);
    cJSON_AddNumberToObject(json, "views", post->views);
    return json;
}

post* json_to_post(cJSON* json) {
    post* post = malloc(sizeof(struct post));
    post->post_id = cJSON_GetObjectItem(json, "id")->valueint;
    post->title = strdup(cJSON_GetObjectItem(json, "title")->valuestring);
    post->slug = strdup(cJSON_GetObjectItem(json, "slug")->valuestring);
    post->path = strdup(cJSON_GetObjectItem(json, "path")->valuestring);
    post->author = strdup(cJSON_GetObjectItem(json, "author")->valuestring);
    post->date = strdup(cJSON_GetObjectItem(json, "date")->valuestring);
    post->status = strdup(cJSON_GetObjectItem(json, "status")->valuestring);
    post->likes = cJSON_GetObjectItem(json, "likes")->valueint;
    post->views = cJSON_GetObjectItem(json, "views")->valueint;
    
    // Handle user_id if present
    cJSON* user_id_item = cJSON_GetObjectItem(json, "user_id");
    if (user_id_item) {
        post->user_id = user_id_item->valueint;
    }
    
    post->user = NULL;
    post->tags = NULL;
    
    return post;
}

cJSON* posts_to_json_array(dynamic_array* posts) {
    cJSON* array = cJSON_CreateArray();
    for (size_t i = 0; i < posts->count; i++) {
        post* p = (post*)posts->array[i];
        cJSON* post_json = post_to_json_basic(p);
        cJSON_AddItemToArray(array, post_json);
    }
    return array;
}

char* posts_to_json_string(dynamic_array* posts) {
    cJSON* json_array = posts_to_json_array(posts);
    char* json_str = cJSON_Print(json_array);
    cJSON_Delete(json_array);
    return json_str;
}