#include <stdio.h>
#include <stdlib.h>
#include "userService.h"
#include "../../../shared/repository/user/userRepo.h"

struct userService{
    userRepo* userRepo;
};

user* userService_add_user(userService* userService,user* u){
    if(userService==NULL) return NULL;
   user* new_user = insert_user(userService->userRepo,u);
   return new_user;
}

int userService_delete_user(userService* userService,int user_id){
    if(userService==NULL) return -1;
    if(remove_user(userService->userRepo,user_id) == -1) return -1;
    return 1;
}

user* userService_edit_user(userService* userService,user* new_user){
    if(userService==NULL) return NULL;
    user* u = update_user(userService->userRepo,new_user);
    return u;
}

user* userService_get_userByUsername(userService* userService,char* username){
    user* u = select_byUsername(userService->userRepo,username);
    return u;
}