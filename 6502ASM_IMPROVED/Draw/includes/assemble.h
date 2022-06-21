//The assembly operation of the 6502 CPU. Interprets the hex values
//The program counter will increment each byte and interpret those hex
//values. NOTE: 6502 is little endian
#include "defines.h"
void ASM(int Line, uint8_t memory[ASM_MEMORY], REGISTER* reg, uint8_t* monitorMem) {
    //PRINTING
    for (int i = 0; i < Line; i++)
        printf("%02x ",memory[i]);
    
    printf("\n======== ASSEMBLING %i BYTES =======\n", Line);
    while (reg->PC < ASM_MEMORY) {
        switch (memory[reg->PC]) {
            //LDA
            //Two byte operation
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
            //Two byte operation
            case 0x85:
                reg->PC++;
                monitorMem[memory[reg->PC]] = reg->A;
                break;
        }
        reg->PC++;
    }
}