#include "interpret.h"

//Store the instructions to read line by line
char* lineInstructions;

//Store each line number
int LineNumber;

//Store byte index
int byteIndex;

//Simple interpretation of 6502 assembly language
void InterpretFile(char* filePath) {
    //Read the file and other init
    FILE* fbuild;
    lineInstructions = NULL;
    size_t len = 0;
    ssize_t read;

    //Line Number init
    LineNumber = 1;

    //Read file at path
    fbuild = fopen(filePath, "r");
    //If unable to read file
    if (fbuild == NULL) {
        fprintf(stderr, "[-] 6502asm is unable to read file '%s'", filePath);
        exit(EXIT_FAILURE);
    }
    
    //Read each line by line
    while ((read = getline(&lineInstructions, &len, fbuild)) != -1) {
        //Make sure it's not empty
        if (!isEmpty(lineInstructions)) {
            //Must trim the leading whitespace from string
            trim_string(lineInstructions);

            //Remove new line characters
            lineInstructions[strcspn(lineInstructions, "\n")] = '\0';

            //Obtain the Instruction opcode
            //If line is an operational instruction
            if (isOPCode(lineInstructions)) {
                //Get the op code string later for parsing
                char getOPCode[4];
                memcpy(getOPCode, lineInstructions, 4);
                getOPCode[3] = '\0';

                //Is the Opcode we're passing valid?
                if (isValidOpcode(getOPCode)) {
                    //Obtain arguments by padding out the opcode string
                    char* instr_cp = (char*)malloc(read * sizeof(char));
                    memcpy(instr_cp, &lineInstructions[3], read);

                    //Remove any leading whitespaces from the instruction argument
                    trim_string(instr_cp);

                    //Must check for empty argument for argument parsing
                    if (strlen(instr_cp) != 0) {
                        //printf("%d %d %s %s\n", LineNumber, strlen(instr_cp), getOPCode, instr_cp);
                        //If argument is not label but uses these characters
                        if (*instr_cp == '$' || *instr_cp == '#' || *instr_cp == '%') {
                            //Check what mode the argument is
                            tokenize_args(instr_cp);
                        }
                    }

                    //Deallocate
                    free(instr_cp);
                }
                //Otherwise error
                else {
                    fprintf(stderr, "[-] 6502asm raised an error at line %d. Unrecognized opcode '%s'\n", LineNumber, getOPCode);
                    break;
                }
            }
        }
        //Increment for each line
        LineNumber++;
    }

    //Deallocation
    fclose(fbuild);
    if (lineInstructions)
        free(lineInstructions);
    exit(EXIT_SUCCESS);
}

//Return the opcode (without strtok usage)
int isOPCode(char* lineInstr) {
    //Gain count
    int i = 0;
    while (lineInstr[i] != '\0') {
        if (lineInstr[i] == ' ')
            break;
        i++;
    }
    //Check if an opcode
    //printf(i == 3 ? " AN OPCODE" : " NOT AN OPCODE");
    //printf("\n");

    return i == 3 ? 1 : 0;
}

//Check if the parsed "opcode" is valid
int isValidOpcode(char* opcode) {
    for (int i = 0; i < sizeof(tokens_k)/sizeof(tokens_k[0]); i++) {
        //If found then is true
        if (!strncmp(opcode, tokens_k[i].op, 4))
            return 1;
    }
    //Otherwise not
    return 0;
}

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