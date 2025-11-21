#ifndef USER_SERIALIZER_H
#define USER_SERIALIZER_H

#include "../../shared/libs/cJSON.h"
#include "../../shared/repository/user/user.h"
#include "../../shared/utils/dynamic_array/dynamic_array.h"

cJSON* user_to_json(user* user);
cJSON* user_to_json_public(user* user); // Without sensitive data
cJSON* users_to_json_array(dynamic_array* users);
char* users_to_json_string(dynamic_array* users);
user* json_to_user(cJSON* json);



#endif