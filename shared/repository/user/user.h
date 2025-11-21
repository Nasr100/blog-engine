#ifndef USER_H

#define  USER_H

typedef struct user{
    int user_id;
    char* email;
    char* password;
    char* firstname;
    char* lastname;
    char* username;
    int age;
    char sex;
    int is_deleted;
    int is_admin;
}user;

user* create_user(int id,char* email,char* password,char*firstname,char* lastname,char* username,int age,char sex,int is_deleted,int is_admin);

void destroy_user(user* user);
#endif