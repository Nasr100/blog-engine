#ifndef USERSERVICE_H
#define USERSERVICE_H


typedef struct userService userService;

user* userService_add_user(userService* userService,user* user);
int userService_delete_user(userService* userService,int user_id);
user* userService_edit_user(userService* userService,user* new_user);
user* userService_get_userByUsername(userService* userService,char* username);

#endif