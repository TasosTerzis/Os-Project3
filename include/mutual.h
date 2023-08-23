#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#define TOTAL_FILES 10   // fileserver has exactly TOTAL_FILES files 
#define TOTAL_LINES 100  // each file has 20 exactly TOTAL_LINES lines
#define BLOCK_SIZE 100   // equal to MAX_LINE_LENGTH. (The existing files have about 70 characters per line.)
#define MAX_QUEUE_SIZE 100 // Maximum number of elements in the queue


struct request {
    pid_t pid; // client pid
    int fileNum;
    int start, stop;
    struct timeval requestTime; // time of request
};
typedef struct request Request;

struct shared_memory {
    sem_t mutex;
    sem_t empty;   
    sem_t full;
    int in, out, size;
    Request array[MAX_QUEUE_SIZE];
};
typedef struct shared_memory* SharedMemory;

void customer (SharedMemory, int K, int L, float l);