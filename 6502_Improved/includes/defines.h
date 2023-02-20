//Init assembly memory
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

//Help menu
#define HELP "Usage: 6502asm [options...]\n \
-b, --build <assembly-file> [-v | --visual]  build, assemble and then run 6502 assembly file\n\n\
6502asm is not yet complete and still has some plans to add more features such as\n\
support for all opcodes.\n"

#define ASM_MEMORY 4096

//Init ROM
#define ROM 0x07FF

//Define console clear ANSI escape codes
#define CNSCLR() printf("\033[H\033[J")

//Label length
#define LBL_LENGTH 64

//Break function
//#define BREAK