#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "../helpers.h"



static stackNode* craete_node(char* data_type,void* data,char* var_name){
    stackNode* sn = malloc(sizeof(stackNode));
    if(sn==NULL) return NULL;
    sn->data_type = my_strdup(data_type);
    sn->var_name = my_strdup(var_name);
    sn->data = data;
    sn->parent = NULL;
    return sn;
}




stackNode* push(stackNode* top,char* data_type,char* var_name,void* data){
    if(data == NULL || data_type == NULL||var_name==NULL) return top;
    stackNode* sn = craete_node(data_type,data,var_name);
    if(top == NULL){
        top = sn;
    }
    sn->parent = top;
    top = sn;
    return top;
}

// stackNode* peek(stackNode* top){
//     return top;
// }

void* search_from_top(stackNode* top,char* var_name){
    stackNode* current = top;
    while(current!=NULL){
        if(strcmp(current->var_name,var_name)==0){
            return current->data;
        }
        current = current->parent;
    }
    return NULL;
}


stackNode* pop(stackNode* top){
    if (top == NULL) return NULL;
    
    stackNode* new_top = top->parent;  // Get next first
    free(top);            // Then free current
    return new_top;
}

void destroy_stack(stackNode* top){
    if(top == NULL) return;
    while(top!=NULL){
        top = pop(top);
    }
}
