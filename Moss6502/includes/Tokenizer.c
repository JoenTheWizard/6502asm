#include "Tokenizer.h"
#include "TokenList.h"
#include "Parser.h"

char c;
char txtbuff[BUFSIZE];

const char* token_str[] = 
{
    "unknown_",
    "identifier_",
    "number_",
    "floatpoint_",
    //Keywords
    "if_",
    "else_",
    "for_",
    "while_",
    "struct_",
    "void_",
    "return_",
    "break_",
    "do_",
    "typedef_",
    "continue_",
    "enum_",
    //Symbols
    "openBrackets_",
    "closeBrackets_",
    "openBraces_",
    "closeBraces_",
    "equals_",
    "greaterThan_",
    "greaterThanEqual_",
    "lessThan_",
    "lessThanEqual_",
    "equalEqual_",
    "semicolon_",
    //Data types
    "int_",
    "char_",
    "float_",
    "double_",
    "short_",
    "plus_",
    "plusplus_",
    "minus_",
    "minusminus_",
    "macro_",
    "eof_"
};

KEYWORDS keywords[] = {
  //Structural keywords
  {"if", if_}, {"else", else_}, {"for", for_},
  {"while", while_}, {"struct", struct_}, {"void", void_},
  {"return", return_}, {"break", break_}, {"do", do_},
  {"typedef", typedef_}, {"continue", continue_},
  {"enum", enum_},

  //Data types
  {"int", int_}, {"char", char_}, {"float", float_},
  {"double", double_}, {"short", short_},
};

Token_List* _tk_list;

//Printing the token to help with debugging
void PrintToken(int index) {
    //Make sure to index eof_ as it stores val of -1
    printf("%s\n", token_str[index == -1 ? sizeof(token_str)/sizeof(token_str[0]) - 1 : index]);
}

Tokens check_keyword(const char* tokn)
{
  Tokens tk = identifier_;
  
  for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
    if (!strcmp(tokn, keywords[i].keyword))
      tk = keywords[i].tok;
  }
   return tk;
}

//Removing whitespace within reading the Moss file for obtaining each of the tokens
void removeWhitespace(char* ch, int* index) {
    while (*ch == ' ' || *ch == '\t')
        *ch = txtbuff[++(*index)];
}

void Tokenize(const char* fileName){
    //Allocate buffer
    Tokens token = unknown_;
    FILE* fasm;

    int ind = 0;
    if (fasm = fopen(fileName, "r")) {
        char ch;
       
        //Init the list
        _tk_list = initList();

        while ((ch = fgetc(fasm)) != EOF) {
            txtbuff[ind++] = ch;
        }

        char currentTokString[255];
        txtbuff[ind] = eof_;
        ind = 0;
        c = txtbuff[ind];

        //Tokenize code while not EOF
        while (c != eof_) 
        {
            c = txtbuff[ind];
            //Remove whitespace (just change the value to next char)
            removeWhitespace(&c, &ind);

            //Check for keywords and variables
            if (isalpha(c))
            {
               int i = 0;
               while (isalpha(c) || isdigit(c) || c == '_')
               {
                  token = identifier_;
                  currentTokString[i++] = c;
                  c = txtbuff[++ind];
                  //printf("== %c\n",c);
               } 
               currentTokString[i] = '\0';
               token = check_keyword(currentTokString);
               //PrintToken(token);
               add_tok_l(_tk_list, token);
            }
            //Check for numeric values (ints, floats, etc)
            else if (isdigit(c)) {
                while (isdigit(c)) {
                    token = number_;
                    c = txtbuff[++ind];
                }
                //Floats
                if (c == '.') {
                    c = txtbuff[++ind];
                    while (isdigit(c)) {
                        token = floatpoint_;
                        c = txtbuff[++ind];
                    }
                }
                add_tok_l(_tk_list, token);
            }
            else {
                switch (c) {
                    case '(':
                        token = openBrackets_;
                        c = txtbuff[++ind];
                        //PrintToken(token);
                        add_tok_l(_tk_list, token);
                        break;
                    case ')':
                        token = closeBrackets_;
                        c = txtbuff[++ind];
                        //PrintToken(token);
                        add_tok_l(_tk_list, token);
                        break;
                    case '{':
                        token = openBraces_;
                        c = txtbuff[++ind];
                        //PrintToken(token);
                        add_tok_l(_tk_list, token);
                        break;
                    case '}':
                        token = closeBraces_;
                        c = txtbuff[++ind];
                        //PrintToken(token);
                        add_tok_l(_tk_list, token);
                        break;
                    case '=':
                        token = equals_;
                        c = txtbuff[++ind];
                        if (c == '=') {
                            token = equalEqual_;
                            c = txtbuff[++ind];
                        }
                        //PrintToken(token);
                        add_tok_l(_tk_list, token);
                        break;
                    case ';':
                        token = semicolon_;
                        c = txtbuff[++ind];
                        //PrintToken(token);
                        add_tok_l(_tk_list, token);
                        break;
                    case '#':
                        token = macro_;
                        c = txtbuff[++ind];
                        add_tok_l(_tk_list, token);
                    case '>':
                        token = greaterThan_;
                        c = txtbuff[++ind];
                        if (c == '=') {
                            token = greaterThanEqual_;
                            c = txtbuff[++ind];
                        }
                        //PrintToken(token);
                        add_tok_l(_tk_list, token);
                        break;
                    case '<':
                        token = lessThan_;
                        c = txtbuff[++ind];
                        if (c == '=') {
                            token = lessThanEqual_;
                            c = txtbuff[++ind];
                        }
                        //PrintToken(token);
                        add_tok_l(_tk_list, token);
                        break;
                    case '+':
                      token = plus_;
                      c = txtbuff[++ind];
                      if (c == '+') {
                        token = plusplus_;
                        c = txtbuff[++ind];
                      }
                      //PrintToken(token);
                      add_tok_l(_tk_list, token);
                      break;
                    case '-':
                      token = minus_;
                      c = txtbuff[++ind];
                      if (c == '-') {
                        token = minusminus_;
                        c = txtbuff[++ind];
                      }
                      //PrintToken(token);
                      add_tok_l(_tk_list, token);
                      break;
                    default:
                        ind++;
                        break;
                }
            }
        }

        //Make sure to set EOF to each Moss program!
        add_tok_l(_tk_list, eof_);

        _moss_parse_statements(_tk_list);

        #pragma region PRINT_TOKS
        #ifdef DEBUG
        printf("\n============ TOKEN LIST ============\n");

        Token_Node* cur = _tk_list->head;
        
        while (cur != NULL) {
            PrintToken(cur->data);
            cur = cur->next;
        }
        printf("===========================\n");
        #endif
        #pragma endregion

        free_tok_l(_tk_list);
        //printf("%i\n",index);
        fclose(fasm);
    } else
        printf("MOSS ERROR: Error with finding file: %s\n", fileName);
}

