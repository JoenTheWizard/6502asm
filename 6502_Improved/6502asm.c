//Im just going to try and write a simple 6502 assembly interpreter. Might implement
//graphics output too possibly (graphics.h, SDL, GTK are options).
//Simple hello world in 6502 asm: https://youtu.be/9hLGvLvTs1w
#include <pthread.h>
#include <assert.h>
#include "includes/register.h"
#include "includes/translate.h"
#include "includes/assemble.h"
#include "includes/gui.h"
#include "includes/defines.h"

void ReadRegs(REGISTER reg);
void PrintFile(const char* fileName, int isHex);
void PrintMemory(uint8_t* mems);

//String manipulation
char* remove_spaces(char* s);
char* toUppercase(char* s);
int is_empty(const char* s);

//Take into account singular opcode
int strcmp_singular(char* op, char* c1, char* c2);

//Monitor memory management
uint8_t* monitorMem;

//Assembly hexdump
uint8_t assembly[ASM_MEMORY];

//Get the line index from file
int LineIndex = 0;

//Check if instruction is a label or not
int isLabel = 0;

//Labels list
#define LBL_LENGTH 64
typedef struct {
    char* name;
    int lineNumber;
    int size;
} LABELS;
LABELS* lblList;
void FreeLBLBuffers(LABELS* LBL_LIST, int length);

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
};

//Im very fucking lazy to fix this might change this later to store it all in one array
static struct {
    char* OP_CODE_SINGULAR;
    char* OP_CODE_SINGULAR_N;
    uint8_t hex_val;
} ops_singular[] = {
    {"INX","INX\n",0xE8}, {"INY","INY\n",0xC8},
    {"DEX","DEX\n",0xCA}, {"DEY","DEY\n",0x88},
    {"TAX","TAX\n",0xAA}, {"TXA","TXA\n",0x8A},
    {"TAY","TAY\n",0xA8}, {"TYA","TYA\n",0x98},
    {"TXS","TXS\n",0x9A}, {"TSX","TSX\n",0xBA},
    {"PHA","PHA\n",0x48}, {"PLA","PLA\n",0x68},
    {"NOP","NOP\n",0xEA}, {"SEC","SEC\n",0x38},
    {"SED","SED\n",0xF8}, {"SEI","SEI\n",0x78},
    {"CLC","CLC\n",0x18}, {"CLD","CLD\n",0xD8},
    {"CLI","CLI\n",0x58},
};
// ===========================================

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
    regs.Status |= U;

    //Set the break flag if BREAK is enabled
    #ifdef BREAK
    regs.Status |= B;
    #endif

    if (argc < 2) {
        printf("Error! Usage: ./6502asm --build prog.asm\n");
        return -1;
    }
    else {
        if (argc == 2 && strcmp(argv[1],"regs")==0)
            ReadRegs(regs);
        if (strcmp(argv[1],"--test")==0) {
            printf("This is just used for debugging.\n");
        }
        else if (argc >= 3) {
            if (strcmp(argv[1],"--read") == 0) {
                PrintFile(argv[2],1);
            }
            if (!strcmp(argv[1],"--build")){
                //File allocate
                FILE* fbuild;
                char* instruction = malloc(1024*sizeof(char));
                size_t* size;
                //Once we try and build, init the monitor memory
                monitorMem = (uint8_t*)malloc(sizeof(uint8_t)*ROM);
                //Allocate for labels
                lblList = (LABELS*)malloc(sizeof(LABELS)*LBL_LENGTH);
                assert(lblList != NULL);
                lblList->size = 0;

                if (fbuild = fopen(*(argv+2),"r"))
                {
                    while(getline(&instruction,size,fbuild) != -1)
                    {
                        isLabel = 0;
                        if (*instruction != '\n') 
                        {
                            if (!is_empty(instruction)) {
                                char* op = strtok(instruction, " ");
                                op = toUppercase(op);
                                //Check for whitespaces!
                                if (op != NULL) {
                                    int label_ = 1;
                                    
                                    //Instructions
                                    for (int i = 0; i < sizeof(ops)/sizeof(ops[0]); i++) {
                                        if (!strcmp(op,ops[i].OP_CODE)) {
                                            op = strtok(NULL, " ");
                                            ops[i].fn(assembly, op, &LineIndex);

                                            label_ = 0;
                                        }
                                    }

                                    //Singular instructions
                                    for (int i = 0; i < sizeof(ops_singular)/sizeof(ops_singular[0]); i++) {
                                        if (!strcmp(op, ops_singular[i].OP_CODE_SINGULAR) || 
                                                    !strcmp(op, ops_singular[i].OP_CODE_SINGULAR_N)) {
                                            assembly[LineIndex] = ops_singular[i].hex_val;
                                            label_ = 0;
                                        }
                                    }

                                    if (label_) {
                                        //Labels
                                        op = strtok(op, ":");

                                        //Add the labels to the list then increment index
                                        lblList[lblList->size].lineNumber = LineIndex;
                                        lblList[lblList->size].name = (char*)malloc(sizeof(char)*strlen(op)+1);
                                        strncpy(lblList[lblList->size].name,op,strlen(op));

                                        printf("%s: %i\n", lblList[lblList->size].name, lblList[lblList->size].lineNumber);

                                        lblList->size++;

                                        //Is a label
                                        isLabel = 1;
                                    }

                                    if (!isLabel)
                                        LineIndex++;
                                }
                            }
                        }
                    }
                    //ASSEMBLE
                    ASM(LineIndex,assembly,&regs,monitorMem);

                    //Monitor memory (Zero page)
                    printf("ZERO PAGE\n");
                    PrintMemory(monitorMem);

                    //SDL menu
                    if (argc >= 4) {
                    //Graphics output
                        if (!strcmp(argv[3],"-v")||!strcmp(argv[3],"--visual")) {
                            // pthread_t thread_id;
                            // pthread_create(&thread_id, NULL, (void*)OpenGUI, monitorMem);
                            // pthread_join(thread_id, NULL);
                            OpenGUI(monitorMem);
                        }
                    }

                    //Read the registers
                    ReadRegs(regs);
                    //Close and deallocate file
                    fclose(fbuild);
                    //pthread_exit(NULL);

                } else
                     printf("ERROR: Error with finding file: %s\n", argv[2]);

                //== Memory deallocation ==
                if (instruction)
                    free(instruction);

                //Free the monitor memory
                free(monitorMem);
                //Free the labels list
                FreeLBLBuffers(lblList, lblList->size);
                free(lblList);
            }
        }
    }
    return 0;
}

void ReadRegs(REGISTER reg) {
    printf("A: %X\nX: %X\nY: %X\nSTCKPTR: %X\nPROG COUNT: 0x%04X\nSTATUS: %X\n\n",
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

//This will just print the zero page memory at the moment (0-255 bytes)
void PrintMemory(uint8_t* mems)
{
    uint16_t adr = 0x0000;
    printf("%04x: %02x ",adr,mems[0]);
    for (int i = 1; i < ROM; i++) {
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
        while (*d == '\n') {
            ++d;
        }
    } while (*s++ = *d++);
    return s;
}

int is_empty(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return 0;
    s++;
  }
  return 1;
}

//Free the name buffers
void FreeLBLBuffers(LABELS* LBL_LIST, int length) {
    for (int i = 0; i < length; i++)
        free(LBL_LIST[i].name);
}

int strcmp_singular(char* op, char* c1, char* c2) {
    return !strcmp(op, c1)||!strcmp(op,c2);
}

char* toUppercase(char* s) {
    for (int i = 0; s[i] != '\0';i++) {
        if (s[i] >= 'a' && s[i] <= 'z')
            s[i] = s[i] - 32;
    }
    return s;
}