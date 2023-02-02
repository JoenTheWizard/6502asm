#include "string_manipulation.h"

//Check for empty strings
int isEmpty(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return 0;
    s++;
  }
  return 1;
}

//To trim the leading string (dealing with heap buffers, so cannot increment pointers)
void trim_string(char* str) {
    if (str == NULL)
        exit(EXIT_FAILURE);

    //Get the index to pad off
    int i = 0;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    
    //Shift all to the left
    int j = 0;
    if (i != 0) {
        //Set the position from the pad
        while (str[i + j] != '\0') {
            str[j] = str[j + i];
            j++;
        }
        
        //Null terminate
        str[j] = '\0';
    }
}

//Must remove whitespaces
void remove_spaces(char* s) {
    //Keep index of nonspace chars
    int nonspace = 0;
 
    //Traverse and implement if not space
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] != ' ')
            s[nonspace++] = s[i];    
    }
    //Null byte terminate
    s[nonspace] = '\0';
}

//To uppercase
void toUppercase(char* s) {
    for (int i = 0; s[i] != '\0';i++) {
        if (s[i] >= 'a' && s[i] <= 'z')
            s[i] = s[i] - 32;
    }
}