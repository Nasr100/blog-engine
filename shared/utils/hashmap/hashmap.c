#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include "hashmap.h"

#define MAX_KEY_LENGTH 100

typedef struct key_value {
    char key[MAX_KEY_LENGTH];
    void* value;
    struct key_value* next;
} key_value_t;

struct hashmap {
    key_value_t** array;
    int capacity;
    int count;
};

static int hash_key(const char* key) {
    int sum = 0;
    for(int i = 0; key[i] != '\0'; i++) {
        sum = sum + (int)key[i];
    }
    return sum;
}

static key_value_t* create_keyval(const char* key, void* value) {
    key_value_t* kv = (key_value_t*)malloc(sizeof(key_value_t));
    if(kv == NULL) {
        fprintf(stderr, "ERROR: Cannot allocate memory for key_value\n");
        return NULL;
    }
    
    strncpy(kv->key, key, MAX_KEY_LENGTH - 1);
    kv->key[MAX_KEY_LENGTH - 1] = '\0';
    kv->value = value;
    kv->next = NULL;
    return kv;
}

static int resize_hashmap(hashmap_t* map) {
    if(!map) return -1;
    
    if(map->count < map->capacity * 0.70) {
        return 0; 
    }
    
    int old_capacity = map->capacity;
    key_value_t** old_array = map->array;
    
    map->capacity = old_capacity * 2;
    key_value_t** new_array = (key_value_t**)calloc(map->capacity, sizeof(key_value_t*));
    if(!new_array) {
        fprintf(stderr, "ERROR: Failed to allocate memory for resized hashmap\n");
        return -1;
    }
    
    map->array = new_array;
    map->count = 0;

    // Rehash all entries
    for(int i = 0; i < old_capacity; i++) {
        key_value_t* current = old_array[i];
        while(current != NULL) {
            key_value_t* next = current->next;
            
            int new_index = hash_key(current->key) % map->capacity;
            
            // Insert at head of chain in new array
            current->next = new_array[new_index];
            new_array[new_index] = current;
            map->count++;
            
            current = next;
        }
    }
    
    free(old_array);
    return 0;
}

hashmap_t* hashmap_create(void) {
    hashmap_t* map = malloc(sizeof(hashmap_t));
    if(map == NULL) {
        fprintf(stderr, "ERROR: Cannot allocate memory for hashmap\n");
        return NULL;
    }
    
    map->capacity = 16;
    map->count = 0;
    map->array = calloc(map->capacity, sizeof(key_value_t*));
    if(!map->array) {
        free(map);
        fprintf(stderr, "ERROR: Cannot allocate memory for hashmap array\n");
        return NULL;
    }
    
    return map;
}

int hashmap_insert(hashmap_t* map, const char* key, void* value) {
    if(!map || !key) return -1;
    
    if(resize_hashmap(map) != 0) {
        return -1;
    }
    
    int index = hash_key(key) % map->capacity;
    
    key_value_t* current = map->array[index];
    while(current != NULL) {
        if(strcmp(current->key, key) == 0) {
            // Key exists, replace value
            current->value = value;
            return 0;
        }
        current = current->next;
    }
    
    key_value_t* kv = create_keyval(key, value);
    if(!kv) return -1;
    
    kv->next = map->array[index];
    map->array[index] = kv;
    map->count++;
    
    return 0;
}

void* hashmap_get(hashmap_t* map, const char* key) {
    if(!map || !key) return NULL;
    
    int index = hash_key(key) % map->capacity;
    key_value_t* current = map->array[index];
    
    while(current != NULL) {
        if(strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    
    return NULL; 
}

int hashmap_size(const hashmap_t* map) {
    return map ? map->count : 0;
}

void hashmap_destroy(hashmap_t* map) {
    if(!map) return;
    
    for(int i = 0; i < map->capacity; i++) {
        key_value_t* current = map->array[i];
        while(current != NULL) {
            key_value_t* next = current->next;
            free(current);
            current = next;
        }
    }
    
    free(map->array);
    free(map);
}

void hashmap_destroy_with_callback(hashmap_t* map, void (*free_value)(void*)) {
    if(!map) return;
    
    for(int i = 0; i < map->capacity; i++) {
        key_value_t* current = map->array[i];
        while(current != NULL) {
            key_value_t* next = current->next;
            if(free_value && current->value) {
                free_value(current->value);
            }
            free(current);
            current = next;
        }
    }
    
    free(map->array);
    free(map);
}