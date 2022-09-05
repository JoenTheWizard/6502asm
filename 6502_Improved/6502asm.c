//Im just going to try and write a simple 6502 assembly interpreter. Might implement
//graphics output too possibly (graphics.h, SDL, GTK are options).
//Simple hello world in 6502 asm: https://youtu.be/9hLGvLvTs1w
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include "includes/register.h"
#include "includes/translate.h"
#include "includes/assemble.h"
#include "includes/gui.h"
#include "includes/defines.h"

void ReadRegs(REGISTER reg);
void PrintFile(const char* fileName, int isHex);
void PrintMemory(uint8_t* mems);

//String manipulation
char* remove_spaces(char* s);
char* toUppercase(char* s);

//Take into account singular opcode
int strcmp_singular(char* op, char* c1, char* c2);

//Monitor memory management
uint8_t* monitorMem;

//Assembly hexdump
uint8_t assembly[ASM_MEMORY];

//Get the line index from file
int LineIndex = 0;

//Check if instruction is a label or not
int isLabel = 0;

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
        .PC = 0x0600, //Point to mem adr of 0x0600 for assembly
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
            printf("This is just used for debugging.\n");
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
                monitorMem = (uint8_t*)malloc(sizeof(uint8_t)*ROM);
                //Allocate for labels
                lblList = (LABELS*)malloc(sizeof(LABELS)*LBL_LENGTH);
                assert(lblList != NULL);
                lblList->size = 0;

                if (fbuild = fopen(*(argv+2),"r"))
                {
                    while(getline(&instruction,size,fbuild) != -1)
                    {
                        isLabel = 0;
                        if (*instruction != '\n') 
                        {
                            char* op = strtok(instruction, " ");
                            //Check for whitespaces!
                            if (op != NULL) {
                                if (!strcmp(toUppercase(op),"LDA")) {
                                    //Load Accumulator register
                                    op = strtok(NULL, " ");
                                    LDA(assembly, op, &LineIndex);
                                }
                                else if (!strcmp(toUppercase(op), "LDX")) {
                                    //Load X register
                                    op = strtok(NULL, " ");
                                    LDX(assembly, op, &LineIndex);
                                } 
                                else if (!strcmp(toUppercase(op), "LDY")) {
                                    //Load Y register
                                    op = strtok(NULL, " ");
                                    LDY(assembly, op, &LineIndex);
                                }
                                else if (!strcmp(toUppercase(op),"STA")) {
                                    //Store accumulator
                                    op = strtok(NULL, " ");
                                    STA(assembly,op,&LineIndex);
                                }
                                else if (!strcmp(toUppercase(op),"STX")) {
                                    //Store X register
                                    op = strtok(NULL, " ");
                                    STX(assembly,op,&LineIndex);
                                }
                                else if (!strcmp(toUppercase(op),"STY")) {
                                    //Store Y register
                                    op = strtok(NULL, " ");
                                    STY(assembly,op,&LineIndex);
                                }
                                else if (!strcmp(toUppercase(op),"ADC")) {
                                    //Add with Carry to A register
                                    op = strtok(NULL," ");
                                    ADC(assembly,op,&LineIndex);
                                }
                                else if (!strcmp(toUppercase(op),"INC")) {
                                    //Increment at memory location
                                    op = strtok(NULL, " ");
                                    INC(assembly,op,&LineIndex);
                                }
                                else if (!strcmp(toUppercase(op),"DEC")) {
                                    //Decrement at memory location
                                    op = strtok(NULL, " ");
                                    DEC(assembly,op,&LineIndex);
                                }
                                else if (!strcmp(toUppercase(op),"ASL")) {
                                    //Shift bits to 1 left
                                    op = strtok(NULL, " ");
                                    ASL(assembly,op,&LineIndex);
                                }
                                else if (strcmp_singular(toUppercase(op),"INX","INX\n")) {
                                    //Increment X register
                                    assembly[LineIndex] = 0xE8;
                                }
                                else if (strcmp_singular(toUppercase(op),"INY","INY\n")) {
                                    //Increment Y register
                                    assembly[LineIndex] = 0xC8;
                                }
                                else if (strcmp_singular(toUppercase(op),"DEX","DEX\n")) {
                                    //Decrement X register
                                    assembly[LineIndex] = 0xCA;
                                }
                                else if (strcmp_singular(toUppercase(op),"DEY","DEY\n")) {
                                    //Decrement Y regsiter
                                    assembly[LineIndex] = 0x88;
                                }
                                else if (strcmp_singular(toUppercase(op),"TAX","TAX\n")) {
                                    //Transfer A to X register
                                    assembly[LineIndex] = 0xAA;
                                }
                                else if (strcmp_singular(toUppercase(op),"TXA","TXA\n")) {
                                    //Transfer X to A register
                                    assembly[LineIndex] = 0x8A;
                                }
                                else if (strcmp_singular(toUppercase(op),"TAY","TAY\n")) {
                                    //Transfer A to Y register
                                    assembly[LineIndex] = 0xA8;
                                }
                                else if (strcmp_singular(toUppercase(op),"TYA","TYA\n")) {
                                    //Transfer Y to A register
                                    assembly[LineIndex] = 0x98;
                                }
                                else if (strcmp_singular(toUppercase(op),"NOP","NOP\n")) {
                                    //NOP instruction
                                    assembly[LineIndex] = 0xEA;
                                }
                                else if (!strcmp(toUppercase(op),"JMP")) {
                                    //Jump to an address in assembly
                                    op = strtok(NULL, " ");
                                    JMP(assembly,op,&LineIndex);
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

                                    //Is a label
                                    isLabel = 1;
                                }
                                if (!isLabel)
                                    LineIndex++;
                            }
                        }
                    }
                    //ASSEMBLE
                    ASM(LineIndex,assembly,&regs,monitorMem);

                    //Monitor memory (Zero page)
                    printf("ZERO PAGE\n");
                    PrintMemory(monitorMem);

                    //SDL menu
                    if (argc >= 4) {
                    //Graphics output
                        if (!strcmp(argv[3],"-v")||!strcmp(argv[3],"--visual")) {
                            //pthread_t thread_id;
                            //pthread_create(&thread_id, NULL, OpenGUI, monitorMem);
                            //pthread_join(thread_id, NULL);
                            OpenGUI(monitorMem);
                        }
                    }

                    //Read the registers
                    ReadRegs(regs);
                    fclose(fbuild);
                    pthread_exit(NULL);

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
    for (int i = 1; i < ROM; i++) {
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
        while (*d == '\n') {
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

int strcmp_singular(char* op, char* c1, char* c2) {
    return !strcmp(op, c1)||!strcmp(op,c2);
}

char* toUppercase(char* s) {
    for (int i = 0; s[i] != '\0';i++) {
        if (s[i] >= 'a' && s[i] <= 'z')
            s[i] = s[i] - 32;
    }
    return s;
}