#include "translate.h"

//This is an ugly but nicely works implementation
//For translation of each instruction
//This can be improved but will do that later
//As for now 'tokenize.c/h' will contain simple tokenization for the assembly language
//translate.c/h will be used for now

//For strings and stuff
char* CheckRegisterOffset(char* op) {
    if (!op) return NULL;

    char *str1, *str2, *copy;
    str1 = strtok(op, ",");
    str2 = strtok(NULL, ",");
    //If tokenize is not null
    if (str2 != NULL) {
        copy = (char*)malloc(strlen(str2) + 1);
        if (!copy) return NULL;
        strcpy(copy, str2);
        return copy;
    }
    //If strtok returns null via delimiter return null
    //This needs to checked when free()
    return NULL;
}

//Store 2 byte instruction (will increment ROM once)
void Store2byte_instr(uint8_t op_code, uint8_t asm_[ASM_MEMORY],  char* op, int* Line) {
    //Store op code
    asm_[*Line] = op_code;
    (*Line)++;
    //Store byte
    asm_[*Line] = (uint8_t)strtol(op, NULL, 16);
}

//Stores 3 byte instruction (will increment ROM twice)
void Store3byte_instr(uint8_t op_code, uint8_t asm_[ASM_MEMORY], char* op, int* Line) {
    //Store op code
    asm_[*Line] = op_code;
    (*Line)++;
    uint16_t val = (uint16_t)strtol(op,NULL,16);
    asm_[*Line] = val & 0xFF;
    (*Line)++;
    asm_[*Line] = (val >> 8) & 0xFF;
}

//LDA
void LDA(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    if (*op == '#' && *op+1 == '$') {
        if (op[2] != '\0') {
            op+=2;
            Store2byte_instr(0xA9, assemble, op, LineInd);
        }
    } 
    else if (*op == '$') {
        if (op[1] != '\0') {
            op += 1;
            char* offSetReg = CheckRegisterOffset(op);
            //Register offsets
            if (offSetReg != NULL) {
                //Zero Page,X
                if (strlen(strtok(op,"\n")) <= 2 && offSetReg[0] == 'X') {
                    Store2byte_instr(0xB5, assemble, op, LineInd);
                }
                //Absolute,X
                else if (strlen(strtok(op,"\n")) <= 4 && offSetReg[0] == 'X') {
                    Store3byte_instr(0xBD, assemble, op, LineInd);
                }
                //Absolute,Y
                else if (strlen(strtok(op,"\n")) <= 4 && offSetReg[0] == 'Y') {
                    Store3byte_instr(0xB9, assemble, op, LineInd);
                }
                free(offSetReg);
            } 
            else {
                //Here to check Zero Page Mode
                if (strlen(strtok(op,"\n")) <= 2) {
                    Store2byte_instr(0xA5, assemble, op, LineInd);
                } else if (strlen(strtok(op,"\n")) <= 4) {
                    //Absolute mode
                    Store3byte_instr(0xAD, assemble, op, LineInd);
                }
            }
        }
    }
    //Indirect (both needs to implemented in assemble.h)
    else if (op[0] == '(' && op[1] == '$') {
        if (*op+2 != '\0') {
            op+=2;
            //Check for register offset
            char* offSetReg = CheckRegisterOffset(op);
            if (offSetReg != NULL) {
                //Indirect,X
                if (strlen(strtok(op,"\n")) <= 2 && (offSetReg[0] == 'X' && offSetReg[1] == ')')) {
                    //Store the 2 byte instruction
                    Store2byte_instr(0xA1, assemble, op, LineInd);
                }
                //Indirect,Y
                else if (strlen(strtok(op,"\n")) <= 3 && op[strlen(op)-1] == ')' && offSetReg[0] == 'Y') {
                    //printf("%c", op[strlen(op)-1]);
                    op[strlen(op)-1] = '\0';
                    Store2byte_instr(0xB1, assemble, op, LineInd);
                }
                free(offSetReg);
            }
        }
    }
}

//LDX
void LDX(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    //Immediate
    if (*op == '#' && *op+1 == '$') {
        op+=2;
        Store2byte_instr(0xA2, assemble, op, LineInd);
    }
    else if (*op == '$') {
        op++;
        //Zero page
        if (strlen(strtok(op,"\n")) <= 2) {
            Store2byte_instr(0xA6, assemble, op, LineInd);
        }
        //Absolute 
        else if (strlen(strtok(op,"\n")) <= 4) {
            Store3byte_instr(0xAE, assemble, op, LineInd);
        }
    }
}

//LDY
void LDY(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    if (*op == '#' && *op+1 == '$') {
        op+=2;
        Store2byte_instr(0xA0, assemble, op, LineInd);
    }
    else if (*op == '$') {
        op++;
        //Zero page
        if (strlen(strtok(op,"\n")) <= 2) {
            Store2byte_instr(0xA4, assemble, op, LineInd);
        }
        //Absolute 
        else if (strlen(strtok(op,"\n")) <= 4) {
            Store3byte_instr(0xAC, assemble, op, LineInd);
        }
    }
}

//STA
void STA(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    if (*op == '$') {
        if (op[1] != '\0') {
            op++;
            char* offSetReg = CheckRegisterOffset(op);
            if (offSetReg != NULL) { 
                //Absolute,X
                if (strlen(strtok(op,"\n")) <= 4)
                    Store3byte_instr(0x9D, assemble, op, LineInd);
                free(offSetReg);
            }
            else {
                //Zero Page
                if (strlen(strtok(op,"\n")) <= 2) {
                    Store2byte_instr(0x85, assemble, op, LineInd);
                }
                //Absolute
                else if (strlen(strtok(op,"\n")) <= 4) {
                    Store3byte_instr(0x8D, assemble, op, LineInd);
                }
            }
        }
    }
    //Indirect (both needs to implemented in assemble.h)
    else if (op[0] == '(' && op[1] == '$') {
        if (*op+2 != '\0') {
            op+=2;
            //Check for register offset
            char* offSetReg = CheckRegisterOffset(op);
            if (offSetReg != NULL) {
                //Indirect,X
                if (strlen(strtok(op,"\n")) <= 2 && (offSetReg[0] == 'X' && offSetReg[1] == ')')) {
                    //Store the 2 byte instruction
                    Store2byte_instr(0x81, assemble, op, LineInd);
                }
                //Indirect,Y
                else if (strlen(strtok(op,"\n")) <= 3 && op[strlen(op)-1] == ')' && offSetReg[0] == 'Y') {
                    //printf("%c", op[strlen(op)-1]);
                    op[strlen(op)-1] = '\0';
                    Store2byte_instr(0x91, assemble, op, LineInd);
                }
                free(offSetReg);
            }
        }
    }
}

//STX
void STX(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    if (*op == '$') {
        if (op[1] != '\0') {
            op++;
            char* offSetReg = CheckRegisterOffset(op);
            if (offSetReg != NULL) {
                //Zero Page,Y
                if (strlen(strtok(op,"\n")) <= 2 && offSetReg[0] == 'Y') {
                    Store2byte_instr(0x96, assemble, op, LineInd);
                }
                free(offSetReg);
            }
            else {
                //Zero Page
                if (strlen(strtok(op,"\n")) <= 2) {
                    Store2byte_instr(0x86, assemble, op, LineInd);
                }
                //Absolute
                else if (strlen(strtok(op,"\n")) <= 4) {
                    Store3byte_instr(0x8E, assemble, op, LineInd);
                }
            }
        }
    }
}

//STY
void STY(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    if (*op == '$') {
        if (op[1] != '\0') {
            op++;
            char* offSetReg = CheckRegisterOffset(op);
            if (offSetReg != NULL) {
                //Zero Page,X
                if (strlen(strtok(op,"\n")) <= 2 && offSetReg[0] == 'X') {
                    Store2byte_instr(0x94, assemble, op, LineInd);
                }
                free(offSetReg);
            }
            else {
                //Zero Page
                if (strlen(strtok(op,"\n")) <= 2) {
                    Store2byte_instr(0x84, assemble, op, LineInd);
                }
                //Absolute
                else if (strlen(strtok(op,"\n")) <= 4) {
                    Store3byte_instr(0x8C, assemble, op, LineInd);
                }
            }
        }
    }
}

//INX
void INX(uint8_t assemble[ASM_MEMORY], int* LineInd) {
    assemble[*LineInd] = 0xE8;
}

//INC
void INC(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    if (*op == '$') {
        if (*op+1 != '\0') {
            op++;
            char* offSetReg = CheckRegisterOffset(op);
            //Check for Register offsets
            if (offSetReg != NULL) {
                //Zero Page, X
                //printf("%s\n",offSetReg);
                if (strlen(strtok(op,"\n")) <= 2 && offSetReg[0] == 'X') {
                    Store2byte_instr(0xF6, assemble, op, LineInd);
                }
                //Absolute, X
                else if (strlen(strtok(op,"\n")) <= 4 && offSetReg[0] == 'X') {
                    Store3byte_instr(0xFE, assemble, op, LineInd);
                }
                free(offSetReg);
            }
            else {
            //Zero Page
                if (strlen(strtok(op,"\n")) <= 2) {
                    Store2byte_instr(0xE6, assemble, op, LineInd);
                }
                //Absolute
                else if (strlen(strtok(op,"\n")) <= 4) {
                    //Store the 3 byte instruction
                    Store3byte_instr(0xEE, assemble, op, LineInd);
                }
            }
        }
    }
}

//DEC
void DEC(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    if (*op == '$') {
        if (*op+1 != '\0') {
            op++;
            char* offSetReg = CheckRegisterOffset(op);
            //Check for Register offsets
            if (offSetReg != NULL) {
                //Zero Page, X
                //printf("%s\n",offSetReg);
                if (strlen(strtok(op,"\n")) <= 2 && offSetReg[0] == 'X') {
                    Store2byte_instr(0xD6, assemble, op, LineInd);
                }
                //Absolute, X
                else if (strlen(strtok(op,"\n")) <= 4 && offSetReg[0] == 'X') {
                    Store3byte_instr(0xDE, assemble, op, LineInd);
                }
                free(offSetReg);
            }
            else {
            //Zero Page
                if (strlen(strtok(op,"\n")) <= 2) {
                    Store2byte_instr(0xC6, assemble, op, LineInd);
                }
                //Absolute
                else if (strlen(strtok(op,"\n")) <= 4) {
                    Store3byte_instr(0xCE, assemble, op, LineInd);
                }
            }
        }
    }
}

//ADC
void ADC(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    if (*op == '$') {
        //printf("%s",op);
        if (*op+1 != '\0') {
            op++;
            char* offSetReg = CheckRegisterOffset(op);
            //Check for Register offsets
            if (offSetReg != NULL) {
                //Zero Page,X
                if (strlen(strtok(op,"\n")) <= 2 && offSetReg[0] == 'X') {
                    Store2byte_instr(0x75, assemble, op, LineInd);
                }
                //Absolute,X
                else if (strlen(strtok(op,"\n")) <= 4 && offSetReg[0] == 'X') {
                    Store3byte_instr(0x7D, assemble, op, LineInd);
                }
                //Absolute,Y
                else if (strlen(strtok(op,"\n")) <= 4 && offSetReg[0] == 'Y') {
                    Store3byte_instr(0x79, assemble, op, LineInd);
                }
                free(offSetReg);
            } 
            else {
                //Zero Page
                if (strlen(strtok(op,"\n")) <= 2) {
                    Store2byte_instr(0x65, assemble, op, LineInd);
                }
                //Absolute
                else if (strlen(strtok(op,"\n")) <= 4) {
                    Store3byte_instr(0x6D, assemble, op, LineInd);
                }
            }
        }
    }
    //Immediate 
    else if (*op == '#' && *op+1 == '$') {
        if (*op+2 != '\0') {
            op+=2;
            Store2byte_instr(0x69, assemble, op, LineInd);
        }
    }
    //Indirect (both needs to implemented in assemble.h)
    else if (op[0] == '(' && op[1] == '$') {
        if (*op+2 != '\0') {
            op+=2;
            //Check for register offset
            char* offSetReg = CheckRegisterOffset(op);
            if (offSetReg != NULL) {
                //Indirect,X
                if (strlen(strtok(op,"\n")) <= 2 && (offSetReg[0] == 'X' && offSetReg[1] == ')')) {
                    //Store the 2 byte instruction
                    Store2byte_instr(0x61, assemble, op, LineInd);
                }
                //Indirect,Y
                else if (strlen(strtok(op,"\n")) <= 3 && op[strlen(op)-1] == ')' && offSetReg[0] == 'Y') {
                    //printf("%c", op[strlen(op)-1]);
                    op[strlen(op)-1] = '\0';
                    Store2byte_instr(0x71, assemble, op, LineInd);
                }
                free(offSetReg);
            }
        }
    }
}

//ASL
void ASL(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    if (op != NULL) {
        //Accumulator
        if (op[0] == 'A') {
            assemble[*LineInd] = 0x0A;
        }
        else if (*op == '$') {
            if (op[1] != '\0') {
                op++;
                char* offSetReg = CheckRegisterOffset(op);
                if (offSetReg != NULL) {
                    //Zero Page,X
                    if (strlen(strtok(op,"\n")) <= 2 && offSetReg[0] == 'X')
                        Store2byte_instr(0x16, assemble, op, LineInd);
                    //Absolute,X
                    else if (strlen(strtok(op,"\n")) <= 4 && offSetReg[0] == 'X')
                        Store3byte_instr(0x1E, assemble, op, LineInd);
                    free(offSetReg);
                } 
                else {
                //Zero Page
                    if (strlen(strtok(op,"\n")) <= 2) {
                        Store2byte_instr(0x06, assemble, op, LineInd);
                    }
                    //Absolute
                    else if (strlen(strtok(op,"\n")) <= 4) {
                        Store3byte_instr(0x0E, assemble, op, LineInd);
                    }
                }
            }
        }
    }
}

//AND
void AND(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    if (*op == '$') {
        if (op[1] != '\0') {
            op++;
            char* offSetReg = CheckRegisterOffset(op);
            if (offSetReg != NULL) {
                //Zero Page,X
                if (strlen(strtok(op,"\n")) <= 2 && offSetReg[0] == 'X')
                    Store2byte_instr(0x35, assemble, op, LineInd);
                 //Absolute,X
                else if (strlen(strtok(op,"\n")) <= 4 && offSetReg[0] == 'X')
                    Store3byte_instr(0x3D, assemble, op, LineInd);
                //Absolute,Y
                else if (strlen(strtok(op,"\n")) <= 4 && offSetReg[0] == 'Y')
                    Store3byte_instr(0x39, assemble, op, LineInd);
                free(offSetReg);
            }
            else {
                //Zero Page
                if (strlen(strtok(op,"\n")) <= 2)
                    Store2byte_instr(0x25, assemble, op, LineInd);
                //Absolute
                else if (strlen(strtok(op,"\n")) <= 4)
                    Store3byte_instr(0x2D, assemble, op, LineInd);
            }
        }
    }
    else if (*op == '#' && *op+1 == '$') {
        if (op[2] != '\0') {
            op+=2;
            Store2byte_instr(0x29, assemble, op, LineInd);
        }
    }
    else if (op[0] == '(' && op[1] == '$') {
        if (*op+2 != '\0') {
            op+=2;
            //Check for register offset
            char* offSetReg = CheckRegisterOffset(op);
            if (offSetReg != NULL) {
                //Indirect,X
                if (strlen(strtok(op,"\n")) <= 2 && (offSetReg[0] == 'X' && offSetReg[1] == ')')) {
                    //Store the 2 byte instruction
                    Store2byte_instr(0x21, assemble, op, LineInd);
                }
                //Indirect,Y
                else if (strlen(strtok(op,"\n")) <= 3 && op[strlen(op)-1] == ')' && offSetReg[0] == 'Y') {
                    //printf("%c", op[strlen(op)-1]);
                    op[strlen(op)-1] = '\0';
                    Store2byte_instr(0x31, assemble, op, LineInd);
                }
                free(offSetReg);
            }
        }
    }
}

//CMP
void CMP(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    if (*op == '$') {
        if (op[1] != '\0') {
            op++;
            char* offSetReg = CheckRegisterOffset(op);
            if (offSetReg != NULL) {
                //Zero Page,X
                if (strlen(strtok(op,"\n")) <= 2 && offSetReg[0] == 'X')
                    Store2byte_instr(0xD5, assemble, op, LineInd);
                //Absolute,X
                else if (strlen(strtok(op,"\n")) <= 4 && offSetReg[0] == 'X')
                    Store3byte_instr(0xDD, assemble, op, LineInd);
                //Absolute,Y
                else if (strlen(strtok(op,"\n")) <= 4 && offSetReg[0] == 'Y')
                    Store3byte_instr(0xD9, assemble, op, LineInd);
                free(offSetReg);
            }
            else {
                //Zero page
                if (strlen(strtok(op,"\n")) <= 2)
                    Store2byte_instr(0xC5, assemble, op, LineInd);
                //Absolute
                else if (strlen(strtok(op,"\n")) <= 4)
                    Store3byte_instr(0xCD, assemble, op, LineInd);
            }
        }
    }
    else if (*op == '#' && *op+1 == '$') {
        if (op[2] != '\0') {
            op+=2;
            Store2byte_instr(0xC9, assemble, op, LineInd);
        }
    }
    else if (op[0] == '(' && op[1] == '$') {
        if (*op+2 != '\0') {
            op+=2;
            //Check for register offset
            char* offSetReg = CheckRegisterOffset(op);
            if (offSetReg != NULL) {
                //Indirect,X
                if (strlen(strtok(op,"\n")) <= 2 && (offSetReg[0] == 'X' && offSetReg[1] == ')')) {
                    //Store the 2 byte instruction
                    Store2byte_instr(0xC1, assemble, op, LineInd);
                }
                //Indirect,Y
                else if (strlen(strtok(op,"\n")) <= 3 && op[strlen(op)-1] == ')' && offSetReg[0] == 'Y') {
                    //printf("%c", op[strlen(op)-1]);
                    op[strlen(op)-1] = '\0';
                    Store2byte_instr(0xD1, assemble, op, LineInd);
                }
                free(offSetReg);
            }
        }
    }
}

//CPX
void CPX(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    if (*op == '$') {
        if (op[1] != '\0') {
            op++;
            //Zero page
            if (strlen(strtok(op,"\n")) <= 2)
                Store2byte_instr(0xE4, assemble, op, LineInd);
            //Absolute
            else if (strlen(strtok(op,"\n")) <= 4)
                Store3byte_instr(0xEC, assemble, op, LineInd);
        }
    }
    else if (*op == '#' && *op+1 == '$') {
        if (op[2] != '\0') {
            op+=2;
            Store2byte_instr(0xE0, assemble, op, LineInd);
        }
    }
}

//CPY
void CPY(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    if (*op == '$') {
        if (op[1] != '\0') {
            op++;
            //Zero page
            if (strlen(strtok(op,"\n")) <= 2)
                Store2byte_instr(0xC4, assemble, op, LineInd);
            //Absolute
            else if (strlen(strtok(op,"\n")) <= 4)
                Store3byte_instr(0xCC, assemble, op, LineInd);
        }
    }
    else if (*op == '#' && *op+1 == '$') {
        if (op[2] != '\0') {
            op+=2;
            Store2byte_instr(0xC0, assemble, op, LineInd);
        }
    }
}

//ROL
void ROL(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    if (op != NULL) {
        //Accumulator
        if (op[0] == 'A') {
            assemble[*LineInd] = 0x2A;
        }
        else if (*op == '$') {
            if (op[1] != '\0') {
                op++;
                char* offSetReg = CheckRegisterOffset(op);
                if (offSetReg != NULL) {
                    //Zero Page,X
                    if (strlen(strtok(op,"\n")) <= 2 && offSetReg[0] == 'X')
                        Store2byte_instr(0x36, assemble, op, LineInd);
                    //Absolute,X
                    else if (strlen(strtok(op,"\n")) <= 4 && offSetReg[0] == 'X')
                        Store3byte_instr(0x3E, assemble, op, LineInd);
                    free(offSetReg);
                } 
                else {
                    //Zero Page
                    if (strlen(strtok(op,"\n")) <= 2) {
                        Store2byte_instr(0x26, assemble, op, LineInd);
                    }
                    //Absolute
                    else if (strlen(strtok(op,"\n")) <= 4) {
                        Store3byte_instr(0x2E, assemble, op, LineInd);
                    }
                }
            }
        }
    }
}

//JMP
void JMP(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    //Absolute addressing
    if (*op == '$') {
        op++;
        if (strlen(strtok(op,"\n")) == 4 || strlen(strtok(op,"\n")) == 3) {
            assemble[*LineInd] = 0x4C;
            (*LineInd)++;
            uint16_t val = (int)strtol(op, NULL, 16);
            assemble[*LineInd] = val & 0xFF;
            (*LineInd)++;
            assemble[*LineInd] = (val >> 8) & 0xFF;
        }
    }
    //Indirect addressing
    else if (op[0] == '(' && op[1] == '$') {
        if (op[2] != '\0') {
            op+=2;
            char* op0 = strtok(op,"\n");
            //printf("%c ==\n",op0[strlen(op0)-1]);
            if ((strlen(op0) == 4 || strlen(op0) == 5) && op0[strlen(op0)-1] == ')') {
                assemble[*LineInd] = 0x6C;
                (*LineInd)++;
                op0[strlen(op0)-1] = '\0';
                uint16_t val = (uint16_t)strtol(op0,NULL,16);
                assemble[*LineInd] = val & 0xFF;
                (*LineInd)++;
                assemble[*LineInd] = (val >> 8) & 0xFF;
            }
        }
    }
}