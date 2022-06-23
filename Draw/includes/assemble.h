//The assembly operation of the 6502 CPU. Interprets the hex values
//The program counter will increment each byte and interpret those hex
//values. NOTE: 6502 is little endian.
//Source: http://www.6502.org/tutorials/6502opcodes.html
#include "defines.h"

void ASM(int Line, uint8_t memory[ASM_MEMORY], REGISTER* reg, uint8_t* monitorMem) {
    //PRINTING
    printf("DISASSEMBLE\n");
    for (int i = 0; i < Line; i++)
        printf("%02x ",memory[i]);
    
    printf("\n======== ASSEMBLING %i BYTES =======\n", Line);
    while (reg->PC < ASM_MEMORY) {
        switch (memory[reg->PC]) {
            //LDA
            case 0xA9:
                reg->PC++;
                reg->A = memory[reg->PC];
                break;
            case 0xA5:
                reg->PC++;
                reg->A = monitorMem[memory[reg->PC]];
                break;
            case 0xAD:
                reg->PC++;
                uint16_t adr = 0;
                adr = adr | memory[reg->PC];
                reg->PC++;
                adr = adr | (memory[reg->PC] << 8);
                reg->A = monitorMem[adr];
                break;
            //STA
            case 0x85:
                reg->PC++;
                monitorMem[memory[reg->PC]] = reg->A;
                break;
            //LDX
            case 0xA2:
                reg->PC++;
                reg->X = memory[reg->PC];
                break;
            case 0xA6:
                reg->PC++;
                reg->X = monitorMem[memory[reg->PC]];
                break;
            case 0xAE:
                reg->PC++;
                uint16_t adrX = 0;
                adrX = adrX | memory[reg->PC];
                reg->PC++;
                adrX = adrX | (memory[reg->PC] << 8);
                reg->X = monitorMem[adrX];
                break;
            //LDY
            case 0xA0:
                reg->PC++;
                reg->Y = memory[reg->PC];
                break;
            case 0xA4:
                reg->PC++;
                reg->Y = monitorMem[memory[reg->PC]];
                break;
            case 0xAC:
                reg->PC++;
                uint16_t adrY = 0;
                adrY = adrY | memory[reg->PC];
                reg->PC++;
                adrY = adrY | (memory[reg->PC] << 8);
                reg->Y = monitorMem[adrY];
                break;
        }
        reg->PC++;
    }
}