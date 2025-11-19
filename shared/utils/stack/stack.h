#ifndef STACK_H
#define STACK_H


typedef struct stackNode{
    struct stackNode* parent;
    char* data_type;
    void* data;
    char* var_name;
}stackNode;

stackNode* push(stackNode* top,char* data_type,char* var_name,void* data);
stackNode* pop(stackNode* top);
void* search_from_top(stackNode* top,char* var_name);
void destroy_stack(stackNode* top);
#endif