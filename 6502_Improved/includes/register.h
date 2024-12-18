//The main 6502 Registers (Program counter is 16 bit)
#pragma once
#include "defines.h"
typedef struct{
    uint8_t A; //accum reg
    uint8_t X; //X reg
    uint8_t Y; //Y reg
    uint8_t StackPt; //Stack pointer reg
    uint16_t PC; //Program counter reg
    uint8_t Status; //Status reg (this will be modified by the flags)
} REGISTER;

//These flags will be used to set certain modes under the Status register for the 6502
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