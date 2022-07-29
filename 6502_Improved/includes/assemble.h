//The assembly operation of the 6502 CPU. Interprets the hex values
//The program counter will increment each byte and interpret those hex
//values. NOTE: 6502 is little endian.
//Source: http://www.6502.org/tutorials/6502opcodes.html
//Some undocumented opcodes such as "SAX" (0x87) which stores in both A and X register will be added soon too
#include "defines.h"

uint16_t Bit16Adr(uint8_t* lower, uint8_t* upper);

void ASM(int Line, uint8_t memory[ASM_MEMORY], REGISTER* reg, uint8_t* monitorMem) {
    //PRINTING THE DISASSEMBLE
    printf("DISASSEMBLE\n");
    for (int i = 0; i < Line; i++)
        printf("%02x ",memory[i]);
    
    //ALLOCATE ROM
    for (int i = 0x0600; i < 0x0600 + Line; i++)
        monitorMem[i] = memory[i - 0x0600];

    printf("\n======== ASSEMBLING %i BYTES =======\n", Line);
    while (reg->PC < ROM) {
        switch (monitorMem[reg->PC]) {
            //LDA
            case 0xA9:
                reg->PC++;
                reg->A = monitorMem[reg->PC];
                break;
            case 0xA5:
                reg->PC++;
                reg->A = monitorMem[monitorMem[reg->PC]];
                break;
            case 0xAD:
                reg->PC++;
                uint16_t adr = 0;
                adr |= monitorMem[reg->PC];
                reg->PC++;
                adr |= (monitorMem[reg->PC] << 8);
                reg->A = monitorMem[adr];
                break;
            case 0xB5:
                reg->PC++;
                reg->A = monitorMem[monitorMem[reg->PC] + reg->X];
                break;
            case 0xBD:
                reg->PC++;
                uint16_t LDAX = 0;
                LDAX |= monitorMem[reg->PC];
                reg->PC++;
                LDAX |= (monitorMem[reg->PC] << 8);
                reg->A = monitorMem[LDAX + reg->X];
                break;
            case 0xB9:
                reg->PC++;
                uint16_t LDAY = 0;
                LDAY |= monitorMem[reg->PC];
                reg->PC++;
                LDAY |= (monitorMem[reg->PC] << 8);
                reg->A = monitorMem[LDAY + reg->Y];
                break;
            //STA
            case 0x85:
                reg->PC++;
                monitorMem[monitorMem[reg->PC]] = reg->A;
                break;
            case 0x8D:
                reg->PC++;
                uint16_t adrSTA = 0;
                adrSTA |= monitorMem[reg->PC];
                reg->PC++;
                adrSTA |= (monitorMem[reg->PC] << 8);
                monitorMem[adrSTA] = reg->A;
                break;
            case 0x9D:
                reg->PC++;
                monitorMem[monitorMem[reg->PC] + reg->X] = reg->A;
                break;
            //STX
            case 0x86:
                reg->PC++;
                monitorMem[monitorMem[reg->PC]] = reg->X;
                break;
            case 0x8E:
                reg->PC++;
                uint16_t adrSTX = 0;
                adrSTX |= monitorMem[reg->PC];
                reg->PC++;
                adrSTX |= (monitorMem[reg->PC] << 8);
                monitorMem[adrSTX] = reg->X;
                break;
            case 0x96:
                reg->PC++;
                monitorMem[monitorMem[reg->PC] + reg->Y] = reg->X;
                break;
            //STY
            case 0x84:
                reg->PC++;
                monitorMem[monitorMem[reg->PC]] = reg->Y;
                break;
            case 0x8C:
                reg->PC++;
                uint16_t adrSTY = 0;
                adrSTY |= monitorMem[reg->PC];
                reg->PC++;
                adrSTY |= (monitorMem[reg->PC] << 8);
                monitorMem[adrSTY] = reg->Y;
                break;
            case 0x94:
                reg->PC++;
                monitorMem[monitorMem[reg->PC] + reg->X] = reg->Y;
                break;
            //LDX
            case 0xA2:
                reg->PC++;
                reg->X = monitorMem[reg->PC];
                break;
            case 0xA6:
                reg->PC++;
                reg->X = monitorMem[monitorMem[reg->PC]];
                break;
            case 0xAE:
                reg->PC++;
                uint16_t adrX = 0;
                adrX |= monitorMem[reg->PC];
                reg->PC++;
                adrX |= (monitorMem[reg->PC] << 8);
                reg->X = monitorMem[adrX];
                break;
            //LDY
            case 0xA0:
                reg->PC++;
                reg->Y = monitorMem[reg->PC];
                break;
            case 0xA4:
                reg->PC++;
                reg->Y = monitorMem[monitorMem[reg->PC]];
                break;
            case 0xAC:
                reg->PC++;
                uint16_t adrY = 0;
                adrY |= monitorMem[reg->PC];
                reg->PC++;
                adrY |= (monitorMem[reg->PC] << 8);
                reg->Y = monitorMem[adrY];
                break;
            //ADC
            case 0x69:
                reg->PC++;
                reg->A += monitorMem[reg->PC];
                break;
            case 0x65:
                reg->PC++;
                reg->A += monitorMem[monitorMem[reg->PC]];
                break;
            case 0x6D:
                reg->PC++;
                uint16_t adrADC = 0;
                adrADC |= monitorMem[reg->PC];
                reg->PC++;
                adrADC |= (monitorMem[reg->PC] << 8);
                reg->A += monitorMem[adrADC];
                break;
            case 0x75:
                reg->PC++;
                reg->A += monitorMem[monitorMem[reg->PC] + reg->X];
                break;
            case 0x7D:
                reg->PC++;
                uint16_t adrADCX = 0;
                adrADCX |= monitorMem[reg->PC];
                reg->PC++;
                adrADCX |= (monitorMem[reg->PC] << 8);
                reg->A += monitorMem[adrADCX+reg->X];
                break;
            case 0x79:
                reg->PC++;
                uint16_t adrADCY = 0;
                adrADCY |= monitorMem[reg->PC];
                reg->PC++;
                adrADCY |= (monitorMem[reg->PC] << 8);
                reg->A += monitorMem[adrADCY+reg->Y];
                break;
            //INC
            case 0xE6:
                reg->PC++;
                monitorMem[monitorMem[reg->PC]]++;
                break;
            case 0xEE:
                reg->PC++;
                uint16_t adrINC = 0;
                adrINC |= monitorMem[reg->PC];
                reg->PC++;
                adrINC |= (monitorMem[reg->PC] << 8);
                monitorMem[adrINC]++;
                break;
            case 0xF6:
                reg->PC++;
                monitorMem[monitorMem[reg->PC] + reg->X]++;
                break;
            case 0xFE:
                reg->PC++;
                uint16_t adrINCX = 0;
                adrINCX |= monitorMem[reg->PC];
                reg->PC++;
                adrINCX |= (monitorMem[reg->PC] << 8);
                monitorMem[adrINCX+reg->X]++;
                break;
            //DEC
            case 0xC6:
                reg->PC++;
                monitorMem[monitorMem[reg->PC]]--;
                break;
            case 0xCE:
                reg->PC++;
                uint16_t adrDEC = 0;
                adrDEC |= monitorMem[reg->PC];
                reg->PC++;
                adrDEC |= (monitorMem[reg->PC] << 8);
                monitorMem[adrDEC]--;
                break;
            case 0xD6:
                reg->PC++;
                monitorMem[monitorMem[reg->PC]+reg->X]--;
                break;
            case 0xDE:
                reg->PC++;
                uint16_t adrDECX = 0;
                adrDECX |= monitorMem[reg->PC];
                reg->PC++;
                adrDECX |= (monitorMem[reg->PC] << 8);
                monitorMem[adrDECX+reg->X]--;
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
                jmpAdr |= monitorMem[reg->PC];
                reg->PC++;
                jmpAdr |= (monitorMem[reg->PC] << 8);
                //Negate address by one since PC always increments
                reg->PC = jmpAdr - 1;
                break;
            case 0x6C:
                reg->PC++;
                //Extract address to point to
                uint16_t jmpAdr0 = 0;
                jmpAdr0 |= monitorMem[reg->PC];
                reg->PC++;
                jmpAdr0 |= (monitorMem[reg->PC] << 8);
                //Then we need to obtain the values to dereference
                uint16_t jmpAdr1 = 0;
                jmpAdr1 |= monitorMem[jmpAdr0];
                //In indirect mode we must check for the high byte
                //(if first vector is at $xxFF address then it wraps about to $xx00 for next address)
                jmpAdr1 |= jmpAdr0 & 0xFF == 0xFF ? (monitorMem[jmpAdr0 ^ 0xFF] << 8) :
                    (monitorMem[jmpAdr0 + 1] << 8);
                //printf("OUR PC: %04x\n",jmpAdr1);
                reg->PC = jmpAdr1;
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
