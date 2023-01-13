#ifndef TOKENIZE_H
#define TOKENIZE_H
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//6502 assembly tokenization for better formatting 
//Will be used to replace 'translate.c' later after this has been completed
//Still experimenting might be or not be used to be honest

//Enum of all possible tokens
typedef enum {
  //Instructions (> 1 byte opcodes)
  LDA_, LDX_, LDY_,
  STA_, STX_, STY_,
  ADC_, INC_, DEC_,
  ASL_, AND_, CMP_,
  CPX_, CPY_, JMP_,
  ROL_,
  //Instructions (1 byte opcodes)
  INX_, INY_, DEX_,
  DEY_, TAX_, TXA_,
  TAY_, TYA_, TXS_,
  TSX_, PHA_, PLA_,
  NOP_, SEC_, SED_,
  SEI_, CLC_, CLD_,
  CLI_,
  //Symbols
  HASHTAG_, DOLLARSIGN_,
  OPENBRACKET_, CLOSEBRACKET_,
  COMMA_, PERCENT_, SEMICOLON_,
  X_, Y_,
  //Numerical
  INT_BASE2, INT_BASE10, INT_BASE16,
  //Label identifiers
  LABEL_IDENTIFIER_,
  //End of file and unknown
  EOF_ = -1, UNKNOWN_,

} ASM6502_TOKENS;

//Important struct to store all the tokenized values to be later on parsed
typedef struct {
  ASM6502_TOKENS tok;
  char* value;
} Tokens;

//Initialize node
typedef struct node {
    Tokens data;
    struct node* next;
} Token_Node;

//Initialize list
typedef struct {
    Token_Node* head;
} Token_List;

//Read the file and return stream
void tokenize_file(char* path);

//=== Tokens list methods ===

//Initialize the list
Token_List* initList();

//Initialize a node
Token_Node* initNode();

//Add element to list
void add_tok_l(Token_List* list, Tokens data);

//Push to list
void push_tok_l(Token_List* list, Tokens data);

//Pop from list
int pop_tok_l(Token_List* list);

//Deallocate list
void free_tok_l(Token_List* list);

//=======

#endif