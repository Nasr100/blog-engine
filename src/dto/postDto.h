#ifndef POSTDTO_H
#define POSTDTO_H
#define MAX_TAGS 5
#include "../../shared/repository/post/post.h"
typedef struct postCreateDto{
    char* title;
    char* slug;
    char* body;
    char* author;
    char* date;
    char* status;
    int tags_id[MAX_TAGS];
    int count_tags;
    int user_id;
    // int tags_id[MAX_TAGS];
}postCreateDto;

typedef struct postResponseDto{
    int post_id;
    char* title;
    char* slug;
    char* body;
    char* author;
    char* date;
    char* status;
    int likes;
    int views;
    int is_deleted;
    user* user;
    dynamic_array* tags;
}postResponseDto;

#endif