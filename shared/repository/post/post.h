#ifndef POST_H

#define  POST_H
#include "../user/user.h"
#include "../tag/tag.h"
#include "../../utils/dynamic_array/dynamic_array.h"
#include "../../utils/helpers.h"

// typedef enum post_status{
//     DRAFT,
//     PUBLISH
// }post_status;

typedef struct post{
    size_t post_id;
    char* title;
    char* slug;
    char* path;
    char* author;
    char* date;
    char* status;
    size_t likes;
    size_t views;
    int is_deleted;
    //FK RELATIONS
    user* user;
    int user_id;
    dynamic_array* tags;
}post;

post* create_post(size_t id,char* title,char*slug,char* path,char* author,char* date,char* status,size_t likes, size_t views,int is_deleted,int user_id,user* user,dynamic_array* tags);

void destroy_post(post* post);



#endif