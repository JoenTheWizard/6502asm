#ifndef INTERPRET_H
#define INTERPRET_H
#include <stdio.h>
#include <signal.h>
#include <assert.h>
#include "translate.h"
#include "tokenize.h"
#include "gui.h"
#include "defines.h"
#include "register.h"
#include "assemble.h"
#include "labels.h"
#include "string_manipulation.h"

//MAIN 6502 ASSEMBLER INTERPRETATION IS HERE
//Fixing the 6502 assembly interpret (as the current implementation is ugly and inefficient)
//Still being worked on and will use the tokenization.h as well which is also being worked on

//For this we need some reserverd keywords to interpret
//Which for most of them will just be the instructions
static struct {
    OP_MODES mode;
    char* op;
} tokens_k[] = {
    {0, "LDA"}, {0, "LDX"}, {0, "LDY"},
    {0 ,"STA"}, {0 ,"STX"}, {0, "STY"},
    {0, "ADC"}, {0, "INC"}, {0, "DEC"},
    {0, "ASL"}, {0, "AND"}, {0, "CMP"},
    {0, "CPX"}, {0, "CPY"}, {0, "JMP"},
    {0, "ROL"}, {0, "INX"}, {0, "INY"},
    {0, "DEX"}, {0, "DEY"}, {0, "TAX"},
    {0, "TXA"}, {0, "TAY"}, {0, "TYA"},
    {0, "TXS"}, {0, "TSX"}, {0, "PHA"},
    {0, "PLA"}, {0, "NOP"}, {0, "SEC"},
    {0, "SED"}, {0, "SEI"}, {0, "CLC"},
    {0, "CLD"}, {0, "CLI"},
};

// ===========================================
//READ ASSEMBLY LANGUAGE (Not all opcodes are implemented)
static struct {
    char* OP_CODE;
    void (*fn)(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd);
} ops[] = {
    {"LDA", &LDA}, {"LDX", &LDX}, {"LDY", &LDY},
    {"STA", &STA}, {"STX", &STX}, {"STY", &STY},
    {"ADC", &ADC}, {"INC", &INC}, {"DEC", &DEC},
    {"ASL", &ASL}, {"AND", &AND}, {"CMP", &CMP}, 
    {"CPX", &CPX}, {"CPY", &CPY}, {"JMP", &JMP},
    {"ROL", &ROL},
};

//Im very fucking lazy so I stored 1 byte op-codes into a seperate array to store it's hex op-code
//rather than storing a function pointer like the 2-3 byte op-code counter-part
static struct {
    char* OP_CODE_SINGULAR;
    uint8_t hex_val;
} ops_singular[] = {
    {"INX",0xE8}, {"INY",0xC8},
    {"DEX",0xCA}, {"DEY",0x88},
    {"TAX",0xAA}, {"TXA",0x8A},
    {"TAY",0xA8}, {"TYA",0x98},
    {"TXS",0x9A}, {"TSX",0xBA},
    {"PHA",0x48}, {"PLA",0x68},
    {"NOP",0xEA}, {"SEC",0x38},
    {"SED",0xF8}, {"SEI",0x78},
    {"CLC",0x18}, {"CLD",0xD8},
    {"CLI",0x58},
};
// ===========================================

//Interpreter 6502 asm (moved here)
void Interpret6502asm(char* filePath, REGISTER* regs, int isVisual);

//Get the OPcode
int isOPCode(char* lineInstr);

//Is a Label?
int isLabel(char* lineInstr);

//Is an existing Opocode
int isValidOpcode(char* opcode);

//Signal
static void wrapup(void);

//Debugging stuff

//Print memory
void PrintMemory(uint8_t* mems);

//Read register
void ReadRegs(REGISTER* regs);

#endif