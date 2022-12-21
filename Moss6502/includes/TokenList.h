#pragma once
#include "Tokenizer.h"

typedef struct node {
    Tokens data;
    struct node* next;
} Token_Node;

typedef struct {
    Token_Node* head;
} Token_List;

//Initialize the list of Tokens
Token_List* initList();

//Initialize Node that contains token
Token_Node* initNode();

//Add to the list
void add_tok_l(Token_List* list, Tokens data);

//Push to the list
void push_tok_l(Token_List* list, Tokens data);

//Pop from the list
int pop_tok_l(Token_List* list);

//Deallocate Token list memory
void free_tok_l(Token_List* list);