#include <stdio.h>
#include "includes/Tokenizer.h"

int main(int argc, char** argv) {
    //Tokenize, aka obtain each character and interpret
    //what type of token they are within the specified file
    if (argc >= 2)
        //printf("%s",argv[1]);
        Tokenize(argv[1]);
    else
        printf("ERROR: Usage ./moss <path-to-file>\n");
    return 0;
}
