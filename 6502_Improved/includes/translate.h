//For translation of each instruction
#pragma once
#include "defines.h"
//For strings and stuff
char* CheckRegisterOffset(char* op);

//Store 2 byte instruction (will increment ROM once)
void Store2byte_instr(uint8_t op_code, uint8_t asm_[ASM_MEMORY],  char* op, int* Line);

//Stores 3 byte instruction (will increment ROM twice)
void Store3byte_instr(uint8_t op_code, uint8_t asm_[ASM_MEMORY], char* op, int* Line);

//Thinking of just storing these all into an array of function pointers

//LDA
void LDA(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd);

//LDX
void LDX(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd);

//LDY
void LDY(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd);

//STA
void STA(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd);

//STX
void STX(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd);

//STY
void STY(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd);

//INX
void INX(uint8_t assemble[ASM_MEMORY], int* LineInd);

//INC
void INC(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd);

//DEC
void DEC(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd);

//ADC
void ADC(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd);

//ASL
void ASL(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd);

//AND
void AND(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd);

//CMP
void CMP(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd);

//CPX
void CPX(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd);

//CPY
void CPY(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd);

//JMP
void JMP(uint8_t assemble[ASM_MEMORY], char* op, int* LineInd);