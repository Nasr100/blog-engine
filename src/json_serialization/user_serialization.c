#include "../../include/user_serialization.h"
#include <stdlib.h>  // for malloc, free
#include <string.h>


// User serialization
cJSON* user_to_json(user* user) {
    cJSON* json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "id", user->user_id);
    cJSON_AddStringToObject(json, "email", user->email);
    cJSON_AddStringToObject(json, "username", user->username);
    cJSON_AddStringToObject(json, "firstname", user->firstname);
    cJSON_AddStringToObject(json, "lastname", user->lastname);
    cJSON_AddNumberToObject(json, "age", user->age);
    cJSON_AddStringToObject(json, "sex", (char[]){user->sex, '\0'}); // char to string
    cJSON_AddBoolToObject(json, "is_deleted", user->is_deleted);
    cJSON_AddBoolToObject(json, "is_admin", user->is_admin);
    return json;
}

cJSON* user_to_json_public(user* user) {
    cJSON* json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "id", user->user_id);
    cJSON_AddStringToObject(json, "username", user->username);
    cJSON_AddStringToObject(json, "firstname", user->firstname);
    cJSON_AddStringToObject(json, "lastname", user->lastname);
    return json; // No sensitive data
}

cJSON* users_to_json_array(dynamic_array* users) {
    cJSON* array = cJSON_CreateArray();
    for (size_t i = 0; i < users->count; i++) {
        user* u = (user*)users->array[i];
        cJSON_AddItemToArray(array, user_to_json_public(u)); // Public data only
    }
    return array;
}

char* users_to_json_string(dynamic_array* users) {
    cJSON* json_array = users_to_json_array(users);
    char* json_str = cJSON_Print(json_array);
    cJSON_Delete(json_array);
    return json_str;
}

user* json_to_user(cJSON* json) {
    user* user = malloc(sizeof(struct user));
    user->user_id = cJSON_GetObjectItem(json, "id")->valueint;
    user->email = strdup(cJSON_GetObjectItem(json, "email")->valuestring);
    user->username = strdup(cJSON_GetObjectItem(json, "username")->valuestring);
    user->firstname = strdup(cJSON_GetObjectItem(json, "firstname")->valuestring);
    user->lastname = strdup(cJSON_GetObjectItem(json, "lastname")->valuestring);
    user->age = cJSON_GetObjectItem(json, "age")->valueint;
    user->sex = cJSON_GetObjectItem(json, "sex")->valuestring[0]; // string to char
    user->is_deleted = cJSON_GetObjectItem(json, "is_deleted")->valueint;
    user->is_admin = cJSON_GetObjectItem(json, "is_admin")->valueint;
    
    // Handle password separately (usually from different input)
    cJSON* password_item = cJSON_GetObjectItem(json, "password");
    user->password = password_item ? strdup(password_item->valuestring) : NULL;
    
    return user;
}
