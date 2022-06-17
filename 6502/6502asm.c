//Im just going to try and write a simple 6502 assembly interpreter. Might implement graphics output too possibly (graphics.h, SDL, GTK are options).
//This is very unfinished but I am planning to add more later
//So far there are only a couple of opcodes and instruction implemented such as storing in a register, storing in memory and incrementing and decrementing
//To use the program, you would need a simple file:
//./6502asm --build <6502assembly>
//This will only output the zero page memory and register values at the moment

//Example 6502 assembly:
//LDA #$02
//STA $0000

//LDA #$05
//STA $0001

//ADC $0000
//STA $0003

//LDA $0001
//ADC $0003
//STA $0008

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

//Unix-based console clear only (uses ANSI escape code)
#define CNSCLR() printf("\033[H\033[J")

//Registers
typedef struct{
    uint8_t A; //accum reg
    uint8_t X; //X reg
    uint8_t Y; //Y reg
    uint8_t StackPt; //Stack pointer reg
    uint16_t PC; //Program counter reg
    uint8_t Status; //Status reg (this will be modified by the flags)
} REGISTER;
//These flags will be used to set certain modes for the 6502
enum FLAGS {
    C = (1 << 0), //Carry bit
    Z = (1 << 1), //Zero
    I = (1 << 2), //Disable Interrupts
    D = (1 << 3), //Decimal mode
    B = (1 << 4), //Break
    U = (1 << 5), //Unused
    V = (1 << 6), //Overflow
    N = (1 << 7), //Negative
};

void ReadRegs(REGISTER reg);
void PrintFile(const char* fileName, int isHex);
void LoadRegister(char* operation, uint8_t* regVal, uint8_t* mems);
void PrintMemory(uint8_t* mems);

//String manipulation
char* remove_spaces(char* s);

//Monitor memory management
uint8_t* monitorMem;

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
            //For monitor memory management in the 6502
                // char *string1;
                // string1 = malloc(sizeof(char)*256);
                // strcpy(string1, "hello");
                // string1[1] = '4';
                // string1[10] = 'k';
                // for (int i = 0; i < 0xff; i++)
                //     printf("%02x ",string1[i]);

                // free(string1);
        }
        else if (argc >= 3) {
            if (strcmp(argv[1],"--read") == 0) {
                PrintFile(argv[2],1);
            }
            if (strcmp(argv[1],"--build") == 0){
                FILE* fbuild;
                char* instruction = NULL;
                size_t* size;
                //Once we try and build, init the monitor memory
                monitorMem = (uint8_t*)malloc(sizeof(uint8_t)*256);

                if (fbuild = fopen(*(argv+2),"r"))
                {
                    while(getline(&instruction,size,fbuild) != -1)
                    {
                        if (*instruction != '\n') 
                        {
                            char* op = strtok(instruction, " ");
                            if (strcmp(op,"LDA") ==0) {
                                //Load Accumulator register
                                LoadRegister(op, &regs.A,monitorMem);
                                ReadRegs(regs);
                            }
                            else if (strcmp(op, "LDX")==0) {
                                //Load X register
                                LoadRegister(op, &regs.X,monitorMem);
                                ReadRegs(regs);
                            } 
                            else if (strcmp(op, "LDY")==0) {
                                //Load Y register
                                LoadRegister(op, &regs.Y,monitorMem);
                                ReadRegs(regs);
                            }
                            else if (strcmp(op, "STA")==0)
                            {
                                op = strtok(NULL, " ");
                                //If value is in hex
                                if (*op == '$'){
                                    op+=1;
                                    int val = (int)strtol(op, NULL, 16);
                                    monitorMem[val] = regs.A;
                                    //printf("%02x", monitorMem[1]);
                                }
                            }
                            else if (strcmp(op,"STX")==0)
                            {
                                op = strtok(NULL, " ");
                                //If value is in hex
                                if (*op == '$'){
                                    op+=1;
                                    int val = (int)strtol(op, NULL, 16);
                                    monitorMem[val] = regs.X;
                                    //printf("%02x", monitorMem[1]);
                                }
                            }
                            else if (strcmp(op,"STY")==0)
                            {
                                op = strtok(NULL, " ");
                                //If value is in hex
                                if (*op == '$'){
                                    op+=1;
                                    int val = (int)strtol(op, NULL, 16);
                                    monitorMem[val] = regs.Y;
                                    //printf("%02x", monitorMem[1]);
                                }
                            }
                            else if (strcmp(op,"ADC")==0)
                            {
                                op = strtok(NULL, " ");
                                //If value is in hex
                                if (*op == '$'){
                                    op+=1;

                                    int val = (int)strtol(op, NULL, 16);
                                    regs.A += monitorMem[val];
                                }
                            }
                            else if (strcmp(op,"INC")==0){
                                //Incrementing at a memory address
                                op = strtok(NULL, " ");
                                //If value is in hex
                                if (*op == '$'){
                                    op+=1;
                                    int val = (int)strtol(op, NULL, 16);
                                    monitorMem[val]++;
                                }
                            }
                            //Incrementing Register values 
                            else if (strcmp(strtok(op,"\n"),"INX")==0) {
                                regs.X++;
                            }
                            else if (strcmp(strtok(op,"\n"),"INY")==0) {
                                regs.Y++;
                            }
                            else if (strcmp(op,"DEC")==0){
                                //Decrementing at a memory address
                                op = strtok(NULL, " ");
                                //If value is in hex
                                if (*op == '$'){
                                    op+=1;
                                    int val = (int)strtol(op, NULL, 16);
                                    monitorMem[val]--;
                                }
                            }
                            else {
                                //Labels
                                op = strtok(op, ":");
                                printf("%s\n",op);
                            }
                        }
                    }
                    //Print the memory after build (for debug)
                    PrintMemory(monitorMem);
                    fclose(fbuild);

                } else
                     printf("ERROR: Error with finding file: %s\n", argv[2]);
                if (instruction)
                    free(instruction);

                free(monitorMem);
            }
        }
    }
    return 0;
}

void ReadRegs(REGISTER reg) {
    printf("A: %X\nX: %X\nY: %X\nSTCKPTR: %X\nPROG COUNT: %X\nSTATUS: %X\n\n",
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

//This function here allocates values to a specified storage register (A, X and Y regs)
void LoadRegister(char* operation, uint8_t* regVal, uint8_t* mems) {
    operation = strtok(NULL, " ");

    //Check if using literal hex values to store value in register
    //# represents literal numbers and $ represents hexadecimal
    if (*operation == '#' && *operation+1 == '$') {
        //Get the value and convert to hex
        operation += 2;
        int val = (int)strtol(operation, NULL, 16);
        //Store value
        *regVal = val;
    }
    //To load register from a memory address (Example: LDA $0002)
    else if (*operation == '$') {
        operation += 1;
        //Load register value from memory address
        int val = (int)strtol(operation, NULL, 16);
        *regVal = mems[val];
    }
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
