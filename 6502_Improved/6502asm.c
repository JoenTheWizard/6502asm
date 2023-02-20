//Im just going to try and write a simple 6502 assembly interpreter. Might implement
//TODO: Labels, binary representation ('%'), rest of the opcodes

//Simple hello world in 6502 asm: https://youtu.be/9hLGvLvTs1w
#include <pthread.h>
#include <assert.h>
#include "includes/defines.h"
#include "includes/interpret.h"

void PrintFile(const char* fileName, int isHex);

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
    regs.Status |= U;

    //Set the break flag if BREAK is enabled
    #ifdef BREAK
    regs.Status |= B;
    #endif

    if (argc < 2) {
        printf("Error! Usage: ./6502asm --build prog.asm\n");
        return -1;
    }
    else {
        if (argc == 2 && strcmp(argv[1],"regs")==0)
            ReadRegs(&regs);
        if (argc == 2 && !strcmp(argv[1],"--help"))
            printf(HELP);
        if (!strcmp(argv[1],"--test")) {
            printf("This is just used for debugging.\n");

            //InterpretFile("test1.asm");
            //tokenize_file("test.asm");
        }
        else if (argc >= 3) {
            if (!strcmp(argv[1],"--read")) {
                PrintFile(argv[2],1);
            }
            if (!strcmp(argv[1],"--build") || !strcmp(argv[1],"-b")) {
                //Check for GUI mode
                int gui = 0;
                if (argc >= 4) {
                    if (!strcmp(argv[3], "-v") || !strcmp(argv[3], "--visual"))
                        gui = 1;
                }
                //Interpret the 6502 assembly file
                //Translates the assembly to opcodes
                Interpret6502asm(argv[2], &regs, gui);
            }
        }
    }
    return 0;
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