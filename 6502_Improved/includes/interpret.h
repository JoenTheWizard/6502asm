#ifndef INTERPRET_H
#define INTERPRET_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokenize.h"

//Fixing the 6502 assembly interpret (as the current implementation is ugly and not efficient)
//Still being worked on and will use the tokenization.h as well which is also being worked on

//For this we need some reserverd keywords to interpret
//Which for most of them will just be the instructions
static struct {
    OP_MODES mode;
    char* op;
} tokens_k[] = {
    {0, "LDA"}, {0, "LDX"}, {0, "LDY"},
    {0 ,"STA"}, {0 ,"STX"}, {0, "STY"},
    {0, "ADC"}, {0, "INC"}, {0, "DEC"},
    {0, "ASL"}, {0, "AND"}, {0, "CMP"},
    {0, "CPX"}, {0, "CPY"}, {0, "JMP"},
    {0, "ROL"}, {0, "INX"}, {0, "INY"},
    {0, "DEX"}, {0, "DEY"}, {0, "TAX"},
    {0, "TXA"}, {0, "TAY"}, {0, "TYA"},
    {0, "TXS"}, {0, "TSX"}, {0, "PHA"},
    {0, "PLA"}, {0, "NOP"}, {0, "SEC"},
    {0, "SED"}, {0, "SEI"}, {0, "CLC"},
    {0, "CLD"}, {0, "CLI"},
};

//Interpreter. To interpret the file which will then be passed to translator for each instruction
void InterpretFile(char* filePath);

//String functions to check for empty strings and etc.
int isEmpty(const char *s);

//Trim leading whitespace
void trim_string(char* str);

//Get the OPcode
int isOPCode(char* lineInstr);

//Is an existing Opocode
int isValidOpcode(char* opcode);

#endif