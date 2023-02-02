#ifndef STRING_MANIPULATION_H
#define STRING_MANIPULATION_H
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

//For string manipulation, reading and anything string related

//Check for empty strings
int isEmpty(const char *s);

//To trim the leading string (dealing with heap buffers, so cannot increment pointers)
void trim_string(char* str);

//Must remove whitespaces
void remove_spaces(char* s);

//To uppercase
void toUppercase(char* s);

#endif