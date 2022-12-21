#include "Parser.h"

int _moss_parse_statements(Token_List* list) {
    uint32_t index = 0;

    Token_Node* cur = list->head;

    while (cur != NULL) {
        //if statement
        if (cur->data == if_ && cur->next->data == openBrackets_) {
            printf("[+] if (\n");
            Token_Node* tmp = cur;

            _moss_if_statement(tmp);
        }
        //for statement
        else if (cur->data == for_ && cur->next->data == openBrackets_) {
            printf("[+] for (\n");
            Token_Node* tmp = cur;

            _moss_for_statement(tmp);
        }
        //PrintToken(cur->data);
        cur = cur->next;
    }

    return 0;
}

int _moss_if_statement(Token_Node* node) {
    if (node == NULL)
        return -1;
    
    while (node != NULL) {
        if (node->data == closeBrackets_) {
            printf("[+] if ()\n");
            break;
        }
        else if (node->data == eof_) {
            printf("[-] Error parsing if");
        }

        node = node->next;
    }
    return 0;
}

int _moss_for_statement(Token_Node* node) {
    if (node == NULL)
        return -1;
    
    while (node != NULL) {
        if (node->data == closeBrackets_) {
            printf("[+] for ()\n");
            break;
        }
        else if (node->data == eof_) {
            printf("[-] Error parsing for");
        }

        node = node->next;
    }
    return 0;
}