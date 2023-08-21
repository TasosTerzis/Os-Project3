#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#define TOTAL_FILES 10   // fileserver has exactly TOTAL_FILES files 
#define TOTAL_LINES 100  // each file has 20 exactly TOTAL_LINES lines
#define BLOCK_SIZE 100   // equal to MAX_LINE_LENGTH. (The existing files have about 70 characters per line.)


// Define REQUESTS structure
typedef struct request* Request;
struct request {
    pid_t pid; // client pid
    int fileNum;
    int start, stop;
    time_t requestTime;
    time_t completeTime;
    Request next;
};

// fifo dynamic queue to store requests
struct queue {
    Request front;
    Request rear;
};
typedef struct queue* Queue;

struct shared_memory {
    sem_t serverSem;
    sem_t clientSem[10];
    sem_t notComplete;
    Queue queue;
};
typedef struct shared_memory* SharedMemory;

void foo ();