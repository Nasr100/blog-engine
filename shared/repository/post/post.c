#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "post.h"


post* create_post(size_t id,char* title,char*slug,char* path,char* author,char* date,char* status,size_t likes, size_t views,int is_deleted,int user_id,user* user,dynamic_array* tags){
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
    p->path = my_strdup(path);
    p->author = my_strdup(author);
    p->date = my_strdup(date);
    p->status = my_strdup(status);
    return p;

}

void destroy_post(post* post){
    if(post == NULL) return ;
    if(post->author)free(post->author);
    post->author =NULL;
    if(post->title)free(post->title);
    post->title =NULL;
    if(post->path)free(post->path);
    post->path =NULL;
    if(post->date)free(post->date);
    post->date =NULL;
    if(post->slug)free(post->slug);
    post->slug =NULL;
    if(post->status)free(post->status);
    post->status =NULL;
    if(post->tags != NULL){
        for(int i = 0;i<post->tags->count;i++){
            tag* t = (tag*)post->tags->array[i];
            if(t) destroy_tag(t);
            t=NULL;
        }
        dynamic_array_destroy(post->tags);
        post->tags=NULL;
    }

    if(post->user) destroy_user(post->user);
    post->user=NULL;
    
    
}

