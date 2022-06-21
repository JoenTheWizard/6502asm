//Im just going to try and write a simple 6502 assembly interpreter. Might implement
//graphics output too possibly (graphics.h, SDL, GTK are options).
//Simple hello world in 6502 asm: https://youtu.be/9hLGvLvTs1w
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "includes/register.h"
#include "includes/assemble.h"
#include "includes/defines.h"

void ReadRegs(REGISTER reg);
void PrintFile(const char* fileName, int isHex);
void PrintMemory(uint8_t* mems);

//String manipulation
char* remove_spaces(char* s);

//Monitor memory management
uint8_t* monitorMem;

//Assembly hexdump
uint8_t assembly[ASM_MEMORY];

//Get the line index from file
int LineIndex = 0;

//Labels list
#define LBL_LENGTH 64
typedef struct {
    char* name;
    int lineNumber;
    int size;
} LABELS;
LABELS* lblList;
void FreeLBLBuffers(LABELS* LBL_LIST, int length);

int main(int argc, char** argv) {
    //Initialize registers
    //Map memory address of 0x0200 to 0x05ff for pixel display
    //The stack would occupy addresses from 0x0100 to 0x01ff
    REGISTER regs = {
        .A = 0x00,
        .X = 0x00,
        .Y = 0x00,
        .StackPt = 0xFF, //Point to top of stack (255)
        .PC = 0x0000, //Point to mem adr of 0x0600 for assembly
        .Status = 0x00
    };

    if (argc < 2) {
        printf("Error! Usage: ./6502asm --build prog.asm\n");
        return -1;
    }
    else {
        if (argc == 2 && strcmp(argv[1],"regs")==0)
            ReadRegs(regs);
        if (strcmp(argv[1],"--test")==0) {
            
        }
        else if (argc >= 3) {
            if (strcmp(argv[1],"--read") == 0) {
                PrintFile(argv[2],1);
            }
            if (strcmp(argv[1],"--build") == 0){
                FILE* fbuild;
                char* instruction = malloc(1024*sizeof(char));
                size_t* size;
                //Once we try and build, init the monitor memory
                monitorMem = (uint8_t*)malloc(sizeof(uint8_t)*256);
                //Allocate for labels
                lblList = (LABELS*)malloc(sizeof(LABELS)*LBL_LENGTH);
                lblList->size = 0;

                if (fbuild = fopen(*(argv+2),"r"))
                {
                    while(getline(&instruction,size,fbuild) != -1)
                    {
                        
                        if (*instruction != '\n') 
                        {
                            char* op = strtok(instruction, " ");
                            if (strcmp(op,"LDA") ==0) {
                                //Load Accumulator register
                                //ASSEMBLY
                                //Immediate mode
                                op = strtok(NULL, " ");
                                if (*op == '#' && *op+1 == '$') {
                                    assembly[LineIndex] = 0xA9;
                                    LineIndex++;
                                    op+=2;
                                    int val = (int)strtol(op, NULL, 16);
                                    assembly[LineIndex] = val;
                                } 
                                else if (*op == '$') {
                                    op += 1;
                                    //Here to check Zero Page Mode
                                    if (strlen(strtok(op,"\n")) <= 2) {
                                        assembly[LineIndex] = 0xA5;
                                        LineIndex++;
                                        int val = (int)strtol(op, NULL, 16);
                                        assembly[LineIndex] = val;
                                    } else {
                                        //Absolute mode
                                        assembly[LineIndex] = 0xAD;
                                        LineIndex++;
                                        char t[3] = {op[0],op[1],'\0'};
                                        int val = (int)strtol(t, NULL, 16);
                                        op += 2;
                                        int val1 = (int)strtol(op, NULL, 16);
                                        assembly[LineIndex] = val1;
                                        LineIndex++;
                                        assembly[LineIndex] = val;
                                    }
                                }
                            }
                            else if (strcmp(op,"STA")==0) {
                                op = strtok(NULL, " ");
                                if (*op == '$') {
                                    op++;
                                    if (strlen(strtok(op,"\n")) <= 2) {
                                        assembly[LineIndex] = 0x85;
                                        LineIndex++;
                                        int val = (int)strtol(op, NULL, 16);
                                        assembly[LineIndex] = val;
                                    }
                                }
                            }
                            else if (strcmp(op, "LDX")==0) {
                                //Load X register
                                //ASSEMBLY ABSOLUTE
                                assembly[LineIndex] = 0xA2;
                                regs.PC++;
                            } 
                            else if (strcmp(op, "LDY")==0) {
                                //Load Y register
                                //ASSEMBLY ABSOLUTE
                                assembly[LineIndex] = 0xA0;
                                regs.PC++;
                            }
                            else {
                                //Labels
                                op = strtok(op, ":");

                                //Add the labels to the list then increment index
                                lblList[lblList->size].lineNumber = LineIndex;
                                lblList[lblList->size].name = (char*)malloc(sizeof(char)*strlen(op)+1);
                                strncpy(lblList[lblList->size].name,op,strlen(op));

                                printf("%s: %i\n", lblList[lblList->size].name, lblList[lblList->size].lineNumber);

                                lblList->size++;
                                //printf("%i\n",lblList->size);
                            }
                            LineIndex++;
                        }
                    }
                    //ASSEMBLE
                    printf("\nDISASSEMBLE\n");
                    ASM(LineIndex,assembly,&regs,monitorMem);

                    //Monitor memory (Zero page)
                    printf("ZERO PAGE\n");
                    PrintMemory(monitorMem);

                    //Read the registers
                    ReadRegs(regs);
                    fclose(fbuild);

                } else
                     printf("ERROR: Error with finding file: %s\n", argv[2]);
                if (instruction)
                    free(instruction);

                //Free the monitor memory
                free(monitorMem);
                //Free the labels list
                FreeLBLBuffers(lblList, lblList->size);
                free(lblList);
            }
        }
    }
    return 0;
}

void ReadRegs(REGISTER reg) {
    printf("A: %X\nX: %X\nY: %X\nSTCKPTR: %X\nPROG COUNT: 0x%04X\nSTATUS: %X\n\n",
        reg.A, reg.X, reg.Y, reg.StackPt, reg.PC, reg.Status);
}

//Read or print file (not related to CPU just testing, has hex printing to)
void PrintFile(const char* fileName, int isHex){
    char txtbuff[2048];
    FILE* fasm;
    if (fasm = fopen(fileName, "r")) {
        char ch;
        int ct = 1;
        uint16_t adr = 0x0000;
        while ((ch = fgetc(fasm)) != EOF) {
            if (isHex) {
                if (ct==1)
                    printf("%04x: ",adr);
                if (ct%16==0) {
                    printf("%02x\n%04x: ",ch,adr+=16);
                } else
                    printf("%02x ",ch);
                ct++;
            } else
                printf("%c",ch);
        }
        fclose(fasm);
    } else
        printf("ERROR: Error with finding file: %s\n", fileName);
}

//This will just print the zero page memory at the moment (0-255 bytes)
void PrintMemory(uint8_t* mems)
{
    uint16_t adr = 0x0000;
    printf("%04x: %02x ",adr,mems[0]);
    for (int i = 1; i < 256; i++) {
        if (i%16==15)
            printf("%02x\n%04x: ",mems[i],adr+=16);
        else
            printf("%02x ",mems[i]);
    }
    printf("%c",'\n');
}

//Just to fix one word opcodes
char* remove_spaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
    return s;
}
//Free the name buffers
void FreeLBLBuffers(LABELS* LBL_LIST, int length) {
    for (int i = 0; i < length; i++)
        free(LBL_LIST[i].name);
}