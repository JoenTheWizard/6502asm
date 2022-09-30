//The assembly operation of the 6502 CPU. Interprets the hex values
//The program counter will increment each byte and interpret those hex
//values. NOTE: 6502 is little endian.
//Source: http://www.6502.org/tutorials/6502opcodes.html
//Some undocumented opcodes such as "SAX" (0x87) which stores in both A and X register will be added soon too

//uint16_t Bit16Adr(uint8_t* lower, uint8_t* upper);
#pragma once
#include <time.h>
#include "register.h"
#include "defines.h"

//Retrieve 2 byte/16-bit address via ROM
uint16_t Bit16Adr(REGISTER* r, uint8_t* mems);

//Retrieve 2 byte address via Indirect address mode (Derefernce adr from X reg offset)
uint16_t _indirectX(REGISTER* r, uint8_t* mems);

//Retrieve 2 byte address via Indirect address mode (Dereference adr then add Y reg offset)
uint16_t _indirectY(REGISTER* r, uint8_t* mems);

//Compare Instruction function 
//Sets certain flags within the Status Register depening on the difference between values
void _cmp_instruction(REGISTER* r, uint8_t* val, uint8_t mems);

//Interpret each of the byte op codes which starts with an instruction op code then any argument
//op codes right after.
void ASM(int Line, uint8_t memory[ASM_MEMORY], REGISTER* reg, uint8_t* monitorMem);
