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
#include <pthread.h>
#include <string.h>
#define TOTAL_FILES 10   // fileserver has exactly TOTAL_FILES files 
#define TOTAL_LINES 100  // each file has 20 exactly TOTAL_LINES lines
#define BLOCK_SIZE 100   // equal to MAX_LINE_LENGTH. (The existing files have about 70 characters per line.)
#define MAX_QUEUE_SIZE 30 // Maximum number of elements in the queue


struct request {
    pid_t pid; // process id
    int shmTempId; // shared memory id
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

struct temp_shared_memory {
    char array[BLOCK_SIZE];
    sem_t mutex;
    sem_t dataReady;
    sem_t dataEaten;
};
typedef struct temp_shared_memory* TempSharedMemory;

void customer (SharedMemory, int, int, float, int);

void* serverThread (void*);