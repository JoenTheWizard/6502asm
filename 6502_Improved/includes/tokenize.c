#include "tokenize.h"

//Local function to read file size for dynamic allocation
static size_t size_of_file(FILE* file) {
    if (file == NULL)
        return -1;
    
    fseek(file, 0, SEEK_END);
    size_t sz = ftell(file);
    fseek(file, 0, SEEK_SET);

    return sz + 1;
}

//Need to tokenize the file and return tokenized list
void tokenize_file(char* path) {
    Token_List* list = initList();

    free_tok_l(list);
}

//Linked List functionalities
//Init list
Token_List* initList() {
    Token_List* list = (Token_List*)malloc(sizeof(Token_List));

    if (!list)
        return NULL;
    list->head = NULL;
    return list;
}
//Init node
Token_Node* initNode(ASM6502_TOKENS token) {
    Token_Node* node = (Token_Node*)malloc(sizeof(Token_Node));

    if (!node)
        return NULL;

    node->data = token;
    node->next = NULL;
    return node;
}
//Add to list
void add_tok_l(Token_List* list, ASM6502_TOKENS data) {
    Token_Node* cur = NULL;
    if (list->head == NULL)
        list->head = initNode(data);
    else {
        cur = list->head;
        while (cur->next != NULL)
            cur = cur->next;
        
        cur->next = initNode(data);
    }
}
//Push to list
void push_tok_l(Token_List* list, ASM6502_TOKENS data) {
    Token_Node* tmp = initNode(data);
    tmp->next = list->head;
    list->head = tmp;
}
//Pop from list
int pop_tok_l(Token_List* list) {
    if (list->head == NULL)
        return -1;

    Token_Node* h = list->head;
    list->head = list->head->next;

    free(h);
    return 0;
}
//Free list
void free_tok_l(Token_List* list) {
    Token_Node* cur = list->head;
    Token_Node* next = cur;

    while (cur != NULL) {
        next = cur->next;
        free(cur);
        cur = next;
    }

    free(list);
}