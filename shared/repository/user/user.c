#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "../../utils/helpers.h"
// #include <openssl/evp.h>

user* create_user(int id,char* email,char*firstname,char* lastname,char* password,char* username,int age,char sex,int is_deleted,int is_admin){
    user* user = malloc(sizeof(user));
    if(user==NULL)return NULL;
    user->email = my_strdup(email);
    user->password= my_strdup(password);
    user->firstname= my_strdup(firstname);
    user->lastname= my_strdup(lastname);
    user->sex = sex;
    user->username = my_strdup(username);
    user->age = age;
    user->user_id = id;
    user->is_deleted = is_deleted;
    user->is_admin = is_admin;
    return user;
}

void destroy_user(user* user){
    if(user==NULL)return ;
    if(user->email)free(user->email);
    user->email = NULL;
    if(user->password)free(user->password);
    user->password = NULL;
    if(user->username)free(user->username);
    user->username = NULL;
    if(user->firstname)free(user->firstname);
    user->firstname = NULL;
    if(user->lastname)free(user->lastname);
    user->lastname = NULL;
    free(user);
}