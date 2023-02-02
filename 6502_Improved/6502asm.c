//Im just going to try and write a simple 6502 assembly interpreter. Might implement
//TODO: Labels, binary representation ('%'), rest of the opcodes

//Simple hello world in 6502 asm: https://youtu.be/9hLGvLvTs1w
#include <pthread.h>
#include <assert.h>
#include "includes/defines.h"
#include "includes/interpret.h"

void PrintFile(const char* fileName, int isHex);

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
            ReadRegs(&regs);
        if (!strcmp(argv[1],"--test")) {
            printf("This is just used for debugging.\n");

            //InterpretFile("test1.asm");
            //tokenize_file("test.asm");
        }
        else if (argc >= 3) {
            if (!strcmp(argv[1],"--read")) {
                PrintFile(argv[2],1);
            }
            if (!strcmp(argv[1],"--build") || !strcmp(argv[1],"-b")) {
                //Check for GUI mode
                int gui = 0;
                if (argc >= 4) {
                    if (!strcmp(argv[3], "-v"))
                        gui = 1;
                }
                //Interpret the 6502 assembly file
                //Translates the assembly to opcodes
                Interpret6502asm(argv[2], &regs, gui);

                #pragma region IGNORE
                // //File allocate
                // FILE* fbuild;
                // char* instruction = malloc(1024*sizeof(char));
                // size_t size;
                // ssize_t read;
                // //Once we try and build, init the monitor memory
                // monitorMem = (uint8_t*)malloc(sizeof(uint8_t)*ROM);
                // //Allocate for labels
                // lblList = (LABELS*)malloc(sizeof(LABELS)*LBL_LENGTH);
                // assert(lblList != NULL);
                // lblList->size = 0;

                // if (fbuild = fopen(*(argv+2),"r"))
                // {
                //     while((read = getline(&instruction, &size, fbuild)) != -1)
                //     {
                //         if (*instruction != '\n') 
                //         {
                //             if (!is_empty(instruction)) {
                //                 //Trim the string and remove the newline characters
                //                 trim_string(instruction);
                //                 instruction[strcspn(instruction, "\n")] = '\0';

                //                 //Check if line is an op code
                //                 if (isOPCode(instruction)) {
                //                     //Copy opcode string
                //                     char getOPCode[4];
                //                     memcpy(getOPCode, instruction, 4);
                //                     getOPCode[3] = '\0';

                //                     //Set string to uppercase (if lowercase)
                //                     toUppercase(getOPCode);

                //                     if (isValidOpcode(getOPCode)) {
                //                         char* instr_cp = (char*)malloc(read * sizeof(char));
                //                         memcpy(instr_cp, &instruction[3], read);

                //                         //Remove any leading whitespaces from the instruction argument
                //                         trim_string(instr_cp);

                //                         //If any arguments
                //                         if (strlen(instr_cp) != 0) {
                //                             for (int i = 0; i < sizeof(ops)/sizeof(ops[0]); i++) {
                //                                 if (!strcmp(getOPCode, ops[i].OP_CODE)) {
                //                                     //Copy
                //                                     char* arg = instr_cp;
                //                                     //Must remove whitespaces
                //                                     remove_spaces(arg);
                //                                     ops[i].fn(assembly, arg, &LineIndex);
                //                                 }
                //                             }
                //                         }
                //                         else {
                //                             //Singular instructions
                //                             for (int i = 0; i < sizeof(ops_singular)/sizeof(ops_singular[0]); i++) {
                //                                 if (!strcmp(getOPCode, ops_singular[i].OP_CODE_SINGULAR)) {
                //                                     assembly[LineIndex] = ops_singular[i].hex_val;
                //                                 }
                //                             }
                //                         }
                //                         free(instr_cp);
                //                         //Must be incremented for each byte
                //                         LineIndex++;
                //                     }
                //                     else
                //                         fprintf(stderr, "[-] 6502asm raised an error: Unrecognized opcode '%s'\n", getOPCode);
                //                 }

                //                 //Keeping this for later, might remove later
                //                 #pragma region OLD IMPLEMENTATION
                //                 // char* op = strtok(instruction, " ");
                //                 // op = toUppercase(op);
                //                 // //Check for whitespaces!
                //                 // if (op != NULL) {
                //                 //     int label_ = 1;
                                    
                //                 //     //Instructions
                //                 //     for (int i = 0; i < sizeof(ops)/sizeof(ops[0]); i++) {
                //                 //         if (!strcmp(op,ops[i].OP_CODE)) {
                //                 //             op = strtok(NULL, " ");
                //                 //             ops[i].fn(assembly, op, &LineIndex);

                //                 //             label_ = 0;
                //                 //         }
                //                 //     }

                //                 //     //Just need to get rid of new line character
                //                 //     op[strcspn(op,"\n")] = '\0';

                //                 //     //Singular instructions
                //                 //     for (int i = 0; i < sizeof(ops_singular)/sizeof(ops_singular[0]); i++) {
                //                 //         if (!strcmp(op, ops_singular[i].OP_CODE_SINGULAR)) {
                //                 //             assembly[LineIndex] = ops_singular[i].hex_val;
                //                 //             label_ = 0;
                //                 //         }
                //                 //     }

                //                 //     if (label_) {
                //                 //         //Labels
                //                 //         op = strtok(op, ":");

                //                 //         //Add the labels to the list then increment index
                //                 //         lblList[lblList->size].lineNumber = LineIndex;
                //                 //         lblList[lblList->size].name = (char*)malloc(sizeof(char)*strlen(op)+1);
                //                 //         strncpy(lblList[lblList->size].name,op,strlen(op));
                                        
                //                 //         //Print memory location of the label (incremented by program counter)
                //                 //         printf("%s: %04x\n", lblList[lblList->size].name, regs.PC + lblList[lblList->size].lineNumber);

                //                 //         lblList->size++;

                //                 //         //Is a label
                //                 //         isLabel = 1;
                //                 //     }

                //                 //     if (!isLabel)
                //                 //         LineIndex++;
                //                 // }
                //                 #pragma endregion
                //             }
                //         }
                //     }

                //     //Handle signal interrupts
                //     signal(SIGINT, (void(*)())wrapup);

                //     //ASSEMBLE
                //     ASM(LineIndex,assembly,&regs,monitorMem);

                //     //Monitor memory (Zero page)
                //     printf("ZERO PAGE\n");
                //     PrintMemory(monitorMem);

                //     //SDL menu
                //     if (argc >= 4) {
                //     //Graphics output
                //         if (!strcmp(argv[3],"-v")||!strcmp(argv[3],"--visual")) {
                //             // pthread_t thread_id;
                //             // pthread_create(&thread_id, NULL, (void*)OpenGUI, monitorMem);
                //             // pthread_join(thread_id, NULL);
                //             OpenGUI(monitorMem);
                //         }
                //     }

                //     //Read the registers
                //     ReadRegs(regs);
                //     //Close and deallocate file
                //     fclose(fbuild);
                //     //pthread_exit(NULL);

                // } else
                //      fprintf(stderr, "[-] 6502asm raised an error: Error with finding file: %s\n", argv[2]);

                // //== Memory deallocation ==
                // if (instruction)
                //     free(instruction);

                // //Free the monitor memory
                // free(monitorMem);
                // //Free the labels list
                // FreeLBLBuffers(lblList, lblList->size);
                // free(lblList);
                #pragma endregion
            }
        }
    }
    return 0;
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