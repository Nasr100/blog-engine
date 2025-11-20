#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "post.h"


post* create_post(size_t id,char* title,char*slug,char* body,char* author,char* date,char* status,size_t likes, size_t views,int is_deleted,int user_id,user* user,dynamic_array* tags){
    post* p = malloc(sizeof(post));
    if(p==NULL){
        return NULL;
    }

    p->user = user;
    p->tags = tags;

    p->post_id = id;
    p->likes = likes;
    p->views = views;
    p->is_deleted = is_deleted;
    p->user_id = user_id;
    p->tags = tags;
    p->title= my_strdup(title);
    p->slug= my_strdup(slug);
    p->body = my_strdup(body);
    p->author = my_strdup(author);
    p->date = my_strdup(date);
    p->status = my_strdup(status);
    return p;

}

void destroy_post(post* post){
    if(post == NULL) return ;
    if(post->author)free(post->author);
    if(post->title)free(post->title);
    if(post->body)free(post->body);
    if(post->date)free(post->date);
    if(post->slug)free(post->slug);
    if(post->status)free(post->status);

    if(post->tags != NULL){
        for(int i = 0;i<post->tags->count;i++){
            tag* t = (tag*)post->tags->array[i];
            if(t) destroy_tag(t);
        }
        dynamic_array_destroy(post->tags);
    }

    if(post->user) destroy_user(post->user);
    
    
}

