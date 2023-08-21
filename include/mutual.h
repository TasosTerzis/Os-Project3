#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#define TOTAL_FILES 10   // fileserver has exactly TOTAL_FILES files 
#define TOTAL_LINES 100  // each file has 20 exactly TOTAL_LINES lines
#define BLOCK_SIZE 100   // equal to MAX_LINE_LENGTH. (The existing files have about 70 characters per line.)

void foo ();