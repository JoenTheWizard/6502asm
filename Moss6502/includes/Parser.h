#ifndef PARSER_H
#define PARSER_H
#include "TokenList.h"
#include "Tokenizer.h"

//Parse the general statements
int _moss_parse_statements(Token_List* list);

//Parse if statements
int _moss_if_statement(Token_Node* node);

//Math parser
int _moss_for_statement(Token_Node* node);

#endif