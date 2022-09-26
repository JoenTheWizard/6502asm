//The assembly operation of the 6502 CPU. Interprets the hex values
//The program counter will increment each byte and interpret those hex
//values. NOTE: 6502 is little endian.
//Source: http://www.6502.org/tutorials/6502opcodes.html
//Some undocumented opcodes such as "SAX" (0x87) which stores in both A and X register will be added soon too
#include "defines.h"

//uint16_t Bit16Adr(uint8_t* lower, uint8_t* upper);

//Retrieve 2 byte/16-bit address
uint16_t Bit16Adr(REGISTER* r, uint8_t* mems);

//Retrieve 2 byte address via Indirect address mode (Derefernce adr from X reg offset)
uint16_t _indirectX(REGISTER* r, uint8_t* mems);

//Retrieve 2 byte address via Indirect address mode (Dereference adr then add Y reg offset)
uint16_t _indirectY(REGISTER* r, uint8_t* mems);

//Compare Instruction function 
//Sets certain flags within the Status Register depening on the difference between values
void _cmp_instruction(REGISTER* r, uint8_t* val, uint8_t mems);

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
        #ifdef BREAK
        if (monitorMem[reg->PC] == 0x00) {
            reg->PC++;
            break;
        }
        #endif
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
                reg->A = monitorMem[Bit16Adr(reg,monitorMem)];
                break;
            case 0xB5:
                reg->PC++;
                reg->A = monitorMem[monitorMem[reg->PC] + reg->X];
                break;
            case 0xBD:
                reg->A = monitorMem[Bit16Adr(reg,monitorMem) + reg->X];
                break;
            case 0xB9:
                reg->A = monitorMem[Bit16Adr(reg,monitorMem) + reg->Y];
                break;
            //STA
            case 0x85:
                reg->PC++;
                monitorMem[monitorMem[reg->PC]] = reg->A;
                break;
            case 0x8D:
                monitorMem[Bit16Adr(reg,monitorMem)] = reg->A;
                break;
            case 0x9D:
                reg->PC++;
                monitorMem[monitorMem[reg->PC] + reg->X] = reg->A;
                break;
            case 0x81:
                monitorMem[_indirectX(reg,monitorMem)] = reg->A;
                break;
            case 0x91:
                monitorMem[_indirectY(reg,monitorMem)] = reg->A;
                break;
            //STX
            case 0x86:
                reg->PC++;
                monitorMem[monitorMem[reg->PC]] = reg->X;
                break;
            case 0x8E:
                monitorMem[Bit16Adr(reg,monitorMem)] = reg->X;
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
                monitorMem[Bit16Adr(reg,monitorMem)] = reg->Y;
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
                reg->X = monitorMem[Bit16Adr(reg,monitorMem)];
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
                reg->Y = monitorMem[Bit16Adr(reg,monitorMem)];
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
                reg->A += monitorMem[Bit16Adr(reg,monitorMem)];
                break;
            case 0x75:
                reg->PC++;
                reg->A += monitorMem[monitorMem[reg->PC] + reg->X];
                break;
            case 0x7D:
                reg->A += monitorMem[Bit16Adr(reg,monitorMem)+reg->X];
                break;
            case 0x79:
                reg->A += monitorMem[Bit16Adr(reg,monitorMem)+reg->Y];
                break;
            case 0x61:
                reg->A += monitorMem[_indirectX(reg,monitorMem)];
                break;
            //INC
            case 0xE6:
                reg->PC++;
                monitorMem[monitorMem[reg->PC]]++;
                break;
            case 0xEE:
                monitorMem[Bit16Adr(reg,monitorMem)]++;
                break;
            case 0xF6:
                reg->PC++;
                monitorMem[monitorMem[reg->PC] + reg->X]++;
                break;
            case 0xFE:
                monitorMem[Bit16Adr(reg,monitorMem)+reg->X]++;
                break;
            //DEC
            case 0xC6:
                reg->PC++;
                monitorMem[monitorMem[reg->PC]]--;
                break;
            case 0xCE:
                monitorMem[Bit16Adr(reg,monitorMem)]--;
                break;
            case 0xD6:
                reg->PC++;
                monitorMem[monitorMem[reg->PC]+reg->X]--;
                break;
            case 0xDE:
                monitorMem[Bit16Adr(reg,monitorMem)+reg->X]--;
                break;
            //ASL
            case 0x0A:
                reg->A = reg->A << 1;
                break;
            case 0x06:
                reg->PC++;
                monitorMem[monitorMem[reg->PC]] = monitorMem[monitorMem[reg->PC]] << 1;
                break;
            case 0x0E:
                reg->PC++;
                uint16_t adrASL = 0;
                adrASL |= monitorMem[reg->PC];
                reg->PC++;
                adrASL |= (monitorMem[reg->PC] << 8);
                monitorMem[adrASL] = monitorMem[adrASL] << 1;
                break;
            case 0x16:
                reg->PC++;
                monitorMem[monitorMem[reg->PC]+reg->X] <<= 1;
                break;
            case 0x1E:
                monitorMem[Bit16Adr(reg,monitorMem)+reg->X] <<= 1;
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
            //Stack Instructions
            //TXS
            case 0x9A:
                reg->StackPt = reg->X;
                break;
            //TSX
            case 0xBA:
                reg->X = reg->StackPt;
                break;
            //PHA
            case 0x48:
                //Remember that if the Stack Pointer goes below
                //0x00 then it wraps about to the top of the stack
                monitorMem[0x0100 + reg->StackPt] = reg->A;
                reg->StackPt--;
                break;
            //PLA
            case 0x68:
                reg->StackPt++;
                reg->A = monitorMem[0x0100 + reg->StackPt];
                break;
            //CMP
            case 0xC9:
                reg->PC++;
                _cmp_instruction(reg, &reg->A, monitorMem[reg->PC]);
                break;
            case 0xC5:
                reg->PC++;
                _cmp_instruction(reg, &reg->A, monitorMem[monitorMem[reg->PC]]);
                break;
            case 0xCD:
                _cmp_instruction(reg, &reg->A, monitorMem[Bit16Adr(reg, monitorMem)]);
                break;
            case 0xD5:
                reg->PC++;
                _cmp_instruction(reg, &reg->A, monitorMem[monitorMem[reg->PC]+reg->X]);
                break;
            case 0xDD:
                _cmp_instruction(reg, &reg->A, monitorMem[Bit16Adr(reg, monitorMem)+reg->X]);
                break;
            case 0xD9:
                _cmp_instruction(reg, &reg->A, monitorMem[Bit16Adr(reg, monitorMem)+reg->Y]);
                break;
            //CPX
            case 0xE0:
                reg->PC++;
                _cmp_instruction(reg, &reg->X, monitorMem[reg->PC]);
                break;
            case 0xE4:
                reg->PC++;
                _cmp_instruction(reg, &reg->X, monitorMem[monitorMem[reg->PC]]);
                break;
            case 0xEC:
                _cmp_instruction(reg, &reg->X, monitorMem[Bit16Adr(reg, monitorMem)]);
                break;
            //CPY
             case 0xC0:
                reg->PC++;
                _cmp_instruction(reg, &reg->Y, monitorMem[reg->PC]);
                break;
            case 0xC4:
                reg->PC++;
                _cmp_instruction(reg, &reg->Y, monitorMem[monitorMem[reg->PC]]);
                break;
            case 0xCC:
                _cmp_instruction(reg, &reg->Y, monitorMem[Bit16Adr(reg, monitorMem)]);
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

uint16_t Bit16Adr(REGISTER* r, uint8_t* mems) {
    r->PC++;
    int adr = 0;
    adr |= mems[r->PC];
    r->PC++;
    adr |= (mems[r->PC] << 8);
    return adr;
}

uint16_t _indirectX(REGISTER* r, uint8_t* mems) {
    r->PC++;
    uint8_t curMem = mems[r->PC] + r->X;
    uint16_t adrADC = 0;
    adrADC |= mems[curMem];
    adrADC |= (mems[++curMem] << 8);
    return adrADC;
}

uint16_t _indirectY(REGISTER* r, uint8_t* mems) {
    r->PC++;
    uint8_t curMem = mems[r->PC];
    uint16_t adr = 0;
    adr |= mems[curMem];
    adr |= (mems[++curMem] << 8);
    //printf("==== %04x\n",adr + r->Y);
    return adr + r->Y;
}

void _cmp_instruction(REGISTER* r, uint8_t* val, uint8_t mems) {
    if (*val > mems) {
        r->Status |= C;
        r->Status &= ~Z;
    }
    if (*val < mems) {
        r->Status &= ~C;
        r->Status &= ~Z;
        r->Status |= N;
    }
    if (*val == mems) {
        r->Status |= C;
        r->Status |= Z;
    }
}
