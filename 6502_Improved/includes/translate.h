#include "defines.h"
//For translation of each instruction

//For strings and stuff
char* CheckRegisterOffset(char* op) {
    char* str = strtok(op,",");
    str = strtok(NULL,",");
    return str;
}

//LDA
void LDA(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    if (*op == '#' && *op+1 == '$') {
        if (op[2] != '\0') {
            assemble[*LineInd] = 0xA9;
            (*LineInd)++;
            op+=2;
            int val = (int)strtol(op, NULL, 16);
            assemble[*LineInd] = val;
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
                    assemble[*LineInd] = 0xB5;
                    (*LineInd)++;
                    assemble[*LineInd] = (uint8_t)strtol(op,NULL,16);
                }
                //Absolute,X
                else if (strlen(strtok(op,"\n")) <= 4 && offSetReg[0] == 'X') {
                    assemble[*LineInd] = 0xBD;
                    (*LineInd)++;
                    uint16_t val = (uint16_t)strtol(op,NULL,16);
                    assemble[*LineInd] = val & 0xFF;
                    (*LineInd)++;
                    assemble[*LineInd] = (val >> 8) & 0xFF;
                }
                //Absolute,Y
                else if (strlen(strtok(op,"\n")) <= 4 && offSetReg[0] == 'Y') {
                    assemble[*LineInd] = 0xB9;
                    (*LineInd)++;
                    uint16_t val = (uint16_t)strtol(op,NULL,16);
                    assemble[*LineInd] = val & 0xFF;
                    (*LineInd)++;
                    assemble[*LineInd] = (val >> 8) & 0xFF;
                }
            } 
            else {
                //Here to check Zero Page Mode
                if (strlen(strtok(op,"\n")) <= 2) {
                    assemble[*LineInd] = 0xA5;
                    (*LineInd)++;
                    int val = (int)strtol(op, NULL, 16);
                    assemble[*LineInd] = val;
                } else if (strlen(strtok(op,"\n")) <= 4) {
                    //Absolute mode
                    assemble[*LineInd] = 0xAD;
                    (*LineInd)++;
                    //Extract bits
                    uint16_t val = (int)strtol(op, NULL, 16);
                    assemble[*LineInd] = val & 0xFF;
                    (*LineInd)++;
                    assemble[*LineInd] = (val >> 8) & 0xFF;
                }
            }
        }
    }
}

//LDX
void LDX(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    //Immediate
    if (*op == '#' && *op+1 == '$') {
        assemble[*LineInd] = 0xA2;
        (*LineInd)++;
        op+=2;
        uint8_t val = (int)strtol(op, NULL, 16);
        assemble[*LineInd] = val;
    }
    else if (*op == '$') {
        op++;
        //Zero page
        if (strlen(strtok(op,"\n")) <= 2) {
            assemble[*LineInd] = 0xA6;
            (*LineInd)++;
            uint8_t val = (int)strtol(op, NULL, 16);
            assemble[*LineInd] = val;
        }
        //Absolute 
        else if (strlen(strtok(op,"\n")) <= 4) {
            assemble[*LineInd] = 0xAE;
            (*LineInd)++;
            uint16_t val = (int)strtol(op, NULL, 16);
            assemble[*LineInd] = val & 0xFF;
            (*LineInd)++;
            assemble[*LineInd] = (val >> 8) & 0xFF;
        }
    }
}

//LDY
void LDY(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    if (*op == '#' && *op+1 == '$') {
        assemble[*LineInd] = 0xA0;
        (*LineInd)++;
        op+=2;
        uint8_t val = (int)strtol(op, NULL, 16);
        assemble[*LineInd] = val;
    }
    else if (*op == '$') {
        op++;
        //Zero page
        if (strlen(strtok(op,"\n")) <= 2) {
            assemble[*LineInd] = 0xA4;
            (*LineInd)++;
            uint8_t val = (int)strtol(op, NULL, 16);
            assemble[*LineInd] = val;
        }
        //Absolute 
        else if (strlen(strtok(op,"\n")) <= 4) {
            assemble[*LineInd] = 0xAC;
            (*LineInd)++;
            uint16_t val = (int)strtol(op, NULL, 16);
            assemble[*LineInd] = val & 0xFF;
            (*LineInd)++;
            assemble[*LineInd] = (val >> 8) & 0xFF;
        }
    }
}

//STA
void STA(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd) {
    if (*op == '$') {
        op++;
        //Zero Page
        if (strlen(strtok(op,"\n")) <= 2) {
            assemble[*LineInd] = 0x85;
            (*LineInd)++;
            int val = (int)strtol(op, NULL, 16);
            assemble[*LineInd] = val;
        }
        //Absolute
        else if (strlen(strtok(op,"\n")) <= 4) {
            assemble[*LineInd] = 0x8D;
            (*LineInd)++;
            uint16_t val = (int)strtol(op, NULL, 16);
            assemble[*LineInd] = val & 0xFF;
            (*LineInd)++;
            assemble[*LineInd] = (val >> 8) & 0xFF;
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
                    assemble[*LineInd] = 0x96;
                    (*LineInd)++;
                    assemble[*LineInd] = (uint8_t)strtol(op,NULL,16);
                }
            }
            else {
                //Zero Page
                if (strlen(strtok(op,"\n")) <= 2) {
                    assemble[*LineInd] = 0x86;
                    (*LineInd)++;
                    int val = (int)strtol(op, NULL, 16);
                    assemble[*LineInd] = val;
                }
                //Absolute
                else if (strlen(strtok(op,"\n")) <= 4) {
                    assemble[*LineInd] = 0x8E;
                    (*LineInd)++;
                    uint16_t val = (int)strtol(op, NULL, 16);
                    assemble[*LineInd] = val & 0xFF;
                    (*LineInd)++;
                    assemble[*LineInd] = (val >> 8) & 0xFF;
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
                    assemble[*LineInd] = 0x94;
                    (*LineInd)++;
                    assemble[*LineInd] = (uint8_t)strtol(op,NULL,16);
                }
            }
            else {
                //Zero Page
                if (strlen(strtok(op,"\n")) <= 2) {
                    assemble[*LineInd] = 0x84;
                    (*LineInd)++;
                    int val = (int)strtol(op, NULL, 16);
                    assemble[*LineInd] = val;
                }
                //Absolute
                else if (strlen(strtok(op,"\n")) <= 4) {
                    assemble[*LineInd] = 0x8C;
                    (*LineInd)++;
                    uint16_t val = (int)strtol(op, NULL, 16);
                    assemble[*LineInd] = val & 0xFF;
                    (*LineInd)++;
                    assemble[*LineInd] = (val >> 8) & 0xFF;
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
                printf("%s\n",offSetReg);
                if (strlen(strtok(op,"\n")) <= 2 && offSetReg[0] == 'X') {
                    assemble[*LineInd] = 0xF6;
                    (*LineInd)++;
                    assemble[*LineInd] = (int)strtol(op,NULL,16);
                }
                //Absolute, X
                else if (strlen(strtok(op,"\n")) <= 4 && offSetReg[0] == 'X') {
                    assemble[*LineInd] = 0xFE;
                    (*LineInd)++;
                    uint16_t val = (int)strtol(op,NULL,16);
                    assemble[*LineInd] = val & 0xFF;
                    (*LineInd)++;
                    assemble[*LineInd] = (val >> 8) & 0xFF;
                } 
            }
            else {
            //Zero Page
                if (strlen(strtok(op,"\n")) <= 2) {
                    assemble[*LineInd] = 0xE6;
                    (*LineInd)++;
                    assemble[*LineInd] = (int)strtol(op,NULL,16);
                }
                //Absolute
                else if (strlen(strtok(op,"\n")) <= 4) {
                    assemble[*LineInd] = 0xEE;
                    (*LineInd)++;
                    uint16_t val = (int)strtol(op,NULL,16);
                    assemble[*LineInd] = val & 0xFF;
                    (*LineInd)++;
                    assemble[*LineInd] = (val >> 8) & 0xFF;
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
            //Zero Page
            if (strlen(strtok(op,"\n")) <= 2) {
                assemble[*LineInd] = 0x65;
                (*LineInd)++;
                int val = (int)strtol(op, NULL, 16);
                assemble[*LineInd] = val;
            }
            //Absolute
            else if (strlen(strtok(op,"\n")) <= 4) {
                assemble[*LineInd] = 0x6D;
                (*LineInd)++;
                uint16_t val = (int)strtol(op, NULL, 16);
                assemble[*LineInd] = val & 0xFF;
                (*LineInd)++;
                assemble[*LineInd] = (val >> 8) & 0xFF;
            }
        }
    }
    //Immediate 
    else if (*op == '#' && *op+1 == '$') {
        if (*op+2 != '\0') {
            op+=2;
            assemble[*LineInd] = 0x69;
            (*LineInd)++;
            assemble[*LineInd] = (uint8_t)strtol(op,NULL,16);
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