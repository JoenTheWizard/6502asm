#include "TokenList.h"

Token_List* initList() {
    Token_List* list = (Token_List*)malloc(sizeof(Token_List));

    if (!list)
        return NULL;
    list->head = NULL;
    return list;
}

Token_Node* initNode(Tokens token) {
    Token_Node* node = (Token_Node*)malloc(sizeof(Token_Node));

    if (!node)
        return NULL;

    node->data = token;
    node->next = NULL;
    return node;
}

void add_tok_l(Token_List* list, Tokens data) {
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

void push_tok_l(Token_List* list, Tokens data) {
    Token_Node* tmp = initNode(data);
    tmp->next = list->head;
    list->head = tmp;
}

int pop_tok_l(Token_List* list) {
    if (list->head == NULL)
        return -1;

    Token_Node* h = list->head;
    list->head = list->head->next;

    free(h);
    return 0;
}

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