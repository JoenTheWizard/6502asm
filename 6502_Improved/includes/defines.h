//Init assembly memory
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ASM_MEMORY 4096

//Init ROM
#define ROM 0x07FF

//Define console clear ANSI escape codes
#define CNSCLR() printf("\033[H\033[J")

//Label length
#define LBL_LENGTH 64

//Break function
//#define BREAK