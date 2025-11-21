#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "userRepo.h"

user* insert_user(userRepo* userRepo,user* user){
     sqlite3_stmt *res;
	char* sql = "INSERT INTO users (email,password,first_name,last_name,username,sex,age,is_admin) VALUES(?,?,?,?,?,?,?,?,?);";
    int rc = sqlite3_prepare_v2(userRepo->database->db, sql, -1, &res, 0);
    if (rc == SQLITE_OK) {
	    sqlite3_bind_text(res, 1,user->email,-1,SQLITE_TRANSIENT);	
	    sqlite3_bind_text(res, 2,user->password,-1,SQLITE_TRANSIENT);	
	    sqlite3_bind_text(res, 3,user->firstname,-1,SQLITE_TRANSIENT);	
	    sqlite3_bind_text(res, 4,user->lastname,-1,SQLITE_TRANSIENT);	
	    sqlite3_bind_text(res, 5,user->username,-1,SQLITE_TRANSIENT);
        char sex_str[2] = {user->sex,'\0'};	
	    sqlite3_bind_text(res, 6,sex_str,-1,SQLITE_TRANSIENT);	
	    sqlite3_bind_int(res, 7,user->age);	
	    sqlite3_bind_int(res, 8,user->is_admin);	
	    if (sqlite3_step(res) != SQLITE_DONE) {
	        printf("Insert failed: %s\n", sqlite3_errmsg(userRepo->database));
            return NULL;
	    } else {
	        printf("Insert successful!\n");
	        int new_user_id = sqlite3_last_insert_rowid(userRepo->database->db);
            user->user_id = new_user_id;
	    }
	}else{
	    fprintf(stderr, "Failed to execute insert user statement: %s\n", sqlite3_errmsg(userRepo->database->db));
	    sqlite3_finalize(res);
        return NULL;    	
	}
    sqlite3_finalize(res);
    if (userRepo->database->err_msg) {
        fprintf(stderr, "Transaction error: %s\n", userRepo->database->err_msg);
        sqlite3_free(userRepo->database->err_msg);
         return NULL;
    }
    return user;
}


int remove_user(userRepo* userRepo,int user_id){
     sqlite3_stmt *res;
    const char *sql = "UPDATE users SET is_deleted = 1  WHERE user_id = ?";
    int rc = sqlite3_prepare_v2(userRepo->database->db, sql, -1, &res, 0);
     if (rc == SQLITE_OK){
        sqlite3_bind_int(res, 1,user_id);	
	    if (sqlite3_step(res) != SQLITE_DONE) {
	        printf("USER DELETE failed: %s\n", sqlite3_errmsg(userRepo->database->db));
            return -1;
	    } else {
	        printf("USER DELETE successful!\n");
	        
	    }
     }else {
	    fprintf(stderr, "Failed to execute DELETE USER statement: %s\n", sqlite3_errmsg(userRepo->database->db));
	    sqlite3_finalize(res);
        return -1;  	
	}
    sqlite3_finalize(res);
    return 1;
}

user* update_user(userRepo* userRepo,user* user){
    sqlite3_stmt *res;
    const char *sql = "UPDATE users SET email=?,username = ?,first_name=?,last_name=?,sex=?,age=?,is_admin=?  WHERE user_id = ?";
    int rc = sqlite3_prepare_v2(userRepo->database->db, sql, -1, &res, 0);
     if (rc == SQLITE_OK){
        sqlite3_bind_text(res, 1,user->email,-1,SQLITE_TRANSIENT);	
        sqlite3_bind_text(res, 1,user->username,-1,SQLITE_TRANSIENT);	
        sqlite3_bind_text(res, 1,user->firstname,-1,SQLITE_TRANSIENT);	
        sqlite3_bind_text(res, 1,user->lastname,-1,SQLITE_TRANSIENT);	
        char sex_str[2] = {user->sex, '\0'};
        sqlite3_bind_text(res, 1,sex_str,-1,SQLITE_TRANSIENT);	
        sqlite3_bind_int(res, 1,user->age);
        sqlite3_bind_int(res, 1,user->is_admin);
        sqlite3_bind_int(res, 1,user->user_id);
	    if (sqlite3_step(res) != SQLITE_DONE) {
	        printf("USER UPDATE failed: %s\n", sqlite3_errmsg(userRepo->database->db));
            return NULL;
	    } else {
	        printf("USER UPADTE successful!\n");
	    }
     }else {
	    fprintf(stderr, "Failed to execute update USER statement: %s\n", sqlite3_errmsg(userRepo->database->db));
	    sqlite3_finalize(res);
         return NULL;  	
	}
    sqlite3_finalize(res);
    return user;
}

user* select_byUsername(userRepo* userRepo,char* username){
    sqlite3_stmt *res;
    user* user = NULL;
    const char *sql = "SELECT user_id,email,first_name,last_name,username,age,sexe,is_admin,is_deleted,password FROM users where username = ?;";
    int rc = sqlite3_prepare_v2(userRepo->database->db, sql, -1, &res, 0);
    if(rc!=SQLITE_OK){
         fprintf(stderr, "Failed to execute select USER  byUsername statement: %s\n", sqlite3_errmsg(userRepo->database->db));
	    sqlite3_finalize(res);
         return NULL;  	
    }

    sqlite3_bind_text(res,1,username,-1,SQLITE_TRANSIENT);
    if(sqlite3_step(res)== SQLITE_ROW){
            int user_id = sqlite3_column_int(res, 0);
	          char *email = sqlite3_column_text(res, 1);
	          char *firstname = sqlite3_column_text(res, 2);
	          char *lastname = sqlite3_column_text(res, 3);
	          char *username = sqlite3_column_text(res, 4);
            int age = sqlite3_column_int(res, 5);
	          char *sex = sqlite3_column_text(res, 6);
	        int is_admin = sqlite3_column_int(res, 7);
	        int is_deleted = sqlite3_column_int(res, 8);
              char *password = sqlite3_column_text(res, 9);

            user = create_user(user_id,email,password,firstname,lastname,username,age,sex,is_deleted,is_admin);
    }
    return user;   
}

user* select_byEmail(userRepo* userRepo,char* email){
    sqlite3_stmt *res;
    user* user = NULL;
    const char *sql = "SELECT user_id,email,first_name,last_name,username,age,sexe,is_admin,is_deleted,password FROM users where email = ?;";
    int rc = sqlite3_prepare_v2(userRepo->database->db, sql, -1, &res, 0);
    if(rc!=SQLITE_OK){
         fprintf(stderr, "Failed to execute select USER  byUsername statement: %s\n", sqlite3_errmsg(userRepo->database->db));
	    sqlite3_finalize(res);
         return NULL;  	
    }

    sqlite3_bind_text(res,1,email,-1,SQLITE_TRANSIENT);
    if(sqlite3_step(res)== SQLITE_ROW){
            int user_id = sqlite3_column_int(res, 0);
	        const unsigned char *email = sqlite3_column_text(res, 1);
	        const unsigned char *firstname = sqlite3_column_text(res, 2);
	        const unsigned char *lastname = sqlite3_column_text(res, 3);
	        const unsigned char *username = sqlite3_column_text(res, 4);
            int age = sqlite3_column_int(res, 5);
	        const unsigned char *sex = sqlite3_column_text(res, 6);
	        int is_admin = sqlite3_column_int(res, 7);
	        int is_deleted = sqlite3_column_int(res, 8);
            const unsigned char *password = sqlite3_column_text(res, 9);

            user = create_user(user_id,email,password,firstname,lastname,username,age,sex,is_deleted,is_admin);
    }
    return user;   
}




