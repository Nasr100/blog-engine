#ifndef HASHMAP_H
#define HASHMAP_H

#define MAX_KEY_LENGTH 256


typedef struct key_value {
    char key[MAX_KEY_LENGTH];
    void* value;
    struct key_value* next;
} key_value_t;

typedef struct hashmap {
    key_value_t** array;
    int capacity;
    int count;
}hashmap_t;


// Generic hashmap interface
hashmap_t* hashmap_create(void);
int hashmap_insert(hashmap_t* map, const char* key, void* value);
void* hashmap_get( hashmap_t* map, const char* key);
void hashmap_destroy(hashmap_t* map);
int hashmap_size(const hashmap_t* map);

// Optional: with custom destructor for values
void hashmap_destroy_with_callback(hashmap_t* map, void (*free_value)(void*));

#endif