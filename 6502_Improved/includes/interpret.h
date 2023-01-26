#ifndef INTERPRET_H
#define INTERPRET_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Fixing the 6502 assembly interpret (as the current implementation is ugly and not efficient)
//Still being worked on and will use the tokenization.h as well which is also being worked on

//Interpreter. To interpret the file which will then be passed to translator for each instruction
void InterpretFile(char* filePath);

//String functions to check for empty strings and etc.
int isEmpty(const char *s);

//Trim leading whitespace
void trim_string(char* str);

//Get the OPcode
int isOPCode(char* lineInstr);

#endif