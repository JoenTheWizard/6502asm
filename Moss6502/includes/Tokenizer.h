#pragma once
#include "defines.h"

typedef enum {
    unknown_,
    identifier_,
    number_,
    floatpoint_,
    //Keywords
    if_,
    else_,
    for_,
    while_,
    struct_,
    void_,
    return_,
    break_,
    do_,
    typedef_,
    continue_,
    enum_,
    //Symbols
    openBrackets_,
    closeBrackets_,
    openBraces_,
    closeBraces_,
    equals_,
    greaterThan_,
    greaterThanEqual_,
    lessThan_,
    lessThanEqual_,
    equalEqual_,
    semicolon_,
    //Data types
    int_,
    char_,
    float_,
    double_,
    short_,
    plus_,
    plusplus_,
    minus_,
    minusminus_,
    //Macro
    macro_,
    eof_ = -1
} Tokens;

typedef struct {
  char* keyword;
  Tokens tok;
} KEYWORDS;


void PrintToken(int index);

Tokens check_keyword(const char* tokn);

void removeWhitespace(char* ch, int* index);

void Tokenize(const char* fileName);