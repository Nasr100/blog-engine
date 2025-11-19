#ifndef USERREPO_H
#define USERREPO_H
#include "../../db/database.h"
#include "user.h"


typedef struct userRepo{
    database* database;
}userRepo;

user* insert_user(userRepo* userRepo,user* user);
int remove_user(userRepo* userRepo,int user_id);
user* update_user(userRepo* userRepo,user* user);
user* select_byUsername(userRepo* userRepo,char* username);
#endif