#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "hashmap.h"

// Custom free function for the test
void free_point(void* ptr) {
    printf("Freeing point at %p\n", ptr);
    free(ptr);
}

void test_basic_operations() {
    printf("Testing basic operations...\n");
    
    hashmap_t* map = hashmap_create();
    assert(map != NULL);
    assert(hashmap_size(map) == 0);
    
    // Test insert and get
    char* value1 = "Hello";
    char* value2 = "World";
    assert(hashmap_insert(map, "key1", value1) == 0);
    assert(hashmap_insert(map, "key2", value2) == 0);
    assert(hashmap_size(map) == 2);
    
    assert(strcmp(hashmap_get(map, "key1"), "Hello") == 0);
    assert(strcmp(hashmap_get(map, "key2"), "World") == 0);
    assert(hashmap_get(map, "nonexistent") == NULL);
    
    hashmap_destroy(map);
    printf("✓ Basic operations test passed\n");
}

void test_simple_struct() {
    printf("Testing simple struct...\n");
    
    hashmap_t* map = hashmap_create();
    
    typedef struct {
        int x;
        int y;
    } Point;
    
    Point* p1 = malloc(sizeof(Point));
    p1->x = 10;
    p1->y = 20;
    printf("Inserting point at %p\n", p1);
    assert(hashmap_insert(map, "point1", p1) == 0);
    
    Point* retrieved = (Point*)hashmap_get(map, "point1");
    printf("Retrieved point at %p\n", retrieved);
    assert(retrieved != NULL);
    assert(retrieved->x == 10);
    assert(retrieved->y == 20);
    
    hashmap_destroy_with_callback(map, free_point);
    printf("✓ Simple struct test passed\n");
}

int main() {
    printf("Running hashmap tests...\n\n");
    
    test_basic_operations();
    test_simple_struct();
    
    printf("\n🎉 All tests passed!\n");
    return 0;
}