#include "labels.h"

//Initialize the list, must be freed
Labels_List* initLblList() {
    Labels_List* list = (Labels_List*)malloc(sizeof(Labels_List));

    if (!list)
        return NULL;
    
    list->head = NULL;
    return list;
}

//Initialize a node, must be freed
Labels_Node* initLblNode(LABELS lbl) {
    Labels_Node* node = (Labels_Node*)malloc(sizeof(Labels_Node));

    if (!node)
        return NULL;
    
    node->data = lbl;
    node->next = NULL;
    return node;
}

//Add value to list
void add_lbl_l(Labels_List* list, LABELS* data) {
    Labels_Node* cur = NULL;
    if (list->head == NULL)
        list->head = initLblNode(*data);
    else {
        cur = list->head;
        while (cur->next != NULL)
            cur = cur->next;
        cur->next = initLblNode(*data);
    }
}

//Push value to list
void push_lbl_l(Labels_List* list, LABELS* data) {
    Labels_Node* tmp = initLblNode(*data);
    tmp->next = list->head;
    list->head = tmp;
}

//Free and deallocate list
void free_lbl_l(Labels_List* list) {
    Labels_Node* cur = list->head;
    Labels_Node* next = cur;

    while (cur != NULL) {
        next = cur->next;
        free(cur);
        cur = next;
    }

    free(list);
}