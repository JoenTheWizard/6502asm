#ifndef LABELS_H
#define LABELS_H
#include <stdlib.h>
#include <stdint.h>

//Labels will need to be read from second pass and will be stored as a list

//Labels structure
typedef struct {
    char* name; //Name of the label
    int lineNumber; //Where the label is at
    uint16_t addr; //The address it stores
} LABELS;

//Nodes
typedef struct lbl_node {
    LABELS data;
    struct lbl_node* next;
} Labels_Node;

//List
typedef struct {
    Labels_Node* head;
} Labels_List;

//To initialize the a list of labels
Labels_List* initLblList();

//To initialize a label node
Labels_Node* initLblNode();

//Add an element to the list
void add_lbl_l(Labels_List* list, LABELS* data);

//Push to the list
void push_lbl_l(Labels_List* list, LABELS* data);

//Free label buffers
void free_lbl_l(Labels_List* list);

#endif