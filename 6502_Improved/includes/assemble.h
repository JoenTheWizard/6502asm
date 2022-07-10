//The assembly operation of the 6502 CPU. Interprets the hex values
//The program counter will increment each byte and interpret those hex
//values. NOTE: 6502 is little endian.
//Source: http://www.6502.org/tutorials/6502opcodes.html
//Some undocumented opcodes such as "SAX" (0x87) which stores in both A and X register will be added soon too
#include "defines.h"

uint16_t Bit16Adr(uint8_t* lower, uint8_t* upper);

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
                adr |= memory[reg->PC];
                reg->PC++;
                adr |= (memory[reg->PC] << 8);
                reg->A = monitorMem[adr];
                break;
            //STA
            case 0x85:
                reg->PC++;
                monitorMem[memory[reg->PC]] = reg->A;
                break;
            case 0x8D:
                reg->PC++;
                uint16_t adrSTA = 0;
                adrSTA |= memory[reg->PC];
                reg->PC++;
                adrSTA |= (memory[reg->PC] << 8);
                monitorMem[adrSTA] = reg->A;
                break;
            case 0x9D:
                reg->PC++;
                monitorMem[memory[reg->PC] + reg->X] = reg->A;
                break;
            //STX
            case 0x84:
                reg->PC++;
                monitorMem[memory[reg->PC]] = reg->X;
                break;
            case 0x8E:
                reg->PC++;
                uint16_t adrSTX = 0;
                adrSTX |= memory[reg->PC];
                reg->PC++;
                adrSTX |= (memory[reg->PC] << 8);
                monitorMem[adrSTX] = reg->X;
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
                adrX |= memory[reg->PC];
                reg->PC++;
                adrX |= (memory[reg->PC] << 8);
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
                adrY |= memory[reg->PC];
                reg->PC++;
                adrY |= (memory[reg->PC] << 8);
                reg->Y = monitorMem[adrY];
                break;
            //INX
            case 0xE8:
                reg->X++;
                break;
            //DEX
            case 0xCA:
                reg->X--;
                break;
            case 0x88:
                reg->Y--;
                break;
            //INY
            case 0xC8:
                reg->Y++;
                break;
            //TAX
            case 0xAA:
                reg->X = reg->A;
                break;
            //TXA
            case 0x8A:
                reg->A = reg->X;
                break;
            //TAY
            case 0xA8:
                reg->Y = reg->A;
                break;
            //TYA
            case 0x98:
                reg->A = reg->Y;
                break;
            //JMP
            case 0x4C:
                reg->PC++;
                uint16_t jmpAdr = 0;
                jmpAdr |= memory[reg->PC];
                reg->PC++;
                jmpAdr |= (memory[reg->PC] << 8);
                //Negate address by one since PC always increments
                reg->PC = jmpAdr - 1;
                break;
        }
        //printf("X: %02x\n",reg->X);
        //The random number generator at memory address '$fe' (after each instruction called)
        srand(time(NULL));
        monitorMem[0xFE] = rand() % (0xFF + 1);
        reg->PC++;
    }
}

uint16_t Bit16Adr(uint8_t* lower, uint8_t* upper) {
    uint16_t adr = 0;
    adr |= *lower;
    adr |= ((*upper) << 8);
    return adr;
}