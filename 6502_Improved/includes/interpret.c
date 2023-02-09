#include "interpret.h"

//Store the instructions to read line by line
char* instruction;

int LineNumber, //Store each line number
    byteIndex; //Store byte index


//Monitor memory management
uint8_t* monitorMem;

//Assembly hexdump
uint8_t assembly[ASM_MEMORY];

//List of labels
LABELS* lblList;

void Interpret6502asm(char* filePath, REGISTER* regs, int isVisual) {
    //File allocate and read
    FILE* fbuild;
    instruction = NULL;
    size_t size;
    ssize_t read;

    //Once we try and build, init the monitor memory
    monitorMem = (uint8_t*)malloc(sizeof(uint8_t) * ROM);

    //Start at line number 1
    LineNumber = 1;

    //Init at byte index 0
    byteIndex = 0;

    //Allocate for labels
    lblList = (LABELS*)malloc(sizeof(LABELS) * LBL_LENGTH);
    assert(lblList != NULL);

    lblList->size = 0;

    if (fbuild = fopen(filePath, "r")) {
        while((read = getline(&instruction, &size, fbuild)) != -1) {
            if (!isEmpty(instruction)) {
                //Trim the string and remove the newline characters
                trim_string(instruction);
                instruction[strcspn(instruction, "\n")] = '\0';

                //Check if line is an op code
                if (isOPCode(instruction)) {
                    //Copy opcode string
                    char getOPCode[4];
                    memcpy(getOPCode, instruction, 4);
                    getOPCode[3] = '\0';

                    //Set string to uppercase (if lowercase)
                    toUppercase(getOPCode);

                    //Is said opcode even 'valid'?
                    if (isValidOpcode(getOPCode)) {
                        char* instr_cp = (char*)malloc(read * sizeof(char));
                        memcpy(instr_cp, &instruction[3], read);

                        //Remove any leading whitespaces from the instruction argument
                        trim_string(instr_cp);

                        //If any arguments
                        if (strlen(instr_cp) != 0) {
                            //By iterating within the loop we need to know if the opcode is supposed to be sigular
                            int isSingular = 0;
                            for (int i = 0; i < sizeof(ops)/sizeof(ops[0]); i++) {
                                if (!strcmp(getOPCode, ops[i].OP_CODE)) {
                                    //Copy
                                    char* arg = instr_cp;
                                    //Must remove whitespaces
                                    remove_spaces(arg);
                                    ops[i].fn(assembly, arg, &byteIndex);

                                    //Argument is valid for the parsed opcode
                                    isSingular = 1;
                                    break;
                                }
                            }
                            //If its a supposed singular opcode raise an error
                            if (!isSingular)
                                fprintf(stderr, "[-] 6502asm raised an error. The singular byte opcode '%s' at line %d requires no arguments.\n",
                                        getOPCode, LineNumber);
                        }
                        //Singular instructions
                        else {
                            //Check if the opcode read requires arguments
                            int isArgument = 0;
                            for (int i = 0; i < sizeof(ops_singular)/sizeof(ops_singular[0]); i++) {
                                if (!strcmp(getOPCode, ops_singular[i].OP_CODE_SINGULAR)) {
                                    assembly[byteIndex] = ops_singular[i].hex_val;

                                    //Opcode doesnt require argument
                                    isArgument = 1;
                                    break;
                                }
                            }
                            //If its a supposed non-singular opcode (requires argument) raise an error
                            if (!isArgument)
                                fprintf(stderr, "[-] 6502asm raised an error. The opcode '%s' at line %d requires arguments to function.\n",
                                        getOPCode, LineNumber);
                        }
                        free(instr_cp);
                        //Must be incremented for each byte
                        byteIndex++;
                    }
                    else
                        fprintf(stderr, "[-] 6502asm raised an error: Unrecognized opcode '%s' at line %i\n", getOPCode, LineNumber);
                }

                //Keeping this for later, might remove later
                #pragma region OLD IMPLEMENTATION
                // char* op = strtok(instruction, " ");
                // op = toUppercase(op);
                // //Check for whitespaces!
                // if (op != NULL) {
                //     int label_ = 1;
                    
                //     //Instructions
                //     for (int i = 0; i < sizeof(ops)/sizeof(ops[0]); i++) {
                //         if (!strcmp(op,ops[i].OP_CODE)) {
                //             op = strtok(NULL, " ");
                //             ops[i].fn(assembly, op, &LineIndex);

                //             label_ = 0;
                //         }
                //     }

                //     //Just need to get rid of new line character
                //     op[strcspn(op,"\n")] = '\0';

                //     //Singular instructions
                //     for (int i = 0; i < sizeof(ops_singular)/sizeof(ops_singular[0]); i++) {
                //         if (!strcmp(op, ops_singular[i].OP_CODE_SINGULAR)) {
                //             assembly[LineIndex] = ops_singular[i].hex_val;
                //             label_ = 0;
                //         }
                //     }

                //     if (label_) {
                //         //Labels
                //         op = strtok(op, ":");

                //         //Add the labels to the list then increment index
                //         lblList[lblList->size].lineNumber = LineIndex;
                //         lblList[lblList->size].name = (char*)malloc(sizeof(char)*strlen(op)+1);
                //         strncpy(lblList[lblList->size].name,op,strlen(op));
                        
                //         //Print memory location of the label (incremented by program counter)
                //         printf("%s: %04x\n", lblList[lblList->size].name, regs.PC + lblList[lblList->size].lineNumber);

                //         lblList->size++;

                //         //Is a label
                //         isLabel = 1;
                //     }

                //     if (!isLabel)
                //         LineIndex++;
                // }
                #pragma endregion
            }
            //Increment line number
            LineNumber++;
        }

        //Handle signal interrupts
        signal(SIGINT, (void(*)())wrapup);

        //ASSEMBLE
        ASM(byteIndex, assembly, regs, monitorMem);

        //Monitor memory (Zero page)
        printf("ZERO PAGE\n");

        //Print the memory
        PrintMemory(monitorMem);

        //SDL menu
        //Graphics output
        if (isVisual) {
            // pthread_t thread_id;
            // pthread_create(&thread_id, NULL, (void*)OpenGUI, monitorMem);
            // pthread_join(thread_id, NULL);
            OpenGUI(monitorMem);
        }

        //Read the registers
        ReadRegs(regs);
        //Close and deallocate file
        fclose(fbuild);

    } else
        fprintf(stderr, "[-] 6502asm raised an error: Error with finding file: %s\n", filePath);

    //== Memory deallocation ==
    if (instruction)
        free(instruction);

    //Free the monitor memory
    free(monitorMem);
    //Free the labels list
    FreeLBLBuffers(lblList);
    free(lblList);
}

//Free the name buffers
void FreeLBLBuffers(LABELS* LBL_LIST) {
    for (int i = 0; i < LBL_LIST->size; i++)
        free(LBL_LIST[i].name);
}

//Return the opcode (without strtok usage)
int isOPCode(char* lineInstr) {
    //Gain count
    int i = 0;
    while (lineInstr[i] != '\0') {
        if (lineInstr[i] == ' ')
            break;
        i++;
    }
    //Check if an opcode
    return i == 3 ? 1 : 0;
}

//Check if the parsed "opcode" is valid
int isValidOpcode(char* opcode) {
    for (int i = 0; i < sizeof(tokens_k)/sizeof(tokens_k[0]); i++) {
        //If found then is true
        if (!strncmp(opcode, tokens_k[i].op, 4))
            return 1;
    }
    //Otherwise not
    return 0;
}

//Read the register vals
void ReadRegs(REGISTER* regs) {
    printf("A: %X\nX: %X\nY: %X\nSTCKPTR: %X\nPROG COUNT: 0x%04X\nSTATUS: %X\n\n",
        regs->A, regs->X, regs->Y, regs->StackPt, regs->PC, regs->Status);
}

//Print memory
void PrintMemory(uint8_t* mems) {
    uint16_t adr = 0x0000;
    printf("%04x: %02x ", adr, mems[0]);
    for (int i = 1; i < ROM; i++) {
        if (i%16==15)
            printf("%02x\n%04x: ", mems[i], adr+=16);
        else
            printf("%02x ", mems[i]);
    }
    printf("%c",'\n');
}

//SIGINT deallocation.
//Not all of the heap buffers are deallocated but I'll fix that later
static void wrapup(void) {
    free(monitorMem);
    //Free the labels list
    FreeLBLBuffers(lblList);
    free(lblList);
    exit(0);
}